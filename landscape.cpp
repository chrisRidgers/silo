#include <fftw3.h>
#include <sndfile.h>
#include <allegro5/allegro.h>
#include <getopt.h>
#include "globals.h"
#include "landscape.h"

landscape::landscape(struct global *global, int width, int height)
{
  this->width 	= width;
  this->height 	= height;
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
  input = i;

  return 0;
}

int landscape::setOutput(char *o)
{
  output = o;

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

char**  landscape::getInput()
{
  return &input;
}

char** landscape::getOutput()
{
  return &output;
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
