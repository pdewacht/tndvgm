#include <conio.h>
#include <i86.h>
#include "psg.h"


unsigned psg_port;
void (* psg_output)(unsigned char);


void psg_reset() {
  psg_output(0x9F);
  psg_output(0xBF);
  psg_output(0xDF);
  psg_output(0xFF);
}


void psg_native_setup() {
  psg_port = 0xC0;
  psg_output = psg_native_output;
  psg_reset();
}


void psg_native_output(unsigned char x) {
  outp(psg_port, x);
}


#define PP_BASE_ADDRESS_DATA  (0x00U)
#define PP_BASE_ADDRESS_STAT  (0x01U)
#define PP_BASE_ADDRESS_CTRL  (0x02U)

#define PP_CTRL_NOT_STROBE    (0x01U)
#define PP_CTRL_INIT          (0x04U)
#define PP_CTRL_NOT_SELECT    (0x08U)

#define PP_STATUS_ERROROR     (0x08U)
#define PP_STATUS_SELECT_IN   (0x10U)
#define PP_STATUS_PAPER_END   (0x20U)
#define PP_STATUS_ACKNOWLEDGE (0x40U)
#define PP_STATUS_NOT_BUSY    (0x80U)


static short get_lpt_port(int i) {
  return *(short __far *)MK_FP(0x40, 6 + 2*i);
}


void psg_lpta_setup(int lpt) {
  psg_port = get_lpt_port(lpt);
  psg_output = psg_lpta_output;
  psg_reset();
}


void psg_lpta_output(unsigned char x) {
  int const lpt_data = psg_port;
  int const lpt_stat = psg_port + PP_BASE_ADDRESS_STAT;
  int const lpt_ctrl = psg_port + PP_BASE_ADDRESS_CTRL;
  int wait;

  outp(lpt_data, x);
  outp(lpt_ctrl, PP_CTRL_NOT_SELECT | PP_CTRL_INIT);

  wait = 24;
  while (wait > 0) {
    if ((inp(lpt_stat) & PP_STATUS_ACKNOWLEDGE) == 0) {
      break;
    }
    wait--;
  }
  while (wait > 0) {
    if ((inp(lpt_stat) & PP_STATUS_ACKNOWLEDGE) != 0) {
      break;
    }
    wait--;
  }

  outp(lpt_ctrl, PP_CTRL_NOT_SELECT | PP_CTRL_NOT_STROBE);
}


void psg_lptb_setup(int lpt) {
  psg_port = get_lpt_port(lpt);
  psg_output = psg_lptb_output;
  psg_reset();
}


void psg_lptb_output(unsigned char x) {
  int const lpt_data = psg_port;
  int const lpt_stat = psg_port + PP_BASE_ADDRESS_STAT;
  int const lpt_ctrl = psg_port + PP_BASE_ADDRESS_CTRL;
  int wait;

  outp(lpt_data, x);
  outp(lpt_ctrl, PP_CTRL_NOT_SELECT | PP_CTRL_INIT);

  wait = 24;
  while (wait > 0) {
    if ((inp(lpt_stat) & PP_STATUS_ACKNOWLEDGE) == 0) {
      break;
    }
    wait--;
  }
  while (wait > 0) {
    if ((inp(lpt_stat) & PP_STATUS_ACKNOWLEDGE) != 0) {
      break;
    }
    wait--;
  }

  outp(lpt_ctrl, PP_CTRL_NOT_SELECT);
}

