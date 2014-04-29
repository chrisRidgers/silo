#include <complex.h>
#include <ctime>
#include <fftw3.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sndfile.h>
#include <sstream>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdint.h>
#include <string.h>
#include <valarray>
#include <vector>


enum{APP, SIZEX, SIZEY, INPUT, OUTPUT};

using namespace std;

int saveLandscape();
int setupVerts();
int soundInput();
int testfftw();
int offsetFreq(fftw_complex *in, int size);
int scaleFreq(fftw_complex *in, int size);
int closeAllegro();
int initialiseAllegro();
int drawAllegro(fftw_complex *in, int size);

int sizeX = 256, sizeY = 256;
char *infilename;
string pathOut = "output/";
valarray<float> landscape;

SNDFILE *input;
SF_INFO sfinfo;

ALLEGRO_DISPLAY *display = NULL;

int main(int argc, char **argv)
{
  initialiseAllegro();

  if(argv[SIZEX]!=NULL && argv[SIZEY]!=NULL){
    sizeX = atoi(argv[SIZEX]);
    sizeY = atoi(argv[SIZEY]);
  }
  landscape.resize((sizeX-1)*(sizeY-1)*3);

  infilename = (char*)argv[INPUT];
  soundInput();

  //Defines file output path based on time
  time_t t = time(0);
  struct tm * now = localtime(&t);
  stringstream ss;
  ss 
    << now->tm_mday 
    << now->tm_mon+1 
    << now->tm_year+1900 
    << now->tm_hour 
    << now->tm_min 
    << now->tm_sec
    << ".obj";
  pathOut += ss.str();
  landscape.resize(sizeX*sizeY*3);

  /*
  cout << "Size X:\t\t\t" << sizeX << endl;
  cout << "Size Y:\t\t\t" << sizeY << endl;
  cout << "Landscape Size:\t\t\t" << landscape.size() << endl;
  cout << "Output:\t\t\t" << pathOut << endl;
  cout << "Input Sound File:\t\t\t" << infilename << endl;
  */

  setupVerts();
  //cout << "setupVerts complete" << endl;
  // for(int i=0;i<landscape.size();i++) cout << landscape[i] << endl;
  saveLandscape();
  /*
  cout << "saveLandscape complete" << endl;
  cout << "Input Sample Rate:\t\t\t" << sfinfo.samplerate << endl;
  cout << "Channels:\t\t\t" << sfinfo.channels<< endl;
  cout << "Format:\t\t\t" << sfinfo.format<< endl;
  cout << "Sections:\t\t\t" << sfinfo.sections<< endl;
  cout << "Seekable:\t\t\t" << sfinfo.seekable<< endl;
  */

  testfftw();

  return 0;
}

int saveLandscape()
{
  ofstream output;
  //cout << "Landscape Test1" << endl;
  output.open(pathOut);
  if(output.is_open())
  {
    //cout << "Landscape Test2" << endl;
    //output << "Hello File World!\n";
  }else
  {
    return 1;
  }

  for(int i=0; i<landscape.size()-1; i+=3)
  {
    valarray<float> vert = landscape[slice(i,3,1)];
    //cout << "v\t" << vert[0] << "\t" << vert[1] << "\t" << vert[2] << "\n";
    output << "v\t" << vert[0] << "\t" << vert[1] << "\t" << vert[2] << "\n";
  }

  output << "\n";
  /*
     for(int i=1; i<sizeX*sizeY-sizeX; i++)
     {
     cout << "f\t" << i << " " << i+sizeX << " " << i+sizeX+1 << " " << i+1 
     << " \n";
     output << "f\t" << i << " " << i+sizeX << " " << i+sizeX+1 << " " << i+1 
     << " \n";
     }
     */
  for(int i=1; i<landscape.size()/3-sizeX-1; i++)
  { 
    if(i%sizeX!=0)output << "f\t" << i << " " << i+sizeX << " " << i+sizeX+1 
      << " " << i+1 << " \n"; 
  }
  output.close();
  return 0;
}

int setupVerts()
{
  float countX = 0.0f;
  float countY = 0.0f;

  for(int i = 0; i < landscape.size(); i += 3)
  {
    landscape[i] 	= countX / 16;
    countX 		= countX >= ((sizeX)-1) ? 0 : countX+=1;
    //cout << countX << endl;
  }

  //countX = 0;
  //
  //test

  for(int i = 1; i < landscape.size(); i += sizeY * 3)
  {
    landscape[slice(i, sizeY, 3)] 	= countY / 16;
    countY 				+= 1;
  }

  return 0;
}

int soundInput()
{
  if(!(input = sf_open(infilename, SFM_READ, &sfinfo)))
  {
    printf("Not able to open input file %s.\n", infilename);
    puts(sf_strerror(NULL));
    return 1;
  }else
  {
    return 0;
  }
}

