#include <iostream>
#include <stdio.h>
using namespace std;

class FileTest{
  public:
    FileTest():buffer{},ipf{}{}
    void fileprint(){
      ipf = fopen("track.mp3","r");
      if(ipf==NULL)perror("Error opening file\n");
      else
      {
	while(!feof(ipf)){
	  if(fgets(buffer, 100, ipf)==NULL)break;
	  fputs(buffer, stdout);
	}
	fclose(ipf);
      }
    }
  private:
    FILE* ipf;
    char buffer[100];
};
int main()
{
  FileTest test;
  test.fileprint();
  return 0;
}
