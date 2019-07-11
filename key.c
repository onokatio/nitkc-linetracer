#include "h8-3069-iodef.h"

#define KEYBUFSIZE 10  /* キーバッファの大きさ */
#define KEYCHKCOUNT 5  /* キーの連続状態を調べるバッファ上の長さ　 */
                         /* ↑キーバッファの大きさよりも小さくすること */
                         /*   余裕が少ないと正しく読めないことがある */
#define KEYROWNUM  1   /* キー配列の列数(縦に並んでいる個数) */
#define KEYCOLNUM  2   /* キー配列の行数(横に並んでいる個数) */
#define KEYMINNUM  1   /* キー番号の最小値 */
#define KEYMAXNUM  2   /* キー番号の最大値 */
#define KEYNONE   -1   /* 指定したキーがない */
#define KEYOFF     0   /* 指定したキーはずっと離されている状態 */
#define KEYON      1   /* 指定したキーはずっと押されている状態 */
#define KEYTRANS   2   /* 指定したキーは遷移状態 */
#define KEYPOSEDGE 3   /* 指定したキーは今押された状態 */
#define KEYNEGEDGE 4   /* 指定したキーは今離された状態 */

// キースキャンを行って、状態を調べる関数群
// 一定時間（数ms程度）毎に keysense() を呼び出すことが前提
// 任意のキー状態を読み出すには key_check() を呼び出す

/* タイマ割り込み処理のため, バッファ関連は大域変数として確保 */
/* これらの変数は key.c 内のみで使用されている               */
int keybufdp; /* キーバッファ参照ポインタ */
unsigned char keybuf[KEYBUFSIZE][KEYROWNUM]; /* キーバッファ */

/* これらの変数はキー処理だけに使用 */
/* 処理の簡易化のために大域変数として確保 */
unsigned char keyoldval[KEYMAXNUM];
unsigned char keynewval[KEYMAXNUM];

void key_init(void);
void key_sense(void);
int key_check(int keynum);
int key_read(int keynum);

void key_init(void)
     /* キーを読み出すために必要な初期化を行う関数 */
     /* PA4-6 が LCD と関連するが, 対策済み       */
{
  int i,j;

  PADR = 0x0f;       /* PA0-3 は0アクティブ, PA4-6 は1アクティブ */
  PADDR = 0x7f;      /* PA0-3 はキーボードマトリクスの出力用 */
                     /* PA4-6 はLCD制御(E,R/W,RS)の出力用 */
  P6DDR = 0;         /* P60-2 はキーボードマトリクスの入力用 */
                     /* P63-6 はCPUのバス制御として固定(モード6の時) */
  keybufdp = 0;
  /* キーバッファのクリア */
  for (i = 0; i < KEYBUFSIZE; i++){
    for (j = 0; j < KEYROWNUM; j++){
      keybuf[i][j] = 0x07; /* 何もキーが押されていない状態に初期化 */
    }
  }
  /* キー状態値のクリア */
  /* どのキーも押されていない状態に初期化 */
  for (i = 0; i < KEYMAXNUM; i++)
    keynewval[i] = keyoldval[i] = KEYOFF;
}

void key_sense(void)
     /* キースキャンしてキーバッファに入れる関数          */
     /*   数ms 程度に一度, タイマ割り込み等で呼び出すこと */
     /*   大域変数 keybuf はキーデータを格納するバッファ  */
{
  int row;
  unsigned char r;

  /* リングバッファポインタ制御 */
  keybufdp++;
  if (keybufdp >= KEYBUFSIZE) keybufdp = 0;
  /* キースキャン */
  for(row = 0; row < KEYROWNUM; row++){ /* 列ごとにスキャン */
    r = ~(1 << row) & 0x0f;  /* スキャンする列のビットだけ 0 にする */
    r = r | (PADR & 0x70);   /*  LCD の制御に影響しないための対策 */
    PADR = r;                /*  キーデータの読み込み (0:ON, 1:OFF) */
    keybuf[keybufdp][row] = P6DR & 0x07; /* キーバッファに格納 */
  }
}

