#ifndef GLOBAL
#define GLOBAL

class global
{
  int width;
  int height;
  
  struct ALLEGRO_DISPLAY *display;

  SNDFILE *infile;
  SF_INFO inInfo;
  long maxSeek;
  long seek;
  float *soundSamples;
  
  fftw_complex *image;
  fftw_complex *image2;
  fftw_plan p, p2;

  public:
  global(){}
  int setWidth(int w);
  int setHeight(int h);
  int getWidth();
  int getHeight();

  int setDisplay(ALLEGRO_DISPLAY *d);
  ALLEGRO_DISPLAY* getDisplay();

  int setInfile(SNDFILE *i);
  int setInInfo(SF_INFO);
  int setMaxSeek(long s);
  int setSeek(long s);
  int setSoundSamplesBuffer(long frames);
  int setImageBuffer(int samples);
  int setImageBuffer2(int samples);
  
  SNDFILE* getInfile();
  SF_INFO* getInInfo();
  long getMaxSeek();
  long getSeek();
  float* getSoundSamplesBuffer();
  fftw_complex* getImageBuffer();
  fftw_complex* getImageBuffer2();
  int setPlan();
  fftw_plan* getPlan();
  int setPlan2();
  fftw_plan* getPlan2();
 
};

#endif
