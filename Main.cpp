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
  fread(&test.RIFF,4,1,fp);
  fread(&test.fileSize,4,1,fp);
  fread(&test.WAVE,4,1,fp);
  fread(&test.fmt,4,1,fp);
  fread(&test.chunkSize,4,1,fp);
  fread(&test.audioFormat,2,1,fp);
  fread(&test.numOfChannels,2,1,fp);
  fread(&test.samplesPerSecond,4,1,fp);
  fread(&test.bytesPerSecond,4,1,fp);
  fread(&test.blockAlign,2,1,fp);
  fread(&test.bitsPerSample,2,1,fp);
  fread(&test.subchunk2ID,4,1,fp);
  fread(&test.subchunk2Size,4,1,fp);

  if(strncmp(test.RIFF, "RIFF",4)!=0)
  {
    errormessage("Error: Not RIFF format.\n");
  }
  if(strncmp(test.WAVE, "WAVE",4)!=0)
  {
    errormessage("Error: Not .wav format.\n");
  }
  if(strncmp(test.fmt, "fmt ",4)!=0)
  {
    errormessage("Error: fmt error.\n");
  }

  cout<<"File Size: "<<test.fileSize<<endl;
  cout<<"Chunk Size: "<<test.chunkSize<<endl;
  cout<<"Format Type: "<<test.audioFormat<<endl;
  cout<<"Channels: "<<test.numOfChannels<<endl;
  cout<<"Sample Rate: "<<test.samplesPerSecond<<endl;
  cout<<"Bytes Per Sec: "<<test.bytesPerSecond<<endl;
  cout<<"Bits Per Sample: "<<test.bitsPerSample<<endl;

  while(cin.get()!=10);



  return 0;
}
int main()
{
  loadWaveFile((char *)"track.wav");
  return 0;
}
