#include <conio.h>
#include <dos.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "zlib/zlib.h"
#include "psg.h"
#include "vgm.h"


#define STR(x) #x
#define XSTR(x) STR(x)


static volatile int interrupted = 0;

static void __interrupt __far ctrlc_handler() {
  interrupted = 1;
}


static void usage() {
  fprintf(stderr, "Usage: TNDVGM [TANDY|LPTA|LPTB] vgm-file\n");
}


int main(int argc, char *argv[]) {
  gzFile f;

  printf("== TNDVGM " XSTR(VERSION) " ==\n\n");
  if (argc != 3) {
    usage();
    return 1;
  }

  if (strcasecmp(argv[1], "TANDY") == 0) {
    psg_native_setup();
  }
  else if (strcasecmp(argv[1], "LPTA") == 0) {
    psg_lpta_setup(1);
  }
  else if (strcasecmp(argv[1], "LPTB") == 0) {
    psg_lptb_setup(1);
  }
  else {
    usage();
    return 1;
  }

  f = gzopen(argv[2], "rb");
  if (!f) {
    fprintf(stderr, "Can't open file\n");
    return 1;
  }

  while (kbhit()) {
    getch();
  }
  _dos_setvect(0x23, ctrlc_handler);

  if (!music_setup(f)) {
    fprintf(stderr, "Not a Tandy VGM file\n");
    return 1;
  }
  printf("Press any key to stop\n");
  while (!interrupted && music_loop() && !kbhit()) {
  }
  music_shutdown();
  psg_reset();

  while (kbhit()) {
    getch();
  }
  return 0;
}
