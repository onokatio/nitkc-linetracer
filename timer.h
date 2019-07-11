extern int timer_set(int ch, unsigned int time_us);
     /* 指定チャネルのタイマを指定時間間隔で割り込み設定     */
     /* ch:0-4, time_us:1-20971 の範囲でないと正常動作しない */
     /* ch = x (x = 0-4) のとき int_imiax() に制御が移る     */
     /* CPU自体の割り込み許可を出さなければ動かない          */
     /* 戻り値: 指定が適正なときは1、適正でないときは -1     */
extern void timer_start(int ch);
     /* 指定チャネルのタイマ動作開始 */
     /* ch: 0-4 */
extern void timer_stop(int ch);
     /* 指定チャネルのタイマ動作停止 */
     /* ch: 0-4 */
extern void timer_init(void);
     /* 16ビットタイマの初期化 */
extern void timer_intflag_reset(int ch);
     /* 指定チャネルのタイマの割り込みフラグのリセットを行う関数   */
     /* 指定チャネルの範囲 ch: 0-4                                 */
     /* 割り込みハンドラ内の割り込みを許可する前に必ず呼び出すこと */

