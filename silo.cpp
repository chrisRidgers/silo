#include <stdio.h>
#include <complex.h>
#include <time.h>
#include <fftw3.h>
#include <sndfile.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "globals.h"
#include "silo.h"

using namespace std;

int main(int argc, char **argv)
{
  global global;
  setupVariables(&global);
  //generateLandscape(&global);
  
  sf_seek(global.getInfile(), global.getSeek(), SEEK_SET);
  sf_read_float(global.getInfile(), global.getSoundSamplesBuffer(), 
      global.getWidth() * global.getHeight() * global.getInInfo()->channels);
  
  averageChannels(&global);
  
  drawAllegro(global.getImageBuffer(), &global);
  
  fftw_execute(*global.getPlan());
  drawAllegro(global.getImageBuffer2(), &global);
  scaleFreq(global.getImageBuffer2(), &global);
  drawAllegro(global.getImageBuffer2(), &global);
  fftw_execute(*global.getPlan2());
  drawAllegro(global.getImageBuffer(), &global);
  

  closeAllegro(&global);

  return 0;
}

int setupVariables(global *global)
{
  global->setWidth(256);
  global->setHeight(256);

  global->setInfile(sf_open("music.wav", SFM_READ, global->getInInfo()));
  //printSoundInfo(global->getInInfo());

  global->setMaxSeek(global->getInInfo()->frames - global->getWidth() 
      * global->getHeight());
  
  srand(time(NULL));
  global->setSeek(((float) global->getMaxSeek() * rand()) / RAND_MAX);
  global->setSoundSamplesBuffer(global->getWidth() * global->getHeight() 
      * global->getInInfo()->channels);
  global->setImageBuffer(global->getWidth() * global->getHeight());
  global->setImageBuffer2(global->getWidth() * global->getHeight());
  global->setPlan();
  global->setPlan2();

  setupAllegro(global);

  return 0;
}

int setupAllegro(global *global)
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

  global->setDisplay(al_create_display(global->getWidth(), 
	global->getHeight()));

  if(!global->getDisplay())
  {
    fprintf(stderr, "failed to intialise allegro display!\n");
    return -1;
  }
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_flip_display();


  return 0;
}
                                         /*
int generateLandscape(global *global)
{
  fftw_complex  *in;
  fftw_complex 	*out;
  fftw_plan 	p, p2;

  in 	= (fftw_complex*) fftw_malloc(global->getWidth() * global->getHeight() 
      * sizeof(fftw_complex));
  out 	= (fftw_complex*) fftw_malloc(global->getWidth() * global->getHeight() 
      * sizeof(fftw_complex));

  srand(time(NULL));
  for(int i = 0; i < global->getWidth() * global->getHeight(); i++)
  {
    in[i][0] = rand() % 255;
    //cout << "in test " << i << " value: " << in[i] << endl;
  }

  //Draw values to allegro
  drawAllegro(in, global->getWidth() * global->getHeight());

  p = fftw_plan_dft_2d(global->getWidth(), global->getHeight(), in, out, 
      FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(p);

  scaleFreq(out, global->getWidth() * global->getHeight());
  drawAllegro(out, global->getWidth() * global->getHeight());

  p2 = fftw_plan_dft_2d(global->getWidth(), global->getHeight(), out, in, 
      FFTW_BACKWARD, FFTW_ESTIMATE);
  fftw_execute(p2);

  drawAllegro(in, global->getWidth() * global->getHeight());

  normaliseImage(in, global->getWidth() * global->getHeight());

  drawAllegro(in, global->getWidth() * global->getHeight());


  fftw_destroy_plan(p);
  fftw_destroy_plan(p2);
  fftw_free(in);
  fftw_free(out);


  return 0;
}
*/

int drawAllegro(fftw_complex *in, global *global)
{
  int size	= global->getWidth() * global->getHeight();
  int sizeX 	= global->getWidth() - 1;
  int sizeY 	= global->getHeight() - 1;
  int posX 	= 0;
  int posY 	= 0;
  float big 	= 0.0;
  float small 	= 0.0;
  
  for(int i = 0; i < size; i++)
  {
    complex<double> z(in[i][0], in[i][1]);
    if(abs(z) > big) 	big 	= abs(z);
    if(abs(z) < small) 	small 	= abs(z);
  }

  for(int i = 0; i < size; i++)
  {
    complex<double> z(in[i][0], in[i][1]);
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
	small \t %f \n \
	valueMapped \t %f \n",
	posX,
	posY,
	in[i][0],
	in[i][1],
	abs(z),
	sqrt(pow(in[i][0], 2) + pow(in[i][1], 2)),
	big,
	small,
	mapValue(abs(z), small, big, 0.0, 255.0));
    
    al_draw_line(
	posX + 0.5, 
	posY, 
	posX + 0.5, 
	posY + 1, 
	al_map_rgb(mapValue(abs(z), small, big, 0.0, 255.0),
	  mapValue(abs(z), small, big, 0.0, 255.0), 
	  mapValue(abs(z), small, big, 0.0, 255.0)), 
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

int scaleFreq(fftw_complex *in, global *global)
{
  int size	= global->getWidth() * global->getHeight();
  int sizeX 	= global->getWidth() - 1;
  int sizeY 	= global->getHeight() - 1;
  int posX 	= 0;
  int posY 	= 0;

  for(int i = 0; i < size; i++)
  {
    double r = sqrt(pow(posX, 2) + pow(posY, 2));
    if(r != 0.0)
    {
      in[i][0] *= 1.0/pow(r, 2.0);
      in[i][1] *= 1.0/pow(r, 2.0);
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
  al_destroy_display(global->getDisplay());

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

int averageChannels(global *global)
{
  for(int i = 0; i < global->getHeight() * global->getWidth(); i++)
  {
    global->getImageBuffer()[i][0] = 0;
    global->getImageBuffer()[i][1] = 0;
    for(int ch = 0; ch < global->getInInfo()->channels ; ch++)
    {
      global->getImageBuffer()[i][0] += 
	global->getSoundSamplesBuffer()[i * global->getInInfo()->channels + ch];
      fprintf(stdout, "SoundSamplesBuffer()[i] : %f \n",
	  global->getSoundSamplesBuffer()
	  [i * global->getInInfo()->channels + ch]);
    }
    global->getImageBuffer()[i][0] /= global->getInInfo()->channels;
  }
  return 0;
}

float mapValue(float value, float oldMin, float oldMax, float newMin, float newMax)
{
  float oldRange = oldMax - oldMin;
  float newRange = newMax - newMin;

  float mapped = value - newMin / oldRange * newRange + newMin;

  return mapped;
}
