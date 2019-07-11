#include "h8-3069-iodef.h"

/* 内部クロックφのときの1usあたりのGRA値 */
#define TCONST1us 25
#define CLKDIV1MAX 2621
#define CLKDIV2MAX 5242
#define CLKDIV4MAX 10485
#define CLKDIV8MAX 20971

int timer_set(int ch, unsigned int time_us);
void timer_start(int ch);
void timer_stop(int ch);
void timer_init(void);
void timer_intflag_reset(int ch);

int timer_set(int ch, unsigned int time_us)
     /* 指定チャネルのタイマを指定時間間隔で割り込み設定する関数 */
     /* ch:0-4, time_us:1-20971 の範囲でないと正常動作しない     */
     /* φ=25MHz のとき, φ/1 : 2621.4 us   φ/2 : 5242.8 us     */
     /* 最大の指定周期は φ/4 : 10485.6 us  φ/8 : 20971.2 us    */
     /* ch = x (x = 0-4) のとき int_imiax() に制御が移る         */
     /* CPU自体の割り込み許可を出さなければ動かない              */
     /* 戻り値: 指定が適正なときは1、適正でないときは -1         */
     /* 注意: 指定したタイマは停止するので別途スタートさせること */
{
  volatile unsigned char *tcr_addr,*tior_addr;
  volatile unsigned char *grah_addr,*gral_addr,*tcnth_addr,*tcntl_addr;
  unsigned int gra;
  unsigned char clk_div,chmask;
  int rf;

  switch (ch){
  case 1:
    tcr_addr   = &T16TCR1;   tior_addr  = &TIOR1;
    tcnth_addr = &T16TCNT1H; tcntl_addr = &T16TCNT1L;
    grah_addr  = &GRA1H;     gral_addr  = &GRA1L;
    break;
  case 2:
    tcr_addr   = &T16TCR2;   tior_addr  = &TIOR2;
    tcnth_addr = &T16TCNT2H; tcntl_addr = &T16TCNT2L;
    grah_addr  = &GRA2H;     gral_addr  = &GRA2L;
    break;
  case 0:
  default:
    ch = 0;
    tcr_addr   = &T16TCR0;   tior_addr  = &TIOR0;
    tcnth_addr = &T16TCNT0H; tcntl_addr = &T16TCNT0L;
    grah_addr  = &GRA0H;     gral_addr  = &GRA0L;
    break;
  }
  clk_div = 0;
  if (time_us > 0) rf = 1;
  else rf = -1;
  if (time_us > CLKDIV1MAX) clk_div = 1; /* Φ = φ/2 */
  if (time_us > CLKDIV2MAX) clk_div = 2; /* Φ = φ/4 */
  if (time_us > CLKDIV4MAX) clk_div = 3; /* Φ = φ/8 */
  if (time_us > CLKDIV8MAX) rf = -1;
  /* 設定するタイマの動作を止める */
  chmask = 1;
  chmask = ~(chmask << ch);
  TSTR = TSTR & chmask;
  /* TCNT(カウンタ)を 0 にリセット */
  *tcnth_addr = 0;
  *tcntl_addr = 0;
  /* GRAコンペアマッチクリア, 内部クロックΦ(自動選択) */
  *tcr_addr = 0xa0 | (clk_div & 0x02);
  /* GRAはアウトプットコンペアレジスタ */
  *tior_addr = (*tior_addr & 0xf0) | 0x08;
  /* TISRAのGRAによるコンペアマッチフラグをリセットし、割り込みを許可 */
  chmask = chmask & 0x0f; /* 指定チャネルのビット位置だけ値を0にする */
  /* 上位4ビット：割り込み許可、下位4ビット：ステータスフラグ */
  TISRA = TISRA & chmask;
  TISRA = TISRA | (1 << (ch + 4));
  /* 指定された時間間隔になるようにGRAをセット */
  gra = (unsigned int)(time_us * TCONST1us / (1 << clk_div));
  *gral_addr = gra & 0xff;
  *grah_addr = (gra >> 8) & 0xff;
  return rf;
}

void timer_start(int ch)
     /* 指定チャネルのタイマ動作を開始する関数                    */
     /* 指定チャネルの範囲 ch: 0-4                                */
     /* CPUの割り込み許可を行わないと割り込みは発生しないので注意 */
{
  unsigned char chmask;

  if ((ch < 0) || (ch > 4)) ch = 0;
  chmask = 1;
  chmask = chmask << ch;
  TSTR = TSTR | chmask;
}

void timer_stop(int ch)
     /* 指定チャネルのタイマ動作を停止する関数                    */
     /* 指定チャネルの範囲 ch: 0-4                                */
{
  unsigned char chmask;

  if ((ch < 0) || (ch > 4)) ch = 0;
  chmask = 1;
  chmask = ~(chmask << ch);
  TSTR = TSTR & chmask;
}

void timer_init(void)
     /* 16ビットタイマの初期化を行う関数                 */
     /* タイマを使用するときには, 最初に必ず呼び出すこと */
{
  unsigned char tmp;

  TSNC = 0xe8;  /* チャネル0-2 全て独立動作 */
  TMDR = 0x98;  /* チャネル0-2 全て通常動作モード */
  tmp = TISRA;  /* フラグクリアのための空読み */
  TISRA = 0x99; /* GRAのコンペアマッチフラグクリアと割り込み禁止 */
  tmp = TISRB;  /* フラグクリアのための空読み */
  TISRB = 0x99; /* GRBのコンペアマッチフラグクリアと割り込み禁止 */
  tmp = TISRC;  /* フラグクリアのための空読み */
  TISRC = 0x99; /* 16CNTのオーバフローフラグクリアと割り込み禁止 */
  TOLR = 0xc0;  /* 通常動作では意味なし、初期値に設定 */
}

void timer_intflag_reset(int ch)
     /* 指定チャネルのタイマの割り込みフラグのリセットを行う関数   */
     /* 指定チャネルの範囲 ch: 0-4                                 */
     /* 割り込みハンドラ内の割り込みを許可する前に必ず呼び出すこと */
{
  unsigned char chmask;

  chmask = 1;
  chmask = ~(chmask << ch);
  TISRA = TISRA & chmask; /* 指定したチャネルだけフラグをリセット */
}
