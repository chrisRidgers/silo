#ifndef LANDSCAPE
#define LANDSCAPE

using namespace std;

class landscape
{
  int width;
  int height;
  int size;
  string input;
  string output;
  float smooth;

  SNDFILE *infile;
  SF_INFO inInfo;
  long maxSeek;
  long seek;
  float *soundSamples;
  
  fftw_complex *image;
  fftw_complex *image2;
  fftw_plan p, p2;

  vector<vector<double>> object;

  public:
  landscape(struct global *global, int width, int height);
  ~landscape();

  int setWidth(int w);
  int setHeight(int h);
  int setInput(char *i);
  int setOutput(char *o);
  int setSmooth(float s);
  int getWidth();
  int getHeight();
  string getInput();
  string getOutput();
  float getSmooth();

  int setInfile(SNDFILE *i);
  int setInInfo(SF_INFO);
  int setMaxSeek(long s);
  int setSeek(long s);
  int setSoundSamplesBuffer(long frames);
  int setImageBuffer(int samples);
  int setImageBuffer2(int samples);
  int setPlan();
  int setPlan2();

  SNDFILE* getInfile();
  SF_INFO* getInInfo();
  long getMaxSeek();
  long getSeek();
  float* getSoundSamplesBuffer();
  fftw_complex* getImageBuffer();
  fftw_complex* getImageBuffer2();
  fftw_plan* getPlan();
  fftw_plan* getPlan2();

  int setupVerts();
  int saveLandscape(global *global);
  int setHeights(global *global);
};
#endif
