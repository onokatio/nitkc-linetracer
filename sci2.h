extern void init_sci2(void);
  /* SCI2を初期化する関数 */
  /*   引数：なし */
  /*   戻り値：なし */
extern unsigned long int gethex(int l, int ec);
  /* 任意の桁長のHex文字列をSCIから取得して、数に変換する関数 */
  /*   引数 : 取得・変換するHex文字列の桁数 */
  /*          エコーするかどうか(DO_ECHO, NOT_ECHO) */
  /*   戻り値 : 変換された数 */
  /* 注意：指定された文字数を受信するまでCPUとSCI2を占有する */
extern char getch(int ec);
  /* 1キャラクタをSCI2から受信する関数        */
  /*   引数：エコーするかどうか(DO_ECHO, NOT_ECHO) */
  /* エラー処理：無視(正常受信するまでループ) */
extern void putstr(char *str,int cr);
  /* 文字列をSCI2から送信する関数 */
  /*   引数：str 送信する文字列ポインタ */
  /* 注意：文字列を送出するまでCPUとSCI2を独占する */
extern void putch(char ch);
  /* 1キャラクタをSCI2から送信する関数 */
  /*   引数：ch 送信するデータ         */
