#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <vector>


enum{APP, SIZEX, SIZEY, INPUT, OUTPUT};

using namespace std;

int saveLandscape();

int sizeX = 1024, sizeY = 1024;
string pathOut = "output/";
vector<float> landscape;

int main(int argc, const char* argv[])
{
  if(argv[SIZEX]!=NULL && argv[SIZEY]!=NULL){
    sizeX = atoi(argv[SIZEX]);
    sizeY = atoi(argv[SIZEY]);
  }
  landscape.resize(sizeX*sizeY*3);

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

  cout << sizeX << endl;
  cout << sizeY << endl;
  cout << landscape.size() << endl;
  cout << pathOut << endl;

  saveLandscape();

  return 0;
}

int saveLandscape()
{
  ofstream output;
  output.open(pathOut);
  output << "Hello File World!" << endl;
  output.close();
  return 0;
}
