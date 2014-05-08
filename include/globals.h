#ifndef GLOBAL
#define GLOBAL

using namespace std;

struct global
{
  struct option long_options[4];
  int option_index;
  int argc;
  char **argv;
  string input;
  string output;
  
  struct ALLEGRO_DISPLAY *display;

  int inputReceived;
  int outputReceived;
  int smoothReceived;
 
};

#endif
