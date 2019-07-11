#include "h8-3069-iodef.h"

#define BITWAIT 100
#define NOT_ECHO 0
#define DO_ECHO 1
#define NOTSENDCR 0
#define SENDCR 1

/* 文字列の長さをはかるときの上限 */
#define MAXSTRLEN 10000

void init_sci2();
char numtohex();
unsigned char hextonum();
unsigned long int gethex();
char getch();
void putstr();
void putch();

void init_sci2(void)
{
  /* SCI2を初期化する関数 */
  /*   引数：なし */
  /*   戻り値：なし */

  int i;

  SCMR2 = 0xf2;    /* SMIFモードを禁止 */
  SCR2  = 0x00;    /* 送受信を不可能に設定 */
  SMR2  = 0x00;    /* 通信パラメータの設定 */
  BRR2  = 19;      /*   8bit,non-parity,1-stop,38400bps[φ=25MHz] */
  for(i = 0; i < BITWAIT; i++); /* 最低でも1bit経過分は待つ */
  SCR2  = 0x30;    /* 送受信可能状態に(割り込み不可) */
}

char numtohex(x)
     char x;
{
  /* 数をHex文字 1文字 に変換する関数 */
  /*   引数 : 0 - 15 までの整数 */
  /*   戻り値 : 対応する '0' - 'F' のキャラクタコード */

  char ch;

  if (x <= 9) {         /* ASCIIコードでは '0' - '9' が連続している */
    ch = x + '0';
  } else {              /* ASCIIコードでは 'A' - 'F' が連続している */
    ch = x - 10 + 'A';
  }
  return ch;
}

unsigned char hextonum(ch)
     char ch;
{
  /* Hex文字 1文字 を数に変換する関数 */
  /*   引数 : 対応する '0' - 'F' のキャラクタコード */
  /*   戻り値 : 0 - 15 までの整数 */

  unsigned char num;

  if (ch <= '9') {
    num = ch - '0';
  } else {
    num = ch - 'A' + 10;
  }
  return num;
}

unsigned long int gethex(l, ec)
     int l, ec;
{
  /* 任意の桁長のHex文字列をSCIから取得して、数に変換する関数 */
  /*   引数 : 取得・変換するHex文字列の桁数 */
  /*          エコーするかどうか(DO_ECHO, NOT_ECHO) */
  /*   戻り値 : 変換された数 */
  /* 注意：指定された文字数を受信するまでCPUとSCI2を占有する */

  int i;
  unsigned long int s;
  char c;

  s = 0;
  for (i = 0; i < l; i++){
    c = getch(ec);
    s <<= 4;
    s += hextonum(c);
  }
  return s;
}

char getch(ec)
     int ec;
{
  /* 1キャラクタをSCI2から受信する関数        */
  /*   引数：エコーするかどうか(DO_ECHO, NOT_ECHO) */
  /* エラー処理：無視(正常受信するまでループ) */

  char ch;
  unsigned char flag;

  flag = SSR2;          /* SCI2のフラグをチェック */
  while((flag & 0x38) != 0){     /* エラーがなくなるまで待つ */
    SSR2 = flag & 0xc7;   /* エラー発生時はフラグをクリア */
    flag = SSR2;          /* SCI2のフラグを再チェック */
  }
  while((flag & 0x40) == 0){     /* 受信データが溜るまで待つ */
    flag = SSR2;          /* SCI2のフラグを再チェック */
  }
  ch = RDR2;            /* 受信データの読み出し */
  flag = SSR2;          /* 直前の状態をチェック */
  SSR2 = flag & 0xbf;   /* 受信フラグのリセット */
  if (ec == DO_ECHO) {           /* 受信文字をエコーするか? */
    putch(ch);
  }
  return ch;
}

void putstr(str,cr)
     char *str;
     int cr;
{
  /* 文字列をSCI2から送信する関数 */
  /*   引数：str 送信する文字列ポインタ */
  /* 注意：文字列を送出するまでCPUとSCI2を独占する */

  int len, po;

  len = strlen(str);
  for (po = 0; po < len; po++) {
    putch(str[po]);
  }
  if (cr == SENDCR) {  /* CRコードを送出する? */
    putch('\n');
  }
}

void putch(ch)
     char ch;
{
  /* 1キャラクタをSCI2から送信する関数 */
  /*   引数：ch 送信するデータ         */

  unsigned char flag;

  flag = SSR2;          /* SCI2のフラグをチェック */
  while((flag & 0x80) == 0){     /* 送信可能状態(TDRE=1)になるまで待つ */
    flag = SSR2;          /* SCI2のフラグを再チェック */
  }
  TDR2 = ch;              /* 送信データの書き出し */
  SSR2 = flag & 0x7f;     /* 送信フラグのリセット */
}
