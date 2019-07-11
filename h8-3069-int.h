/* 割り込み処理を行うために必要なヘッダファイル */
/* 2009/05/01 和崎 */

/* CCRレジスタに割り込み許可/不許可を書き込む                */
/*     ENINT();   <= これ以降は全割り込み許可状態になる      */
/*     ENINT1();  <= プライオリティ1の割り込み許可状態になる */
/*     DISINT();  <= これ以降は全割り込み不許可状態になる    */
/* 注意：この他に割り込みコントローラの設定が必要!!          */

#define ENINT()   asm volatile ("andc.b #0x7f,ccr") 
#define ENINT1()  asm volatile ("andc.b #0xbf,ccr") 
#define DISINT()  asm volatile ("orc.b #0x80,ccr")
#define ROMEMU()  RAMCR=0xf8
