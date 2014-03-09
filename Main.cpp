#include <ctime>
#include <fftw3.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sndfile.h>
#include <sstream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <valarray>
#include <vector>


enum{APP, SIZEX, SIZEY, INPUT, OUTPUT};

using namespace std;

int saveLandscape();
int setupVerts();
int soundInput();

int sizeX = 256, sizeY = 256;
char *infilename;
string pathOut = "output/";
valarray<float> landscape;

SNDFILE *input;
SF_INFO sfinfo;

int main(int argc, const char* argv[])
{
  if(argv[SIZEX]!=NULL && argv[SIZEY]!=NULL){
    sizeX = atoi(argv[SIZEX]);
    sizeY = atoi(argv[SIZEY]);
  }
  landscape.resize((sizeX-1)*(sizeY-1)*3);
  
  infilename = (char*)argv[INPUT];
  soundInput();

  //Defines file output path based on time
  time_t t = time(0);
  struct tm * now = localtime(&t);
  stringstream ss;
  ss 
    << now->tm_mday 
    << now->tm_mon+1 
    << now->tm_year+1900 
    << now->tm_hour 
    << now->tm_min 
    << now->tm_sec
    << ".obj";
  pathOut += ss.str();
  landscape.resize(sizeX*sizeY*3);


  cout << "Size X:\t\t\t" << sizeX << endl;
  cout << "Size Y:\t\t\t" << sizeY << endl;
  cout << "Landscape Size:\t\t\t" << landscape.size() << endl;
  cout << "Output:\t\t\t" << pathOut << endl;
  cout << "Input Sound File:\t\t\t" << infilename << endl;
  
  setupVerts();
  cout << "setupVerts complete" << endl;
  // for(int i=0;i<landscape.size();i++) cout << landscape[i] << endl;
  saveLandscape();
  cout << "saveLandscape complete" << endl;
  cout << "Input Sample Rate:\t\t\t" << sfinfo.samplerate << endl;
  cout << "Channels:\t\t\t" << sfinfo.channels<< endl;
  cout << "Format:\t\t\t" << sfinfo.format<< endl;
  cout << "Sections:\t\t\t" << sfinfo.sections<< endl;
  cout << "Seekable:\t\t\t" << sfinfo.seekable<< endl;

  return 0;
}

int saveLandscape()
{
	ofstream output;
	//cout << "Landscape Test1" << endl;
	output.open(pathOut);
	if(output.is_open())
	{
		//cout << "Landscape Test2" << endl;
		//output << "Hello File World!\n";
	}else
	{
		return 1;
	}

	for(int i=0; i<landscape.size()-1; i+=3)
	{
		valarray<float> vert = landscape[slice(i,3,1)];
		//cout << "v\t" << vert[0] << "\t" << vert[1] << "\t" << vert[2] << "\n";
		output << "v\t" << vert[0] << "\t" << vert[1] << "\t" << vert[2] << "\n";
	}

	output << "\n";
	/*
	   for(int i=1; i<sizeX*sizeY-sizeX; i++)
	   {
	   cout << "f\t" << i << " " << i+sizeX << " " << i+sizeX+1 << " " << i+1 << " \n";
	   output << "f\t" << i << " " << i+sizeX << " " << i+sizeX+1 << " " << i+1 << " \n";
	   }
	   */
	for(int i=1; i<landscape.size()/3-sizeX-1; i++){ if(i%sizeX!=0)output << "f\t" << i << " " << i+sizeX << " " << i+sizeX+1 << " " << i+1 << " \n"; }
	output.close();
	return 0;
}

int setupVerts()
{
	float countX = 0.0f;
	float countY = 0.0f;

	for(int i=0; i<landscape.size(); i+=3)
	{
		landscape[i]=countX/16;
		countX = countX>=((sizeX)-1) ? 0 : countX+=1;
		//cout << countX << endl;
	}

	//countX = 0;
	//
	//test

	for(int i=1; i<landscape.size(); i+=sizeY*3)
	{
		landscape[slice(i,sizeY,3)] = countY/16;
		countY+=1;
	}

	return 0;
}

int soundInput()
{
	if(!(input = sf_open(infilename, SFM_READ, &sfinfo)))
	{
		printf("Not able to open input file %s.\n", infilename);
		puts(sf_strerror(NULL));
		return 1;
	}else
	{
		return 0;
	}
}
