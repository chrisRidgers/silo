#include <stdio.h>
#include <string>
#include <vector>
#include <getopt.h>
#include <complex.h>
#include <time.h>
#include <fftw3.h>
#include <sndfile.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "globals.h"
#include "landscape.h"
#include "silo.h"

using namespace std;

int main(int argc, char **argv)
{
  struct global global{
    {
      {"input",		required_argument, 0, 'i'},
      {"output",	required_argument, 0, 'o'},
      {"smoothness",	required_argument, 0, 's'},
      {0, 0, 0, 0}
    },
      0,
      argc,
      argv
  };

  landscape *test = new landscape(&global, 512, 512);
  setupVariables(&global, test);
  generateLandscape(&global, test);
  test->saveLandscape(&global);
  delete test;

  closeAllegro(&global);

  return 0;
}

int setupVariables(global *global, landscape *test)
{
  int c;
  while(( c = getopt_long(
	  global->argc,
	  global->argv,
	  "i:o:s:",
	  global->long_options,
	  &global->option_index)) != -1)
  {
    switch(c)
    {
      case 0:
	if(global->long_options[global->option_index].flag !=0) break;

	fprintf(stdout, "Option %s", 
	    global->long_options[global->option_index].name);
	if(optarg) fprintf(stdout, " with arg %s", optarg);
	fprintf(stdout, "\n");
	break;

      case 'i':
	fprintf(stdout, "Option -i with value `%s'\n", optarg);
	global->inputReceived = 1;
	test->setInput(optarg);
	break;

      case 'o':
	fprintf(stdout, "Option -o with value `%s'\n",optarg);
	global->smoothReceived = 1;
	test->setOutput(optarg);
	break;

      case 's':
	fprintf(stdout, "Option -s with value `%s'\n", optarg);
	global->smoothReceived = 1;
	test->setSmooth(atof(optarg));
	break;

      case '?':
	break;

      default:
	abort();
    }
  }

  if(optind < global->argc)
  {
    fprintf(stderr, "non-option ARGV-elements: ");
    while(optind < global->argc)
      fprintf(stderr, "%s ", global->argv[optind++]);
    fputc('\n', stderr);
    exit(0);
  }

  if(!global->smoothReceived || !test->getSmooth()
      || !global->inputReceived) exit(0);

  test->setWidth(512);
  test->setHeight(512);

  test->setInfile(sf_open(test->getInput().c_str(), SFM_READ, 
	test->getInInfo()));
  if(!test->getInfile()) 
  {
    fprintf(stderr, "Invalid input file\n");
    sf_close(test->getInfile());
    exit(0);
  }
  //printSoundInfo(test->getInInfo());

  test->setMaxSeek(test->getInInfo()->frames - test->getWidth() 
      * test->getHeight());

  srand(time(NULL));
  test->setSeek(((float) test->getMaxSeek() * rand()) / RAND_MAX);
  test->setSoundSamplesBuffer(test->getWidth() * test->getHeight() 
      * test->getInInfo()->channels);
  test->setImageBuffer(test->getWidth() * test->getHeight());
  test->setImageBuffer2(test->getWidth() * test->getHeight());
  test->setPlan();
  test->setPlan2();

  
  setupAllegro(global, test);


  return 0;
}

int setupAllegro(global *global, landscape *test)
{
  if(!al_init())
  { 
    fprintf(stderr, "failed to intialise allegro!\n");
    return -1;
  }

  if(!al_init_primitives_addon())
  {
    fprintf(stderr, "failed to initialize allegro primitves \
	addon!\n");
    return -1;
  }

  global->display = al_create_display(test->getWidth(), 
      test->getHeight());

  if(!global->display)
  {
    fprintf(stderr, "failed to intialise allegro display!\n");
    return -1;
  }
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_flip_display();

  return 0;
}

int generateLandscape(global *global, landscape *test)
{
  sf_seek(test->getInfile(), test->getSeek(), SEEK_SET);

  sf_read_float(test->getInfile(), test->getSoundSamplesBuffer(), 
      test->getWidth() * test->getHeight() * test->getInInfo()->channels);

  averageChannels(global, test);
  drawAllegro(test->getImageBuffer(), global, test);

  fftw_execute(*test->getPlan());
  //drawAllegro(test->getImageBuffer2(), global, test);

  for(int i = 0; i < test->getWidth() * test->getHeight(); i++)
  {
    complex<double> v(test->getImageBuffer2()[i][0], 
	test->getImageBuffer2()[i][1]);
    test->getImageBuffer2()[i][1] *= 1.0 / 
      sqrt((test->getWidth() * test->getHeight()));
    test->getImageBuffer2()[i][1] *= 1.0 / 
      sqrt((test->getWidth() * test->getHeight()));
  }
  //drawAllegro(test->getImageBuffer2(), global, test);

  scaleFreq(test->getImageBuffer2(), global, test);
  //drawAllegro(test->getImageBuffer2(), global, test);

  fftw_execute(*test->getPlan2());
  drawAllegro(test->getImageBuffer(), global, test);

  test->setHeights(global);

  return 0;
}

