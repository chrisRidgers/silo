#include <iostream>
#include <stdio.h>
using namespace std;

class FileTest{
  public:
    FileTest():filesize{},buffer{},ipf{}{}

    void fileprint(){
      ipf = fopen("track.wav","rb");
      if(ipf==NULL)perror("Error opening file\n");
      else
      {
	fseek(ipf,0,SEEK_END);
	filesize=ftell(ipf);
	fseek(ipf,0,SEEK_SET);
	//cout << filesize << "\n";
	buffer=(char*)malloc(filesize);
	fread(buffer,filesize,1,ipf);
	fputs(buffer, stdout);
	fclose(ipf);
      }
    }

  private:
    FILE* ipf;
    char* buffer;
    unsigned long filesize;
};
int main()
{
  FileTest test;
  test.fileprint();
  return 0;
}
