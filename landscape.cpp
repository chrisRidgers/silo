#include <complex.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <fftw3.h>
#include <sndfile.h>
#include <allegro5/allegro.h>
#include <getopt.h>
#include "globals.h"
#include "landscape.h"
#include "silo.h"

landscape::landscape(struct global *global, int width, int height)
{
  this->width 	= width;
  this->height 	= height;
  this->size 	= this->width * this->height;
  this->output	= "output/";

  //Defines file output path based on time
  time_t t = time(0);
  struct tm *now = localtime(&t);
  stringstream ss;
  ss 
    << now->tm_mday 
    << now->tm_mon+1 
    << now->tm_year+1900 
    << now->tm_hour 
    << now->tm_min 
    << now->tm_sec
    << ".obj";
  output += ss.str();

  object.resize(size);
  for(int i = 0; i < object.size(); i++)
  {
    object[i].resize(3);
  }

  landscape::setupVerts();
}

landscape::~landscape()
{
  fftw_destroy_plan(p);
  fftw_destroy_plan(p2);
  fftw_free(image);
  fftw_free(image2);
}

int landscape::setWidth(int w)
{
  width = w;

  return 0;
}

int landscape::setHeight(int h)
{
  height = h;

  return 0;
}

int landscape::setInput(char *i)
{
  input = string(i);

  return 0;
}

int landscape::setOutput(char *o)
{
  output = string(o);

  return 0;
}

int landscape::setSmooth(float s)
{
  smooth = s;

  return 0;
}

int landscape::getWidth()
{
  return width;
}

int landscape::getHeight()
{
  return height;
}

string landscape::getInput()
{
  return input;
}

string landscape::getOutput()
{
  return output;
}

float landscape::getSmooth()
{
  return smooth;
}

int landscape::setInfile(SNDFILE *i)
{
  infile = i;

  return 0;
}

int landscape::setInInfo(SF_INFO i)
{
  inInfo = i;

  return 0;
}

SNDFILE* landscape::getInfile()
{
  return infile;
}

SF_INFO* landscape::getInInfo()
{
  return &inInfo;
}

int landscape::setSoundSamplesBuffer(long frames)
{
  soundSamples = (float*) malloc(frames * sizeof(float));

  return 0;
}

float* landscape::getSoundSamplesBuffer()
{
  return soundSamples;
}

int landscape::setMaxSeek(long s)
{
  maxSeek = s;

  return 0;
}

long landscape::getMaxSeek()
{
  return maxSeek;
}

int landscape::setSeek(long s)
{
  seek = s;

  return 0;
}

long landscape::getSeek()
{
  return seek;
}

int landscape::setImageBuffer(int samples)
{
  image = fftw_alloc_complex(samples);

  return 0;
}

fftw_complex* landscape::getImageBuffer()
{
  return image;
}

int landscape::setImageBuffer2(int samples)
{
  image2 = fftw_alloc_complex(samples);

  return 0;
}

fftw_complex* landscape::getImageBuffer2()
{
  return image2;
}

int landscape::setPlan()
{
  p = fftw_plan_dft_2d(width, height, image, image2, FFTW_FORWARD, FFTW_ESTIMATE);

  return 0;
}

fftw_plan* landscape::getPlan()
{
  return &p;
}

int landscape::setPlan2()
{
  p2 = fftw_plan_dft_2d(width, height, image2, image, FFTW_BACKWARD, FFTW_ESTIMATE);

  return 0;
}

fftw_plan* landscape::getPlan2()
{
  return &p2;
}

int landscape::setupVerts()
{
  int countX = 0;
  int countY = 0;

  for(int i = 0; i < object.size(); i++)
  {
    object[i][0] = (double)countX / 32.0;
    object[i][1] = (double)countY / 32.0;

    countX++;
    if(countX >= width)
    {
      countY++;
      countX = 0;
    }
  }

  return 0;
}

int landscape::saveLandscape(global *global)
{
  ofstream outputStream;
  //cout << "Landscape Test1" << endl;
  outputStream.open(output);
  if(outputStream.is_open())
  {
    //cout << "Landscape Test2" << endl;
    //output << "Hello File World!\n";
  }else
  {
    return 1;
  }

  for(int i=0; i<object.size(); i++)
  {
    //cout << "v\t" << vert[0] << "\t" << vert[1] << "\t" << vert[2] << "\n";
    outputStream << "v\t" << object[i][0] << "\t" << object[i][1] << "\t" 
      << object[i][2] << "\n";
  }

  outputStream<< "\n";
  /*
     for(int i=1; i<sizeX*sizeY-sizeX; i++)
     {
     cout << "f\t" << i << " " << i+sizeX << " " << i+sizeX+1 << " " << i+1 
     << " \n";
     output << "f\t" << i << " " << i+sizeX << " " << i+sizeX+1 << " " << i+1 
     << " \n";
     }
     */
  for(int i=1; i<object.size()-width-1; i++)
  { 
    if(i%width!=0)outputStream << "f\t" << i << " " << i+width<< " " 
      << i+width+1 << " " << i+1 << " \n"; 
  }
  outputStream.close();
  return 0;
}

int landscape::setHeights(global *global)
{
  float little 	= 1.0;
  float big	= 0.0;
  
  for(int i = 0; i < size; i++)
  {
    complex<double> v(image[i][0], image[i][1]);
    double value;
    ::linearMap(abs(v), value, little, big, 0, 1.0);

    little 	= (little < value) ? value : little;
    big 	= (big < value) ? big : value;
  }


  for(int i = 0; i < size; i++)
  {
    complex<double> v(image[i][0], image[i][1]);
    double value;
    ::linearMap(abs(v), value, little, big, 0, 1.0);
    object[i][2] = value;
  }

  return 0;
}
