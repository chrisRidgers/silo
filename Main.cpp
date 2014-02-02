#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
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

int sizeX = 256, sizeY = 256;
string pathOut = "output/";
valarray<float> landscape;

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
  landscape.resize(sizeX*sizeY*3);

  cout << sizeX << endl;
  cout << sizeY << endl;
  cout << landscape.size() << endl;
  cout << pathOut << endl;

  setupVerts();
 // for(int i=0;i<landscape.size();i++) cout << landscape[i] << endl;
  saveLandscape();

  return 0;
}

int saveLandscape()
{
  ofstream output;
  output.open(pathOut);
  output << "Hello File World!" << endl;
  for(int i=0; i<landscape.size(); i+=3)
  {
    valarray<float> vert = landscape[slice(i,3,1)];
    output << "v\t" << vert[0] << "\t" << vert[1] << "\t" << vert[2] << endl;
  }
  output.close();
  return 0;
}

int setupVerts()
{
  int countX = 0;
  int countY = 0;

  for(int i=0; i<landscape.size(); i+=3)
  {
    //cout << countX << endl;
    landscape[i]=countX;
    if(countX >= 1023)
    {
      countX = 0;
    }else
    {
      countX++;
    }
    //countX = countX>=1023 ? 0 : countX++;
  }

  for(int i=1; i<landscape.size(); i+=sizeY)
  {
    landscape[slice(i,sizeY,3)] = countY;
    cout << countY << endl;
    countY++;
  }

  return 0;
}
