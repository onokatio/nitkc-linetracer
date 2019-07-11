#include "h8-3069-iodef.h"
#include "h8-3069-int.h"

#include "lcd.h"
#include "ad.h"
#include "timer.h"
#include "key.h"

/* タイマ割り込みの時間間隔[μs] */
#define TIMER0 1000

/* 割り込み処理で各処理を行う頻度を決める定数 */
#define DISPTIME 100
#define KEYTIME 1
#define ADTIME  2
#define PWMTIME 1
#define CONTROLTIME 10

/* LED関係 */
/* LEDがPBに接続されているビット位置 */
#define LMOTOR_IN1   0x01
#define LMOTOR_IN2   0x02
#define RMOTOR_IN1   0x04
#define RMOTOR_IN2   0x08

/* LCD表示関連 */
/* 1段に表示できる文字数 */
#define LCDDISPSIZE 10

/* PWM制御関連 */
/* 制御周期を決める定数 */
#define MAXPWMCOUNT 255

/* A/D変換関連 */
/* A/D変換のチャネル数とバッファサイズ */
#define ADCHNUM   4
#define ADBUFSIZE 8
/* 平均化するときのデータ個数 */
#define ADAVRNUM 4
/* チャネル指定エラー時に返す値 */
#define ADCHNONE -1


/* 割り込み処理に必要な変数は大域変数にとる */
volatile int disp_time, key_time, ad_time, pwm_time, control_time;

/* LED関係 */
unsigned char sensor_r, sensor_l;

/* LCD関係 */
volatile int disp_flag;
volatile char lcd_str_upper[LCDDISPSIZE+1];
volatile char lcd_str_lower[LCDDISPSIZE+1];

/* モータ制御関係 */
volatile int pwm_count;

/* A/D変換関係 */
volatile unsigned char adbuf[ADCHNUM][ADBUFSIZE];
volatile int adbufdp;

int main(void);
void int_imia0(void);
void int_adi(void);
int  ad_read(int ch);
void pwm_proc(void);
void control_proc(void);

int main(void)
{
  /* 初期化 */
  ROMEMU();           /* ROMエミュレーションをON */

  /* ここでmoterポート(PB)の初期化を行う */
  PBDDR = 0xff;

  /* 割り込みで使用する大域変数の初期化 */
  pwm_time = pwm_count = 0;     /* PWM制御関連 */
  disp_time = 0; disp_flag = 1; /* 表示関連 */
  key_time = 0;                 /* キー入力関連 */
  ad_time = 0;                  /* A/D変換関連 */
  control_time = 0;             /* 制御関連 */
  /* ここまで */
  adbufdp = 0;         /* A/D変換データバッファポインタの初期化 */
  lcd_init();          /* LCD表示器の初期化 */
  //key_init();          /* キースキャンの初期化 */
  ad_init();           /* A/Dの初期化 */
  timer_init();        /* タイマの初期化 */
  timer_set(0,TIMER0); /* タイマ0の時間間隔をセット */
  timer_start(0);      /* タイマ0スタート */
  ENINT();             /* 全割り込み受付可 */
  sensor_l = sensor_r = 0; /* 赤・緑LEDの両方を消灯とする */

  /* ここでLCDに表示する文字列を初期化しておく */
  lcd_clear();

  while (1){ /* 普段はこのループを実行している */

    /* ここで disp_flag によってLCDの表示を更新する */
	  if(disp_flag){
		disp_flag = 0;

  		lcd_cursor(0,0);
		lcd_printch((sensor_r/100)%10 + '0');
  		lcd_cursor(1,0);
		lcd_printch((sensor_r/10)%10 + '0');
  		lcd_cursor(2,0);
		lcd_printch('0' + sensor_r%10);

  		lcd_cursor(5,0);
		lcd_printch((sensor_l/100)%10 + '0');
  		lcd_cursor(6,0);
		lcd_printch((sensor_l/10)%10 + '0');
  		lcd_cursor(7,0);
		lcd_printch('0' + sensor_l%10);
	  }

    /* その他の処理はタイマ割り込みによって自動的に実行されるため  */
    /* タイマ 0 の割り込みハンドラ内から各処理関数を呼び出すことが必要 */
  }
}

