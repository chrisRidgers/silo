#include <iostream>
#include <fstream>
using namespace std;

class WavData{
  public:
    short* data;
    unsigned long size;

    WavData(){
      data = NULL;
      size = 0;
    }
};

void loadWaveFile(char *fname, WavData *ret){
  FILE* fp = fopen(fname,"rb");
  if(fp){
    char id[5];
    unsigned long size;
    short format_tag, channels, block_align, bits_per_sample;
    unsigned long format_length, sample_rate, avg_bytes_sec, data_size;

    fread(id, sizeof(char), 4, fp);
    id[4] = '\0';

    if(!strcmp(id, "RIFF")){
      fread(&size, sizeof(char), 4, fp);
      fread(id, sizeof(char), 4, fp);
      id[4] = '\0';

      cout<<sizeof(char)<<"\n"<<sizeof(short)<<"\n"<<sizeof(unsigned long)<<"\n";

      if(!strcmp(id,"WAVE")){
	fread(id, sizeof(char), 4, fp);
	fread(&format_length, sizeof(char), 4, fp);
	fread(&format_tag, sizeof(short),1,fp);
	fread(&channels, sizeof(short),1,fp);
	fread(&sample_rate, sizeof(short),2,fp);
	fread(&avg_bytes_sec, sizeof(short),2,fp);
	fread(&block_align, sizeof(short),1,fp);
	fread(&bits_per_sample, sizeof(short),1,fp);
	fread(id, sizeof(char),4,fp);
	fread(&data_size, sizeof(short),2,fp);
	cout<<&data_size<<"\n";

	ret->size = data_size/sizeof(short);
	ret->data = (short*) malloc(data_size);
	fread(ret->data, sizeof(short), ret->size, fp);
      }
      else {
	cout<<"Error: RIFF file but not a WAVE file\n";
      }
    }
    else {
      cout<<"Error: not a RIFF file\n";
    }
  }
  fclose(fp);
}
int main()
{
  WavData test;
  loadWaveFile((char *)"track.wav",&test);
  cout<<"There are "<<test.size/2<<" samples in this WAV file.\n";
  return 0;
}
