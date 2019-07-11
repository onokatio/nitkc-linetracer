/*   H8/3069版 ROM化用のスタートアップルーチン (2009/5/1 和崎) */
/*   RAM版と異なるのは、.data領域を初期化すること              */
/*   .bss領域の 0 クリアはしない                               */
	.h8300h
	.section .text
	.global _start
/* 初期化に必要な各レジスタの定義 */
__syscr  = 0xfee012
__ramcr  = 0xfee077
__abwcr  = 0xfee020
__astcr  = 0xfee021
__wcrh   = 0xfee022
__wcrl   = 0xfee023
__brcr   = 0xfee013
__bcr    = 0xfee024
__cscr   = 0xfee01f
__adrcr  = 0xfee01e
__p1ddr  = 0xfee000
__p2ddr  = 0xfee001
__p5ddr  = 0xfee004
__p8ddr  = 0xfee007
__rtcnt  = 0xfee029
__rtcor  = 0xfee02a
__rtmcsr = 0xfee028
__drcrb  = 0xfee027
__drcra  = 0xfee026
_start:
/* 内蔵RAM上にスタックポインタをセット */
	mov.l   #_stack, sp    /* NMI対策、内蔵RAM：0xffd000 */
/* 必要最低限のレジスタ初期化 */
/*   内蔵RAM・ROMと外部RAMを使用可にするためのもの */
	mov.b	#9, r0l       /* 内蔵RAM使用、スリープ命令でスリープ */
	mov.b	r0l, @__syscr
	mov.b	#0xf0, r0l      /* 内蔵RAMによるエミュレーションなし */
	mov.b	r0l, @__ramcr
	mov.b	#0xff, r0l    /* 外部エリアは全て8ビット幅 */
	mov.b	r0l, @__abwcr
	mov.b	#0xff, r0l    /* 外部エリアは全て3ステートアクセス */
	mov.b	r0l, @__astcr
	mov.b	#0, r0l       /* ウェイト禁止 */
	mov.b	r0l, @__wcrl
	mov.b	#0, r0l       /* 端子ウェイトモード禁止 */
	mov.b	r0l, @__wcrh
	mov.b	#0xfe, r0l    /* A23-A21を開放(PA7-PA4)、バス権開放禁止 */
	mov.b	r0l, @__brcr
	mov.b	#0xc6, r0l    /* アイドルサイクル付加、外部領域2M、P60開放 */
	mov.b	r0l, @__bcr
	mov.b	#0xff, r0l    /* P1XはA7-A0を出力 */
	mov.b	r0l, @__p1ddr
	mov.b	#0xff, r0l    /* P2XはA15-A8を出力 */
	mov.b	r0l, @__p2ddr
	mov.b	#0xf0, r0l    /* A16-A19を開放(P50-P53) */
	mov.b	r0l, @__p5ddr
	mov.b	#0x0f, r0l    /* CS7-CS4は出力禁止, デフォルト */
	mov.b	r0l, @__cscr
	mov.b	#0xe4, r0l    /* CS2(DRAM選択)を出力 */
	mov.b	r0l, @__p8ddr
/* DRAMの制御 */
	mov.b	#0, r0l        /* リフレッシュタイマカウンタをリセット */
	mov.b	r0l, @__rtcnt
	mov.b	#149, r0l      /* リフレッシュサイクル周期をセット */
	mov.b	r0l, @__rtcor
	mov.b	@__rtmcsr, r0l /* フラグクリアのためのダミーリード*/
	mov.b	#0x0f, r0l     /* フラグクリア、リフレッシュクロックΦ/2 */
	mov.b	r0l, @__rtmcsr
	mov.b	#0x98, r0l     /* 10ビットカラム、UCAS,LCASにPB4,PB5使用 */
	mov.b	r0l, @__drcrb
	mov.b	#0x30, r0l     /* エリア2はDRAM、バースト禁止、RFSH(P80)開放 */
	mov.b	r0l, @__drcra
__pon_wait:	
	mov.b	@__rtmcsr, r0l /* 電源ONの後、1リフレッシュサイクル待つ */
	btst.b	#7, r0l        /* フラグをチェック(bit7->z flag) */
	bne	__pon_wait     /* フラグが立っていなければ(bit7 == 0)ループ */
/* 外部RAM上にスタックポインタをセット */
	mov.l   #__ext_stack, sp    /* 外部RAM：0x600000(CS2の終わり+1) */
/* .data領域の初期化 */
/*   er0：.data領域の先頭アドレス (コピー先のポインタ) */
/*   er1：.data領域の終了アドレス+1 (.bss領域の先頭アドレス) */
/*   er2：初期値のあるROM内配置アドレス (コピー元のポインタ) */
/*   r3h：初期値コピー用のテンポラリ */
	mov.l	#__data_start,er0
	mov.l	#__data_end,er1
	mov.l	#__idata_start,er2
__loop_copy:
	cmp.l	er0,er1		/* er1 - er0 を計算する */
	ble	__go_main	/* er1 <= er0 なら初期化終了 */ 
	mov.b	@er2+,r3h	/* ROM領域からデータ読み出し 元ポインタ +1 */
	mov.b	r3h,@er0	/* データをコピー先に */
	inc.l	#1,er0		/* コピー先のポインタ +1 */
	bra	__loop_copy	
/* main()を呼び出す */
/*   実行が終了したらスリープ状態になる */
__go_main:
	jsr     @_main
	sleep
	bra     __go_main
