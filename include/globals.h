#ifndef GLOBAL
#define GLOBAL

struct global
{
  struct option long_options[4];
  int option_index;
  int argc;
  char **argv;
  char *input;
  char *output;
  
  struct ALLEGRO_DISPLAY *display;

  int inputReceived;
  int smoothReceived;
 
};

#endif
