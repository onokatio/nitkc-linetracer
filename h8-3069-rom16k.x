/*
  H8-3069F ROM版リンカスクリプト (スタック領域とデータ領域が外部RAM)
  2009/05/1 和崎
*/
OUTPUT_FORMAT("coff-h8300")
OUTPUT_ARCH(h8300h)
ENTRY("_start")
/* スタックポインタの設定 */
/*   内蔵RAMをスタック領域に使うときのスタックポインタ初期化アドレス */
PROVIDE(_stack = 0xffff20);
/*   外部RAMをスタック領域に使うときのスタックポインタ初期化アドレス */
PROVIDE(__ext_stack = 0x600000);
/*
  Memory Map
  0x000000 - 0x0000ff ( 0x00100 bytes) : Vector Area        (256Byte)
  0x000000 - 0x07ffff ( 0x80000 bytes) : Internal Flash-ROM (512kB)
  0x400000 - 0x5fffff (0x200000 bytes) : External RAM Area  (2MB)
  0xffbf20 - 0xffff1f ( 0x04000 bytes) : Internal RAM Area  (16KB)
    ROM Emulation : 
    0xffbf20 - 0xffdfff (0x020e0 bytes) : Stack & Data Area     (8KB)
    0xffe000 - 0xffefff (0x01000 bytes) <=> 0x000000 - 0x000fff (4kB)
    0xfff000 - 0xffff1f (0x00f20 bytes) : Stack & Data Area     (4kB)
*/
MEMORY
{
    /* ROM化したときのベクタ領域 */
    /*   リセット後は必ず Flash-ROM 領域になっている */
    vectors	: o = 0x000000, l = 0x100
    /* プログラム領域として使える Flash-ROM 領域 */
    /*   先頭の256バイト分はベクタ領域として使用している */
    rom		: o = 0x000100, l = 0x7ff00
    /* データ領域の設定 */
    /* 外付けRAM(2MB)をデータ領域に使う場合はこちらを有効にする */
    /*   最後の 64kB はスタック領域 */
    /* ram	   	: o = 0x400000, l = 0x1f0000 */
    /* 内蔵RAM(16K)をデータ領域に使う場合はこちらを有効にする   */
    /*   ROM化する場合はRAMエミュレーションは考えなくてよいはず */
    /*   実際のプログラム + 変数領域は 14kB */
    /*   スタック領域は残り(最後)の2kB */
    ram		: o = 0xffbf20, l = 0x03800
    /*   RAM化かつ内蔵RAMをスタック領域とする場合は、*/
    /*   RAMエミュレーションを考える必要がある */
    /*   実際のプログラム + 変数領域は 8kB */
    /*   RAMエミュレーション領域は 4kB */
    /*   スタック領域は残り(最後)の4kB */
    /* ram		: o = 0xffbf20, l = 0x020e0 */
}
SECTIONS                
{                   
.vectors : {
	/* 例外処理ベクタの処理 */
	/* ROMエミュレーションのため、RAM上に全て置かれる */
	/* 割り込みハンドラが無いときは、全てスタートになる */
	/* リセットベクタ */
	LONG(ABSOLUTE(_start))
	/* リザーブ */
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	/* NMIベクタ */
	LONG(DEFINED(_int_nmi)?ABSOLUTE(_int_nmi):ABSOLUTE(_start))
	/* トラップベクタ */
	LONG(DEFINED(_int_trap0)?ABSOLUTE(_int_trap0):ABSOLUTE(_start))
	LONG(DEFINED(_int_trap1)?ABSOLUTE(_int_trap1):ABSOLUTE(_start))
	LONG(DEFINED(_int_trap2)?ABSOLUTE(_int_trap2):ABSOLUTE(_start))
	LONG(DEFINED(_int_trap3)?ABSOLUTE(_int_trap3):ABSOLUTE(_start))
	/* IRQベクタ */
	LONG(DEFINED(_int_irq0)?ABSOLUTE(_int_irq0):ABSOLUTE(_start))
	LONG(DEFINED(_int_irq1)?ABSOLUTE(_int_irq1):ABSOLUTE(_start))
	LONG(DEFINED(_int_irq2)?ABSOLUTE(_int_irq2):ABSOLUTE(_start))
	LONG(DEFINED(_int_irq3)?ABSOLUTE(_int_irq3):ABSOLUTE(_start))
	LONG(DEFINED(_int_irq4)?ABSOLUTE(_int_irq4):ABSOLUTE(_start))
	LONG(DEFINED(_int_irq5)?ABSOLUTE(_int_irq5):ABSOLUTE(_start))
	/* リザーブ */
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	/* インターバルタイマ */
	LONG(DEFINED(_int_wovi)?ABSOLUTE(_int_wovi):ABSOLUTE(_start))
	/* リザーブ */
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	/* A/Dエンド */
	LONG(DEFINED(_int_adi)?ABSOLUTE(_int_adi):ABSOLUTE(_start))
	/* 16ビットタイマch0 */
	LONG(DEFINED(_int_imia0)?ABSOLUTE(_int_imia0):ABSOLUTE(_start))
	LONG(DEFINED(_int_imib0)?ABSOLUTE(_int_imib0):ABSOLUTE(_start))
	LONG(DEFINED(_int_ovi0)?ABSOLUTE(_int_ovi0):ABSOLUTE(_start))
	/* リザーブ */
	LONG(ABSOLUTE(_start))
	/* 16ビットタイマch1 */
	LONG(DEFINED(_int_imia1)?ABSOLUTE(_int_imia1):ABSOLUTE(_start))
	LONG(DEFINED(_int_imib1)?ABSOLUTE(_int_imib1):ABSOLUTE(_start))
	LONG(DEFINED(_int_ovi1)?ABSOLUTE(_int_ovi1):ABSOLUTE(_start))
	/* リザーブ */
	LONG(ABSOLUTE(_start))
	/* 16ビットタイマch2 */
	LONG(DEFINED(_int_imia2)?ABSOLUTE(_int_imia2):ABSOLUTE(_start))
	LONG(DEFINED(_int_imib2)?ABSOLUTE(_int_imib2):ABSOLUTE(_start))
	LONG(DEFINED(_int_ovi2)?ABSOLUTE(_int_ovi2):ABSOLUTE(_start))
	/* リザーブ */
	LONG(ABSOLUTE(_start))
	/* 8ビットタイマch0/1 */
	LONG(DEFINED(_int_cmia0)?ABSOLUTE(_int_cmia0):ABSOLUTE(_start))
	LONG(DEFINED(_int_cmib0)?ABSOLUTE(_int_cmib0):ABSOLUTE(_start))
	LONG(DEFINED(_int_cmia1_cmib1)?ABSOLUTE(_int_cmia1_cmib1):ABSOLUTE(_start))
	LONG(DEFINED(_int_tovi0_tovi1)?ABSOLUTE(_int_tovi0_tovi1):ABSOLUTE(_start))
	/* 8ビットタイマch2/3 */
	LONG(DEFINED(_int_cmia2)?ABSOLUTE(_int_cmia2):ABSOLUTE(_start))
	LONG(DEFINED(_int_cmib2)?ABSOLUTE(_int_cmib2):ABSOLUTE(_start))
	LONG(DEFINED(_int_cmia3_cmib3)?ABSOLUTE(_int_cmia3_cmib3):ABSOLUTE(_start))
	LONG(DEFINED(_int_tovi2_tovi3)?ABSOLUTE(_int_tovi2_tovi3):ABSOLUTE(_start))
	/* リザーブ */
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	/* SCI ch0 */
	LONG(DEFINED(_int_eri0)?ABSOLUTE(_int_eri0):ABSOLUTE(_start))
	LONG(DEFINED(_int_rxi0)?ABSOLUTE(_int_rxi0):ABSOLUTE(_start))
	LONG(DEFINED(_int_txi0)?ABSOLUTE(_int_txi0):ABSOLUTE(_start))
	LONG(DEFINED(_int_tei0)?ABSOLUTE(_int_tei0):ABSOLUTE(_start))
	/* SCI ch1 */
	LONG(DEFINED(_int_eri1)?ABSOLUTE(_int_eri1):ABSOLUTE(_start))
	LONG(DEFINED(_int_rxi1)?ABSOLUTE(_int_rxi1):ABSOLUTE(_start))
	LONG(DEFINED(_int_txi1)?ABSOLUTE(_int_txi1):ABSOLUTE(_start))
	LONG(DEFINED(_int_tei1)?ABSOLUTE(_int_tei1):ABSOLUTE(_start))
	/* リザーブ */
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	LONG(ABSOLUTE(_start))
	}  > vectors
/* コード領域、文字列、定数の領域 → 内蔵ROM */
.text : {
      __text_start = . ;
      *(.text)                
      *(.strings)
      *(.rodata)              
      __text_end = . ; 
}  > rom
/* C++のコンストラクタとデストラクタ → 内蔵ROM */
.tors : {
    __ctors = . ;
    *(.ctors)
    __ctors_end = . ;
    __dtors = . ;
    *(.dtors)
    __dtors_end = . ;
    __idata_start = . ;
    }  > rom
/* 初期値をもつ変数 → 内蔵ROM →(転送)→ RAM領域 */
/*   RAM領域への転送は、romcrt.s で行う */
.data : AT(__idata_start) {
    __data_start = .;
    *(.data)
    __data_end = . ;
    }  > ram
/* 初期値をもたない変数 → RAM領域 */
.bss : {
    __bss_start = .;
    *(.bss)
    *(COMMON)
    __bss_end = .;
    }  >ram
}