#pragma interrupt
void int_imia0(void)
     /* タイマ0(GRA) の割り込みハンドラ　　　　　　　　　　　　　　　 */
     /* 関数の名前はリンカスクリプトで固定している                   */
     /* 関数の直前に割り込みハンドラ指定の #pragama interrupt が必要 */
     /* タイマ割り込みによって各処理の呼出しが行われる               */
     /*   呼出しの頻度は KEYTIME,ADTIME,PWMTIME,CONTROLTIME で決まる */
     /* 全ての処理が終わるまで割り込みはマスクされる                 */
     /* 各処理は基本的に割り込み周期内で終わらなければならない       */
{
  /* LCD表示の処理 */
  /* 他の処理を書くときの参考 */
  disp_time++;
  if (disp_time >= DISPTIME){
    disp_flag = 1;
    disp_time = 0;
  }

  /* ここにキー処理に分岐するための処理を書く */
  /* キー処理の中身は全て key.c にある */
  key_time++;
  if (key_time >= KEYTIME){
    key_time = 0;
	//key_sense();
  }

  /* ここにPWM処理に分岐するための処理を書く */
  pwm_time++;
  if (pwm_time >= PWMTIME){
    pwm_time = 0;
	pwm_proc();
  }

  /* ここにA/D変換開始の処理を直接書く */
  /* A/D変換の初期化・スタート・ストップの処理関数は ad.c にある */
  ad_time++;
  if (ad_time >= ADTIME){
    ad_time = 0;
	//ad_start(0,1);
	ad_scan(0,1);
  }

  /* ここに制御処理に分岐するための処理を書く */
  control_time++;
  if (control_time >= CONTROLTIME){
    control_time = 0;
	control_proc();
  }

  timer_intflag_reset(0); /* 割り込みフラグをクリア */
  ENINT();                /* CPUを割り込み許可状態に */
}

#pragma interrupt
void int_adi(void)
     /* A/D変換終了の割り込みハンドラ                               */
     /* 関数の名前はリンカスクリプトで固定している                   */
     /* 関数の直前に割り込みハンドラ指定の #pragma interrupt が必要  */
{
  ad_stop();    /* A/D変換の停止と変換終了フラグのクリア */

  /* ここでバッファポインタの更新を行う */
  adbufdp++;
  if(adbufdp >= ADBUFSIZE){
	  adbufdp = 0;
  }
  /* 　但し、バッファの境界に注意して更新すること */

  /* ここでバッファにA/Dの各チャネルの変換データを入れる */
  adbuf[0][adbufdp] = ADDRAH;
  adbuf[1][adbufdp] = ADDRBH;
  adbuf[2][adbufdp] = ADDRCH;
  adbuf[3][adbufdp] = ADDRDH;
  /* スキャングループ 0 を指定した場合は */
  /*   A/D ch0〜3 (信号線ではAN0〜3)の値が ADDRAH〜ADDRDH に格納される */
  /* スキャングループ 1 を指定した場合は */
  /*   A/D ch4〜7 (信号線ではAN4〜7)の値が ADDRAH〜ADDRDH に格納される */

  ENINT();      /* 割り込みの許可 */
}

int ad_read(int ch)
     /* A/Dチャネル番号を引数で与えると, 指定チャネルの平均化した値を返す関数 */
     /* チャネル番号は，0〜ADCHNUM の範囲 　　　　　　　　　　　             */
     /* 戻り値は, 指定チャネルの平均化した値 (チャネル指定エラー時はADCHNONE) */
{
  int i,ad,bp;
  ad = 0;
  bp = adbufdp;
  int tmp = 0;

  if ((ch > ADCHNUM) || (ch < 0)) ad = ADCHNONE; /* チャネル範囲のチェック */
  else {

    /* ここで指定チャネルのデータをバッファからADAVRNUM個取り出して平均する */
    /* データを取り出すときに、バッファの境界に注意すること */
    /* 平均した値が戻り値となる */
	for(i=0;i<ADAVRNUM;i++){
		tmp = bp - i;
		if(tmp < 0) tmp=ADBUFSIZE+tmp;
		ad+=adbuf[ch][tmp];
	}
	ad /= ADAVRNUM;

  }
  return ad; /* データの平均値を返す */
}

void pwm_proc(void)
     /* PWM制御を行う関数                                        */
     /* この関数はタイマ割り込み0の割り込みハンドラから呼び出される */
{
	

  /* ここにPWM制御の中身を書く */
  if(pwm_count < sensor_r){
	PBDR |= RMOTOR_IN1;
	PBDR &= ~RMOTOR_IN2;
  }else{
	PBDR &= ~RMOTOR_IN1;
	PBDR &= ~RMOTOR_IN2;
  }

  if(pwm_count < sensor_l){
	PBDR |= LMOTOR_IN1;
	PBDR &= ~LMOTOR_IN2;
  }else{
	PBDR &= ~LMOTOR_IN1;
	PBDR &= ~LMOTOR_IN2;
  }

  pwm_count++;
  if (pwm_count >= MAXPWMCOUNT){
    pwm_count = 0;
  }


}

void control_proc(void)
     /* 制御を行う関数                                           */
     /* この関数はタイマ割り込み0の割り込みハンドラから呼び出される */
{

  /* ここに制御処理を書く */
	
	if(key_read(3) == KEYPOSEDGE){
		//greenval++;
	}
	if(key_read(6) == KEYPOSEDGE){
		//greenval--;
	}

	sensor_l = ad_read(1)/2;
	sensor_r = ad_read(2)/2;

}
