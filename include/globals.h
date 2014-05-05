#ifndef GLOBAL
#define GLOBAL

struct global
{
  struct option long_options[4];
  char *input;
  char *output;
  
  struct ALLEGRO_DISPLAY *display;

  char **argv;
  int argc;
  int inputReceived;
  int smoothReceived;
 
};

#endif
