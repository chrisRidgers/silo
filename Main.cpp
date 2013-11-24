#include <iostream>
using namespace std;

class AudioAnalyse{
  public:
  void testInOut(char[]);
};
void AudioAnalyse::testInOut(){
  char[] path = args;
  FILE *ifp = fopen(path, "r");
  FILE *ofp = fopen("output.wav", "w");
  fclose(ifp);
  fclose(ofp);
}

int main()
{
  AudioAnalyse test;
  test.testInOut("track.mp3");

  
}
