#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
using namespace std;

class Wave{
  public:
    char RIFF[4];
    uint32_t filesize;
    char WAVE[4];
    char fmt[4];
    uint32_t chunksize;
    uint16_t audioFormat;
    uint16_t numOfChannels;
    uint32_t samplesPerSecond;
    uint32_t bytesPerSecond;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2ID[4];
    uint32_t subchunk2Size;
    char* soundData;
    
    Wave(){}
};

int errormessage(const char* msg, int error=0){
  cout<<msg<<endl;
  return error;
}

int loadWaveFile(char *fname){
  FILE* fp = fopen(fname,"rb");
  if(!fp){
    perror("open");
    return errormessage("Error: cannot open file");
  }

  Wave w; 
  fread(&w,sizeof(w),1,fp);
  w.soundData=(char*)malloc(w.subchunk2Size);
  fseek(fp,44,SEEK_SET);
  cout<<fread(w.soundData, w.subchunk2Size,1,fp)<<endl;

  if(strncmp(w.RIFF, "RIFF",4)!=0)
  {
    errormessage("Error: Not RIFF format.\n");
  }
  if(strncmp(w.WAVE, "WAVE",4)!=0)
  {
    errormessage("Error: Not .wav format.\n");
  }
  if(strncmp(w.fmt, "fmt ",4)!=0)
  {
    errormessage("Error: fmt error.\n");
  }

  cout<<"File Size:\t\t "<<w.filesize<<endl;
  cout<<"Data Size:\t\t "<<w.subchunk2Size<<endl;
  cout<<"Chunk Size:\t\t "<<w.chunksize<<endl;
  cout<<"Format Type:\t\t "<<w.audioFormat<<endl;
  cout<<"Channels:\t\t "<<w.numOfChannels<<endl;
  cout<<"Sample Rate:\t\t "<<w.samplesPerSecond<<endl;
  cout<<"Bytes Per Sec:\t\t "<<w.bytesPerSecond<<endl;
  cout<<"Bits Per Sample:\t "<<w.bitsPerSample<<endl;
  
  return 0;
}
int main()
{
  loadWaveFile((char *)"track.wav");
  return 0;
}