int drawAllegro(fftw_complex *in, global *global, landscape *test)
{
  int size	= test->getWidth() * test->getHeight();
  int sizeX 	= test->getWidth() - 1;
  int sizeY 	= test->getHeight() - 1;
  int posX 	= 0;
  int posY 	= 0;
  float big 	= 0.0;
  float small 	= 1.0;

  for(int i = 0; i < size; i++)
  {
    complex<double> z(in[i][0], in[i][1]);
    if(abs(z) > big) 	big 	= abs(z);
    if(abs(z) < small) 	small 	= abs(z);
  }

  for(int i = 0; i < size; i++)
  {
    complex<double> z(in[i][0], in[i][1]);
    /*
       fprintf(
       stdout, 
       "\
       posX \t %d \n \
       posY \t %d \n \
       out[r] \t %f \n \
       out[i] \t %f \n \
       out[a] \t %f \n \
       out[b] \t %f \n \
       big \t %f \n \
       small \t %f \n",
       posX,
       posY,
       in[i][0],
       in[i][1],
       abs(z),
       abs(z) / 512,
       big,
       small);
       */

    double value;
    linearMap(abs(z), value, small, big, 0.0, 1.0); 
    al_draw_line(
	posX + 0.5, 
	posY, 
	posX + 0.5, 
	posY + 1, 
	al_map_rgb(
	  value * 255, 
	  value * 255, 
	  value * 255), 
	1);

    posX++;
    if(posX > sizeX)
    {
      posY++;
      posX = 0;
    }
  }

  al_flip_display();
  al_rest(5);

  return 0;
}

int scaleFreq(fftw_complex *in, global *global, landscape *test)
{
  int size	= test->getWidth() * test->getHeight();
  int sizeX 	= test->getWidth() - 1;
  int sizeY 	= test->getHeight() - 1;
  int posX 	= 0;
  int posY 	= 0;

  for(int i = 0; i < size; i++)
  {
    double r = sqrt(pow(posX, 2) + pow(posY, 2));
    if(r != 0.0)
    {
      in[i][0] *= 1.0/pow(r, test->getSmooth());
      in[i][1] *= 1.0/pow(r, test->getSmooth());
    }
    else
    {
      in[i][0] *= 0;
      in[i][1] *= 0;
    }

    posX++;
    if(posX > sizeX)
    {
      posY++;
      posX = 0;
    }
  }

  return 0;
}

int closeAllegro(global *global)
{
  al_destroy_display(global->display);

  return 0;
}

int normaliseImage(fftw_complex *in, int size)
{
  for(int i = 0; i < size; i++)
  {
    in[i][0] *= 1.0 / 255;
    in[i][1] *= 1.0 / 255;
  }

  return 0;
}

int printSoundInfo(SF_INFO *info)
{
  fprintf(stdout, "Sound frames: \t %lld \n \
      Sample Rate: \t %d \n \
      Channels: \t %d \n \
      Format: \t %d \n \
      Sections: \t %d \n \
      Seekable: \t %d \n",
      info->frames,
      info->samplerate,
      info->channels,
      info->format,
      info->sections,
      info->seekable);

  return 0;
}

int averageChannels(global *global, landscape *test)
{
  double biggest = 0.0;
  double smallest = 1.0;
  for(int i = 0; i < test->getHeight() * test->getWidth(); i++)
  {
    test->getImageBuffer()[i][0] = 0;
    test->getImageBuffer()[i][1] = 0;
    for(int ch = 0; ch < test->getInInfo()->channels ; ch++)
    {
      test->getImageBuffer()[i][0] += 
	test->getSoundSamplesBuffer()
	[i * test->getInInfo()->channels + ch];
      //fprintf(stdout, "SoundSamplesBuffer()[i] : %f \n",
      //   test->getSoundSamplesBuffer()
      //  [i * test->getInInfo()->channels + ch]);
    }

    test->getImageBuffer()[i][0] /= test->getInInfo()->channels;

    if(test->getImageBuffer()[i][0] > biggest) 
      biggest = test->getImageBuffer()[i][0];

    if(test->getImageBuffer()[i][0] < smallest)
      smallest = test->getImageBuffer()[i][0];

    // fprintf(stdout, "%-15s\t:\t% f\n", "original", 
    // test->getImageBuffer()[i][0]);

  }

  for(int i = 0; i < test->getHeight() * test->getWidth(); i++)
  {
    double value;
    linearMap(test->getImageBuffer()[i][0], value, smallest, biggest, 0.0, 
	255.0);
    test->getImageBuffer()[i][0] = value;

    //fprintf(stdout, "%-15s\t:\t% f\n", "mapped", 
    //test->getImageBuffer()[i][0]);
  }

  return 0;
}

int linearMap(double value, double &result, double oldMin, double oldMax, 
    double newMin, double newMax)
{ 
  result = (value - oldMin) * ((newMax - newMin) / (oldMax - oldMin)) + newMin;

  return 0;
}
