/* リフレッシュコントローラのカウンタを使って */
/* 擬似的に乱数を発生させるための関数群   */

extern void random_init(void);      /* 乱数初期化, プログラムの最初に必要 */
extern unsigned char random(void);  /* 疑似乱数を返す関数 */
