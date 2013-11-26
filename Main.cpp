#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
using namespace std;

class WavData{
  public:
    short* data;
    unsigned long size;

    char RIFF[4];
    char WAVE[4];
    char fmt[4];
    char subchunk2ID[4];
    uint32_t fileSize, chunkSize, samplesPerSecond, bytesPerSecond, subchunk2Size;
    uint16_t audioFormat, numOfChannels, blockAlign, bitsPerSample;

    WavData(){
    }
};

int errormessage(const char* msg, int error=0){
  cout<<msg<<endl;
  while(cin.get()!=10);

  return error;
}

int loadWaveFile(char *fname){
  FILE* fp = fopen(fname,"r");
  if(!fp){
    perror("open");
    return errormessage("Error: cannot open file");
  }

  WavData test;
  
  cout<<test.RIFF<<endl;
  cout<<test.WAVE<<endl;
  cout<<test.fmt<<endl;
  if(!strcmp(test.RIFF, "RIFF"))
  {
    errormessage("Error: Not RIFF format.\n");
  }
  if(!strcmp(test.WAVE, "WAVE"))
  {
    errormessage("Error: Not .wav format.\n");
  }
  if(!strcmp(test.fmt, "fmt "))
  {
    errormessage("Error: fmt error.\n");
  }

  cout<<"File Size: "<<test.fileSize<<endl;
  cout<<"Chunk Size: "<<test.chunkSize<<endl;
  cout<<"Format Type: "<<test.audioFormat<<endl;
  cout<<"Channels: "<<test.numOfChannels<<endl;
  cout<<"Sample Rate: "<<test.samplesPerSecond<<endl;
  cout<<"Bytes Per Sec: "<<test.bytesPerSecond<<endl;

  while(cin.get()!=10);



  return 0;
}
int main()
{
  loadWaveFile((char *)"track.wav");
  return 0;
}