int key_check(int keynum)
     /* キー番号を引数で与えると, キーの状態を調べて返す関数                 */
     /* キー番号(keynum)は 1-12 で指定(回路図の sw1-sw12 に対応)            */
     /* 基板上の 1-9 のキーは sw1-sw9 に対応している                        */
     /* 基板上の *,0,# のキーは sw10,sw11,sw12 にそれぞれ対応している       */
     /* 戻り値は, KEYOFF, KEYON, KEYTRANS, KEYNONE のいずれか              */
     /* チェック中の割り込みによるバッファ書き換え対策はバッファの大きさで対応 */
{
  int bitpos,row,r,i,dp;
  int kbdp,count_swon,count_swoff;
  unsigned char bitmask,keydata;

  if ((keynum < KEYMINNUM) || (keynum > KEYMAXNUM))
    r = KEYNONE; /* キー番号指定が正しくないときはKEYNONEを返す */
  else {
    keynum--; /* キー番号を 0-11の範囲に変更 */
    bitpos = keynum % KEYCOLNUM; /* キーの配置されているビット位置 */
    bitmask = 1 << bitpos;       /* ビット位置にマスクを設定 */
    row = (keynum - bitpos) / KEYCOLNUM;
    row = KEYROWNUM - 1 - row;   /* キーの配置されている列位置 */
    kbdp = keybufdp;             /* キーバッファポインタを覚えておく */
    count_swoff = count_swon = 0;    /* 連続数カウンタの初期化 */
    /* 指定された長さ分だけキーの状態を調べる */
    for (i = 0; i < KEYCHKCOUNT; i++){
      dp = kbdp - i;
      if (dp < 0) dp = dp + KEYBUFSIZE; /* キーバッファポインタの範囲チェック */
      keydata = keybuf[dp][row];        /* バッファからデータを取り出す */
      /* バッファから状態を調べる (キーは押されると 0 になることに注意) */
      if ((keydata & bitmask) != 0) count_swoff++;  /* 押されていない */
      else count_swon++;                            /* 押されている   */
    }
    /* キー状態の判別 */
    /* 全てキーがON(KEYON)かOFF(KEYOFF)か、それらでないか(KEYTRANS)で判別 */
    r = KEYTRANS;
    if (count_swoff == KEYCHKCOUNT) r = KEYOFF;
    if (count_swon  == KEYCHKCOUNT) r = KEYON;
  }
  return r;
}

int key_read(int keynum)
     /* キー番号を引数で与えると, キーの状態を調べて返す関数                  */
     /* キー番号(keynum)は 1-12 で指定(回路図の sw1-sw12 に対応)             */
     /* 基板上の 1-9 のキーは sw1-sw9 に対応している                         */
     /* 基板上の *,0,# のキーは sw10,sw11,sw12 にそれぞれ対応している        */
     /* 戻り値は, KEYOFF, KEYON, KEYPOSEDGE, KEYNEGEDGE, KEYNONE のいずれか */
     /*      離されている，押されている，今押された，今離された，キーなし      */
{
  int keyval = KEYNONE;
  int i;

  if ((keynum < KEYMINNUM) || (keynum > KEYMAXNUM))
    keyval = KEYNONE; /* キー番号指定が正しくないときはKEYNONEを返す */
  else {

	keyoldval[keynum] = keynewval[keynum];
	keynewval[keynum] = key_check(keynum);

    /* ここでキー状態の判定を行う */
    /* チャタリング処理関数 key_check() は遷移中を示す KEYTRANS を */
    /* 含んだ値を返すので、正しく処理を行うこと */
    /* 過去のキー情報を知らないと、正しい処理ができない */
	if(
			(keyoldval[keynum] == KEYOFF && keynewval[keynum] == KEYON) ||
			(keyoldval[keynum] == KEYTRANS && keynewval[keynum] == KEYON)
	){
		keyval = KEYPOSEDGE;
	}else if(
			(keyoldval[keynum] == KEYON && keynewval[keynum] == KEYOFF) ||
			(keyoldval[keynum] == KEYTRANS && keynewval[keynum] == KEYOFF)
	){
		keyval = KEYNEGEDGE;
	}else{
		if(keynewval[keynum] == KEYON) keyval = KEYON;
		if(keynewval[keynum] == KEYOFF) keyval = KEYOFF;
		if(keynewval[keynum] == KEYTRANS) keyval = KEYNONE;
	}

  }
  return keyval;
}
