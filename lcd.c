#include <string.h>
#include "h8-3069-iodef.h"

#define TIMECONST 500
#define TCONST40us 200
#define TCONST1ms 5000
#define LCD_RS 0x40
#define LCD_RW 0x20
#define LCD_E 0x10

void lcd_init(void);
void lcd_cursor(int x, int y);
void lcd_clear(void);
void lcd_printstr(unsigned char *str);
void lcd_printch(unsigned char ch);
void lcd_putch(unsigned char ch, unsigned char rs);
void wait1ms(int ms);

void lcd_init(void)
     /* LCD 表示器を使うための初期化関数             */
     /* 一部, キーセンスとの兼ね合いがあるが対策済み */
     /* LCD 表示器の初期化には時間がかかるので注意   */
{
  wait1ms(15);
  PADR = 0x0f;       /* PA0-3 は0アクティブ, PA4-6 は1アクティブ */
  PADDR = 0x7f;      /* PA4-6 は出力に設定(LCD制御 E,R/W,RS) */
                     /* PA0-3 はキーボードマトリクスの出力用 */
  P4DR = 0;          /* LCD データ */
  P4DDR = 0xff;      /* P40-7 全て出力に設定(LCD DB0-7) */
  lcd_putch(0x3f,0); /* データ転送幅(8ビット) */
  wait1ms(5);
  lcd_putch(0x3f,0);
  wait1ms(100);
  lcd_putch(0x3f,0);
  lcd_putch(0x3f,0); /* データ転送幅8ビット, 2ライン, 5x10ドット */
  lcd_putch(0x04,0); /* ディスプレイON/OFF制御(OFF) */
  lcd_putch(0x01,0); /* ディスプレイ全クリア */
  wait1ms(2);        /* 処理待ち > 1.64ms */
  lcd_putch(0x06,0); /* エントリーモードセット Inc. without Disp.shift */
  lcd_putch(0x0c,0); /* ディスプレイON/OFF制御(ON) */
  lcd_putch(0x80,0); /* DDRAMアドレスセット */
}

void lcd_cursor(int x, int y)
     /* 表示位置を変更する関数 */
     /* 表示器左上が (0,0) となる */
{
  char adr;

  if ((x < 16) && (y < 4)) {
    switch (y) {
    case 1: adr = 0xc0; break; /* 2行目の先頭 */
    case 2: adr = 0x94; break; /* 4桁LCD(SC2004C, L2034)などの3行目の先頭 */
    case 3: adr = 0xd4; break; /* 4桁LCD(SC2004C, L2034)などの4行目の先頭 */
    default: adr = 0x80;       /* 1行目の先頭 */
    }
    lcd_putch(adr+x,0);
  }
}

void lcd_clear(void)
     /* 表示器の全画面をクリアするための関数 */
{
  lcd_putch(0x01,0); /* ディスプレイ全クリア */
  wait1ms(2);        /* 処理待ち > 1.64ms */
}

void lcd_printstr(unsigned char *str)
     /* 表示器に文字列を表示する関数 */
     /* 文字コードは ASCII コードの範囲なら問題ない */
{
  int l, i;

  l = strlen(str);
  for (i = 0; i < l; i++) lcd_printch(str[i]);
}

void lcd_printch(unsigned char ch)
     /* 表示器に1文字だけ表示する関数 */
     /* 文字コードは ASCII コードの範囲なら問題ない */
{
  lcd_putch(ch,1);  /* キャラクタを1文字転送 */
}

void lcd_putch(unsigned char ch, unsigned char rs)
     /* LCD にコマンドやデータを送るための関数   */
     /* ch にコマンドまたはデータを入れる        */
     /* rs が 0 のときコマンド, 1 のときはデータ */
     /* 通常, ユーザから直接呼び出すことはない   */
{
  int i;
  unsigned char st13,st2,key;

  rs = rs << 6;
  st13 = LCD_RS & rs;
  st2 = st13 | LCD_E;
  key = PADR & 0x0f;   /* PA0-3 は未使用           */
  st13 = st13 | key;   /* PA4-6 はLCDの制御線      */
  st2 = st2 | key;     /* PA0-3 のビットは保存する */
  PADR = st13;      /* レジスタ選択を行う */
  PADR = st2;       /* E信号を 1 にする */
  P4DR = ch;        /* データまたはコマンドを送る */
  PADR = st13;      /* E信号を 0 にする */
  for (i = 0; i < TCONST40us; i++); /* 40us 待ち(LCDの処理時間) */
}

void wait1ms(int ms)
     /* ms = 1 のとき, 約1msだけムダなループで待つ関数            */
     /* LCDの動作を保証するために必要な時間を確保するのが目的     */
     /* この間, CPUを独占するので注意し, 他の用途に使用しないこと */
{
  int i,j;

  for (i = 0; i < ms; i++){
    for (j = 0; j < TCONST1ms; j++);
  }
}
