#ifndef SILO
#define SILO

int setupVariables(global *global);
int printSoundInfo(SF_INFO *info);
int averageChannels(global *global);
int generateLandscape(global *global);
int setupAllegro(global *global);
int drawAllegro(fftw_complex *in, global *global);
int scaleFreq(fftw_complex *in, global *global);
int closeAllegro(global *global);
int normaliseImage(fftw_complex *in, int size);
float mapValue(float value, float oldMin, float oldMax, float newMin, float newMax);

#endif