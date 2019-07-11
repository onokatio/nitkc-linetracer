/*   H8/3069版 Sフォーマットローダー             (2009/5/1 和崎) */
/*   USB変換が接続されているSCI2側からデータを読み込む           */
/*   Linux では、/dev/ttyUSB0 のように自動的に認識される         */

#include "h8-3069-iodef.h"
#include "loader.h"
#include "sci2.h"
#include "lcd.h"

#define DATAWAIT  1
  /* データ待ち状態 */
#define COMPLETED 0
  /* データロード状態 */
#define ERROR -1
  /* データロードエラー状態 */

int sload();
void call();

int main(void)
{
  //  RAMCR = 0xf8; /* ROMエミュレーションをON */
  init_sci2();  /* SCI2を初期化 */
  lcd_init();   /* LCDを初期化  */

  lcd_cursor(0,0);
  lcd_printstr("S-Loader");
  lcd_cursor(0,1);
  lcd_printstr(" Ready..");
  putch('\n');
  putstr("S-format Loader (by H.Wasaki)",SENDCR);
  putstr("  38400 baud, Non-parity, 1-Stop bit",SENDCR);
  putstr("Ready for data receive :",SENDCR);
  while (1) {
    if (sload() == ERROR){ 
      putstr("S-load error!!",SENDCR);
      lcd_cursor(0,1);
      lcd_printstr(" Load Error !!  ");
    }
  }
  return 1;
}

int sload(void)
{
  /* Sフォーマットダウンロードと実行の本体 */
  /*   戻り値 : 0→正常終了, -1→Sレコード違反 */
  /* S0,S1,S2,S3 と S9,S8,S7 のみ処理をする */
  /*   → 他の S4,5,6 レコードはSレコード違反となる */
  int     i,run,len;
  char    c;
  unsigned long int adr;
  char fname[256];

  run = DATAWAIT;
  while (run == DATAWAIT) {  /* エンドレコードがくるまで読み込みを続ける */
    while ((c = getch(NOT_ECHO)) != 'S');  /* Sがくるまで読み飛ばす */
    putch('S');
    switch (c = getch(DO_ECHO)) {
    case '0' :         /* S0 レコードの処理は読み飛ばすだけ */ 
      len = (int)gethex(2,DO_ECHO) - 3; /* ファイル名の長さを計算 */
      adr = gethex(4,DO_ECHO);          /* アドレス"0000"は読み飛ばす */
      for (i = 0; i < len; i++) {       /* ファイル名を取得 */
	fname[i] = gethex(2,DO_ECHO);
      }
      fname[len] = '\0';
      i = (int)gethex(2,DO_ECHO); putch('\n'); 
                                        /* チェックサムは読み飛ばす */
      putch('\n');
      break;
    case '1' :         /* S1 レコードの処理 */
                       /*   ロードアドレスサイズは2バイト(16ビット) */
      len = (int)gethex(2,DO_ECHO) - 3; adr = gethex(4,DO_ECHO);
      for (i = 0; i < len; i++) {
	*(char *)adr++ = (char)gethex(2,DO_ECHO);
      }
      i = (int)gethex(2,DO_ECHO); putch('\n');
                       /* チェックサムは読み飛ばす */
      break;
    case '2' :         /* S2 レコードの処理 */
                       /*   ロードアドレスサイズは3バイト(24ビット) */
      len = (int)gethex(2,DO_ECHO) - 4; adr = gethex(6,DO_ECHO);
      for (i = 0; i < len; i++) {
	*(char *)adr++ = (char)gethex(2,DO_ECHO);
      }
      i = (int)gethex(2,DO_ECHO); putch('\n');
                       /* チェックサムは読み飛ばす */
      break;
    case '3' :         /* S3 レコードの処理 */
                       /*   ロードアドレスサイズは4バイト(32ビット) */
      len = (int)gethex(2,DO_ECHO) - 5; adr = gethex(8,DO_ECHO);
      for (i = 0; i < len; i++) {
	*(char *)adr++ = (char)gethex(2,DO_ECHO);
      }
      i = (int)gethex(2,DO_ECHO); putch('\n');
                       /* チェックサムは読み飛ばす */
      break;
    case '9' :         /* S9 レコードの処理 */
                       /*   S1に対応するエンドレコード */
      len = (int)gethex(2,DO_ECHO); adr = gethex(4,DO_ECHO);
      i = (int)gethex(2,DO_ECHO); putch('\n');
                       /* チェックサムは読み飛ばす */
      run = COMPLETED;
      break;
    case '8' :         /* S8 レコードの処理 */
                       /*   S2に対応するエンドレコード */
      len = (int)gethex(2,DO_ECHO); adr = gethex(6,DO_ECHO);
      i = (int)gethex(2,DO_ECHO); putch('\n');
                       /* チェックサムは読み飛ばす */
      run = COMPLETED;
      break;
    case '7' :         /* S7 レコードの処理 */
                       /*   S3に対応するエンドレコード */
      len = (int)gethex(2,DO_ECHO); adr = gethex(8,DO_ECHO);
      i = (int)gethex(2,DO_ECHO); putch('\n');
                       /* チェックサムは読み飛ばす */
      putstr("Run!!",SENDCR);
      run = COMPLETED;
      break;
    default :          /* 上記以外の Sx レコードの処理 */
                       /*   不正なSレコードとして処理する */
       run =  ERROR; break;
    }
    if (run == COMPLETED){
      lcd_clear();
      lcd_cursor(0,0);
      lcd_printstr(fname);
      lcd_cursor(0,1);
      lcd_printstr("Run !!");
      call(adr);
    }
  }
  return(run);
}

void call(func)
     int (*func)();
{
  (*func)();
}