int testfftw()
{
  fftw_complex  *in;
  fftw_complex 	*out;
  fftw_plan 	p, p2;

  in 	= (fftw_complex*) fftw_malloc(256 * 256 * sizeof(fftw_complex));
  out 	= (fftw_complex*) fftw_malloc(256 * 256 * sizeof(fftw_complex));

  srand(time(NULL));
  for(int i = 0; i < 256 * 256; i++)
  {
    in[i][0] = rand() % 255;
    //cout << "in test " << i << " value: " << in[i] << endl;
  }

  //Draw values to allegro
  drawAllegro(in, 256 * 256);

  p = fftw_plan_dft_2d(256, 256, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(p);

  drawAllegro(out, 256 * 256);
  
  for(int i = 0; i < 256 * 256; i++)
  {
    out[i][0] = 1.0 / 255 * out[i][0];
    out[i][1] = 1.0 / 255 * out[i][1];
  }
  drawAllegro(out, 256 * 256);

  offsetFreq(out, 256 * 256);
  drawAllegro(out, 256 * 256);
  
  scaleFreq(out, 256 * 256);
  drawAllegro(out, 256 * 256);

  p2 = fftw_plan_dft_2d(256, 256, out, in, FFTW_BACKWARD, FFTW_ESTIMATE);
  fftw_execute(p2);

  drawAllegro(in, 256 * 256);

  fftw_destroy_plan(p);
  fftw_destroy_plan(p2);
  fftw_free(in);
  fftw_free(out);

  closeAllegro();

  return 0;
}

int initialiseAllegro()
{
  if(!al_init())
  {
    fprintf(stderr, "failed to initialize allegro!\n");
    return -1;
  }

  if(!al_init_primitives_addon())
  {
    fprintf(stderr, "failed to initialize allegro primitves \
	addon!\n");
    return -1;
  }

  display = al_create_display(256, 256);
  if(!display)
  {
    fprintf(stderr, "failed to create display!\n");
    return -1;
  }

  al_clear_to_color(al_map_rgba_f(0, 0, 0, 0.1));
  al_flip_display();

  return 0;
}

int closeAllegro()
{
  al_destroy_display(display);

  return 0;
}

int drawAllegro(fftw_complex *in, int size)
{
  int sizeX 	= 256;
  int sizeY 	= 256;
  int posX 	= 1;
  int posY 	= 1;

  for(int i = 0; i < size; i++)
  {
    fprintf(
	stdout, 
	"\
	posX \t %d \n \
	posY \t %d \n \
	out[r] \t %f \n \
	out[i] \t %f \n",
	posX,
	posY,
	in[i][0],
	in[i][1]);
    al_draw_line(
	posX + 0.5, 
	posY, 
	posX + 0.5, 
	posY + 1, 
	al_map_rgb(abs(in[i][0]), abs(in[i][0]), abs(in[i][0])), 
	1);
    posX++;
    if(posX > sizeX)
    {
      posY++;
      posX = 1;
    }
  }

  al_flip_display();
  al_rest(5);

  return 0;
}

int scaleFreq(fftw_complex *in, int size)
{
  int sizeX 	= 256;
  int sizeY 	= 256;
  int posX 	= 1;
  int posY 	= 1;
  
  for(int i = 0; i < size; i++)
  {
    in[i][0] *= 1.0/pow(posX + posY, 1.4);
    in[i][1] *= 1.0/pow(posX + posY, 1.4);

    posX++;
    if(posX > sizeX)
    {
      posY++;
      posX = 1;
    }
  }

  return 0;
}
int offsetFreq(fftw_complex *in, int size)
{
  int sizeX 	= 256;
  int sizeY 	= 256;
  int posX 	= 1;
  int posY 	= 1;

  for(int i = 0; i < size / 2; i++)
  {
    fftw_complex temp;
    
    temp[0] 		= in[i][0];
    temp[1] 		= in[i][1];

    in[i][0] 		= in[i+size/2][0];
    in[i][1] 		= in[i+size/2][1];

    in[i+size/2][0] 	= temp[0];
    in[i+size/2][1] 	= temp[1];

    posX++;
    if(posX > sizeX)
    {
      posY++;
      posX = 1;
    }
  }
  
  for(posY = 1;  posY < sizeY; posY++)
  {
    for(posX = 1; posX < sizeX / 2; posX++)
    {
      fftw_complex temp;
      fprintf(
	  stdout, 
	  "\
	  posY \t %d \n \
	  posX \t %d \n",
	 posY, posX);
	  

      temp[0] 					= in[posY * sizeX + posX][0];
      temp[1] 					= in[posY * sizeX + posX][1];

      in[posY * sizeX + posX][0] 		= 
	in[posY * sizeX + posX + sizeX/2][0];
      
      in[posY * sizeX + posX][1] 		= 
	in[posY * sizeX + posX + sizeX/2][1];

      in[posY * sizeX + posX + sizeX/2][0] 	= temp[0];
      in[posY * sizeX + posX + sizeX/2][1] 	= temp[1]; 
    }
  }

  return 0;
}

