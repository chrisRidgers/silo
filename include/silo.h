#ifndef SILO
#define SILO

int setupVariables(global *global, landscape *test);
int printSoundInfo(SF_INFO *info);
int averageChannels(global *global, landscape *test);
int generateLandscape(global *global, landscape *test);
int setupAllegro(global *global, landscape *test);
int drawAllegro(fftw_complex *in, global *global, landscape *test);
int scaleFreq(fftw_complex *in, global *global, landscape *test);
int closeAllegro(global *global);
int normaliseImage(fftw_complex *in, int size);
int linearMap(double value, double &result, double oldMin, double oldMax, double newMin, double newMax);

#endif
