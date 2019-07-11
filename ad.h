#define ADSTATUS() (ADCSR & 0x80)
#define ADREAD() ADDRAH
#define ADREAD0() ADDRAH
#define ADREAD1() ADDRBH
#define ADREAD2() ADDRCH
#define ADREAD3() ADDRDH
#define ADREAD4() ADDRAH
#define ADREAD5() ADDRBH
#define ADREAD6() ADDRCH
#define ADREAD7() ADDRDH

extern void ad_init();
extern void ad_start(unsigned char ch, unsigned char int_sw);
extern void ad_scan(unsigned char ch_grp, unsigned char int_sw);
extern void ad_stop(void);
