#include <getopt.h>
#include <fftw3.h>
#include <sndfile.h>
#include <allegro5/allegro.h>
#include "globals.h"

int global::setWidth(int w)
{
  width = w;

  return 0;
}

int global::setHeight(int h)
{
  height = h;

  return 0;
}

int global::setInput(char *i)
{
  input = i;

  return 0;
}

int global::setOutput(char *o)
{
  output = o;

  return 0;
}

int global::setSmooth(float s)
{
  smooth = s;

  return 0;
}

int global::getWidth()
{
  return width;
}

int global::getHeight()
{
  return height;
}

char**  global::getInput()
{
  return &input;
}

char** global::getOutput()
{
  return &output;
}

float global::getSmooth()
{
  return smooth;
}

int global::setDisplay(ALLEGRO_DISPLAY *d)
{
  display = d;
  
  return 0;
}

ALLEGRO_DISPLAY* global::getDisplay()
{
  return display;
}

int global::setInfile(SNDFILE *i)
{
  infile = i;

  return 0;
}

int global::setInInfo(SF_INFO i)
{
  inInfo = i;

  return 0;
}

SNDFILE* global::getInfile()
{
  return infile;
}

SF_INFO* global::getInInfo()
{
  return &inInfo;
}

int global::setSoundSamplesBuffer(long frames)
{
  soundSamples = (float*) malloc(frames * sizeof(float));

  return 0;
}

float* global::getSoundSamplesBuffer()
{
  return soundSamples;
}

int global::setMaxSeek(long s)
{
  maxSeek = s;

  return 0;
}

long global::getMaxSeek()
{
  return maxSeek;
}

int global::setSeek(long s)
{
  seek = s;

  return 0;
}

long global::getSeek()
{
  return seek;
}

int global::setImageBuffer(int samples)
{
  image = fftw_alloc_complex(samples);

  return 0;
}

fftw_complex* global::getImageBuffer()
{
  return image;
}

int global::setImageBuffer2(int samples)
{
  image2 = fftw_alloc_complex(samples);

  return 0;
}

fftw_complex* global::getImageBuffer2()
{
  return image2;
}

int global::setPlan()
{
  p = fftw_plan_dft_2d(width, height, image, image2, FFTW_FORWARD, FFTW_ESTIMATE);

  return 0;
}

fftw_plan* global::getPlan()
{
  return &p;
}

int global::setPlan2()
{
  p2 = fftw_plan_dft_2d(width, height, image2, image, FFTW_BACKWARD, FFTW_ESTIMATE);

  return 0;
}

fftw_plan* global::getPlan2()
{
  return &p2;
}


