extern unsigned psg_port;

extern void (* psg_output)(unsigned char);
void psg_reset();

void psg_native_setup();
void psg_native_output(unsigned char);

void psg_lpta_setup(int lpt);
void psg_lpta_output(unsigned char);

void psg_lptb_setup(int lpt);
void psg_lptb_output(unsigned char);
