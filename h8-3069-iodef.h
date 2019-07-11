/* h8-3069-iodef.h */
/* H8300Hシリーズ3064の各レジスタ群の定義 */

/*   putbyte, getbyte を使わなくてよい定義の仕方 */
/*     例：P1DDR = a; a = P1DR; のように使える */ 
/*   (putbyte, getbyte を使う場合は h8-3069.h を使用すること) */
/*   頻繁にI/Oアクセスする場合はこちらの方が効率がよい */

/*   設定の詳細はPDFマニュアル(h83069)参照のこと */
/*   作成2006年10月19日 和崎 */

/* タイマ関係で数字から始まるレジスタのdefineはできない */
/* 対策として、最初に"T"を付加しているので注意!!        */
/* ［例］ 16TCR0 => T16TCR0                             */

/* モードコントロールレジスタ */
#define MDCR (*(volatile unsigned char *)0xfee011)
/* システムコントロールレジスタ */
#define SYSCR (*(volatile unsigned char *)0xfee012)

/* 内蔵ROM関連 */
/* フラッシュメモリコントロールレジスタ1 */
#define FLMCR1 (*(volatile unsigned char *)0xfee030)
/* フラッシュメモリコントロールレジスタ2 */
#define FLMCR2 (*(volatile unsigned char *)0xfee031)
/* 消去ブロック指定レジスタ1 */
#define EBR1 (*(volatile unsigned char *)0xfee032)
/* 消去ブロック指定レジスタ2 */
#define EBR2 (*(volatile unsigned char *)0xfee033)
/* RAMコントロールレジスタ */
#define RAMCR (*(volatile unsigned char *)0xfee077)

/* 低消費電力状態関連 */
/* システムコントロールレジスタ */
/* #define SYSCR (*(volatile unsigned char *)0xfee012 */
/* モジュールスタンバイコントロールレジスタH */
#define MSTCRH (*(volatile unsigned char *)0xfee01c)
/* モジュールスタンバイコントロールレジスタL */
#define MSTCRL (*(volatile unsigned char *)0xfee01d)

/* IRQ関連 */
/* IRQセンスコントロールレジスタ */
#define ISCR (*(volatile unsigned char *)0xfee014)
/* IRQイネーブルレジスタ */
#define IER (*(volatile unsigned char *)0xfee015)
/* IRQステータスレジスタ */
#define ISR (*(volatile unsigned char *)0xfee016)
/* インタラプト・プライオリティレジスタA */
#define IPRA (*(volatile unsigned char *)0xfee018)
/* インタラプト・プライオリティレジスタB */
#define IPRB (*(volatile unsigned char *)0xfee019)

/* バスコントロール関連 */
/* バス幅コントロールレジスタ */
#define ABWCR (*(volatile unsigned char *)0xfee020)
/* アクセスステートコントロールレジスタ */
#define ASTCR (*(volatile unsigned char *)0xfee021)
/* ウェイトコントロールレジスタH */
#define WCRH (*(volatile unsigned char *)0xfee022)
/* ウェイトコントロールレジスタL */
#define WCRL (*(volatile unsigned char *)0xfee023)
/* バスリリースコントロールレジスタ */
#define BRCR (*(volatile unsigned char *)0xfee013)
/* チップセレクトコントロールレジスタ */
#define CSCR (*(volatile unsigned char *)0xfee01f)
/* アドレスコントロールレジスタ */
#define ADRCR (*(volatile unsigned char *)0xfee01e)
/* バスコントロールレジスタ */
#define BCR (*(volatile unsigned char *)0xfee024)
/* D-RAM制御は3069にはあるが3064にはない機能 */
/* D-RAMコントロールレジスタ A */
#define DRCRA (*(volatile unsigned char *)0xfee026)
/* D-RAMコントロールレジスタ B */
#define DRCRB (*(volatile unsigned char *)0xfee027)
/* リフレッシュタイマコントロール・ステータスレジスタ */
#define RTMCSR (*(volatile unsigned char *)0xfee028)
/* リフレッシュタイマカウンタ */
#define RTCNT (*(volatile unsigned char *)0xfee029)
/* リフレッシュタイムコンスタントレジスタ */
#define RTCOR (*(volatile unsigned char *)0xfee02A)

/* DMAコントローラは3069にはあるが3064にはない機能 */
/* DMAコントローラ関連 */
/* チャネル 0 */
/* メモリアドレスレジスタ0AR */
#define MAR0AR (*(volatile unsigned char *)0xffff20)
/* メモリアドレスレジスタ0AE */
#define MAR0AE (*(volatile unsigned char *)0xffff21)
/* メモリアドレスレジスタ0AH */
#define MAR0AH (*(volatile unsigned char *)0xffff22)
/* メモリアドレスレジスタ0AL */
#define MAR0AL (*(volatile unsigned char *)0xffff23)
/* 転送カウントレジスタ0AH */
#define ETCR0AH (*(volatile unsigned char *)0xffff24)
/* 転送カウントレジスタ0AL */
#define ETCR0AL (*(volatile unsigned char *)0xffff25)
/* I/Oアドレスレジスタ0A */
#define IOAR0A (*(volatile unsigned char *)0xffff26)
/* データ転送制御レジスタ0A */
#define DTCR0A (*(volatile unsigned char *)0xffff27)
/* メモリアドレスレジスタ0BR */
#define MAR0BR (*(volatile unsigned char *)0xffff28)
/* メモリアドレスレジスタ0BE */
#define MAR0BE (*(volatile unsigned char *)0xffff29)
/* メモリアドレスレジスタ0BH */
#define MAR0BH (*(volatile unsigned char *)0xffff2a)
/* メモリアドレスレジスタ0BL */
#define MAR0BL (*(volatile unsigned char *)0xffff2b)
/* 転送カウントレジスタ0BH */
#define ETCR0BH (*(volatile unsigned char *)0xffff2c)
/* 転送カウントレジスタ0BL */
#define ETCR0BL (*(volatile unsigned char *)0xffff2d)
/* I/Oアドレスレジスタ0B */
#define IOAR0B (*(volatile unsigned char *)0xffff2e)
/* データ転送制御レジスタ0B */
#define DTCR0B (*(volatile unsigned char *)0xffff2f)
/* チャネル 1 */
/* メモリアドレスレジスタ1AR */
#define MAR1AR (*(volatile unsigned char *)0xffff30)
/* メモリアドレスレジスタ1AE */
#define MAR1AE (*(volatile unsigned char *)0xffff31)
/* メモリアドレスレジスタ1AH */
#define MAR1AH (*(volatile unsigned char *)0xffff32)
/* メモリアドレスレジスタ1AL */
#define MAR1AL (*(volatile unsigned char *)0xffff33)
/* 転送カウントレジスタ1AH */
#define ETCR1AH (*(volatile unsigned char *)0xffff34)
/* 転送カウントレジスタ1AL */
#define ETCR1AL (*(volatile unsigned char *)0xffff35)
/* I/Oアドレスレジスタ1A */
#define IOAR1A (*(volatile unsigned char *)0xffff36)
/* データ転送制御レジスタ1A */
#define DTCR1A (*(volatile unsigned char *)0xffff37)
/* メモリアドレスレジスタ1BR */
#define MAR1BR (*(volatile unsigned char *)0xffff38)
/* メモリアドレスレジスタ1BE */
#define MAR1BE (*(volatile unsigned char *)0xffff39)
/* メモリアドレスレジスタ1BH */
#define MAR1BH (*(volatile unsigned char *)0xffff3a)
/* メモリアドレスレジスタ1BL */
#define MAR1BL (*(volatile unsigned char *)0xffff3b)
/* 転送カウントレジスタ1BH */
#define ETCR1BH (*(volatile unsigned char *)0xffff3c)
/* 転送カウントレジスタ1BL */
#define ETCR1BL (*(volatile unsigned char *)0xffff3d)
/* I/Oアドレスレジスタ1B */
#define IOAR1B (*(volatile unsigned char *)0xffff3e)
/* データ転送制御レジスタ1B */
#define DTCR1B (*(volatile unsigned char *)0xffff3f)

/* IOポート関連 */
/* ポート１データディレクションレジスタ */
#define P1DDR (*(volatile unsigned char *)0xfee000)
/* ポート１データレジスタ */
#define P1DR (*(volatile unsigned char *)0xffffd0)
/* ポート２データディレクションレジスタ */
#define P2DDR (*(volatile unsigned char *)0xfee001)
/* ポート２データレジスタ */
#define P2DR (*(volatile unsigned char *)0xffffd1)
/* ポート２入力プルアップMOSコントロールレジスタ */
#define P2PCR (*(volatile unsigned char *)0xfee03c)
/* ポート３データディレクションレジスタ */
#define P3DDR (*(volatile unsigned char *)0xfee002)
/* ポート３データレジスタ */
#define P3DR (*(volatile unsigned char *)0xffffd2)
/* ポート４データディレクションレジスタ */
#define P4DDR (*(volatile unsigned char *)0xfee003)
/* ポート４データレジスタ */
#define P4DR (*(volatile unsigned char *)0xffffd3)
/* ポート４入力プルアップMOSコントロールレジスタ */
#define P4PCR (*(volatile unsigned char *)0xfee03e)
/* ポート５データディレクションレジスタ */
#define P5DDR (*(volatile unsigned char *)0xfee004)
/* ポート５データレジスタ */
#define P5DR (*(volatile unsigned char *)0xffffd4)
/* ポート５入力プルアップMOSコントロールレジスタ */
#define P5PCR (*(volatile unsigned char *)0xfee03f)
/* ポート６データディレクションレジスタ */
#define P6DDR (*(volatile unsigned char *)0xfee005)
/* ポート６データレジスタ */
#define P6DR (*(volatile unsigned char *)0xffffd5)
/* ポート７データレジスタ（入力のみ） */
#define P7DR (*(volatile unsigned char *)0xffffd6)
/* ポート８データディレクションレジスタ */
#define P8DDR (*(volatile unsigned char *)0xfee007)
/* ポート８データレジスタ（５ビット） */
#define P8DR (*(volatile unsigned char *)0xffffd7)
/* ポート９データディレクションレジスタ */
#define P9DDR (*(volatile unsigned char *)0xfee008)
/* ポート９データレジスタ（６ビット） */
#define P9DR (*(volatile unsigned char *)0xffffd8)
/* ポートAデータディレクションレジスタ */
#define PADDR (*(volatile unsigned char *)0xfee009)
/* ポートAデータレジスタ */
#define PADR (*(volatile unsigned char *)0xffffd9)
/* ポートBデータディレクションレジスタ */
#define PBDDR (*(volatile unsigned char *)0xfee00A)
/* ポートBデータレジスタ */
#define PBDR (*(volatile unsigned char *)0xffffdA)

/* １６ビットタイマ */
/* チャネル共通 */
/* タイマスタートレジスタ */
#define TSTR (*(volatile unsigned char *)0xffff60)
/* タイマシンクロレジスタ */
#define TSNC (*(volatile unsigned char *)0xffff61)
/* タイマモードレジスタ */
#define TMDR (*(volatile unsigned char *)0xffff62)
/* タイマアウトプットレベルセットレジスタ */
#define TOLR (*(volatile unsigned char *)0xffff63)
/* タイマインタラプトレベルセットレジスタA */
#define TISRA (*(volatile unsigned char *)0xffff64)
/* タイマインタラプトレベルセットレジスタB */
#define TISRB (*(volatile unsigned char *)0xffff65)
/* タイマインタラプトレベルセットレジスタC */
#define TISRC (*(volatile unsigned char *)0xffff66)
/* チャネル０ */
/* タイマコントロールレジスタ０ */
#define T16TCR0 (*(volatile unsigned char *)0xffff68)
/* タイマI/Oコントロールレジスタ０ */
#define TIOR0 (*(volatile unsigned char *)0xffff69)
/* タイマカウンタ０H */
#define T16TCNT0H (*(volatile unsigned char *)0xffff6A)
/* タイマカウンタ０L */
#define T16TCNT0L (*(volatile unsigned char *)0xffff6B)
/* ジェネラルレジスタA0H */
#define GRA0H (*(volatile unsigned char *)0xffff6c)
/* ジェネラルレジスタA0L */
#define GRA0L (*(volatile unsigned char *)0xffff6d)
/* ジェネラルレジスタB0H */
#define GRB0H (*(volatile unsigned char *)0xffff6e)
/* ジェネラルレジスタB0L */
#define GRB0L (*(volatile unsigned char *)0xffff6f)
/* チャネル１ */
/* タイマコントロールレジスタ１ */
#define T16TCR1 (*(volatile unsigned char *)0xffff70)
/* タイマI/Oコントロールレジスタ１ */
#define TIOR1 (*(volatile unsigned char *)0xffff71)
/* タイマカウンタ１H */
#define T16TCNT1H (*(volatile unsigned char *)0xffff72)
/* タイマカウンタ１L */
#define T16TCNT1L (*(volatile unsigned char *)0xffff73)
/* ジェネラルレジスタA1H */
#define GRA1H (*(volatile unsigned char *)0xffff74)
/* ジェネラルレジスタA1L */
#define GRA1L (*(volatile unsigned char *)0xffff75)
/* ジェネラルレジスタB1H */
#define GRB1H (*(volatile unsigned char *)0xffff76)
/* ジェネラルレジスタB1L */
#define GRB1L (*(volatile unsigned char *)0xffff77)
/* チャネル２ */
/* タイマコントロールレジスタ２ */
#define T16TCR2 (*(volatile unsigned char *)0xffff78)
/* タイマI/Oコントロールレジスタ２ */
#define TIOR2 (*(volatile unsigned char *)0xffff79)
/* タイマカウンタ２H */
#define T16TCNT2H (*(volatile unsigned char *)0xffff7A)
/* タイマカウンタ２L */
#define T16TCNT2L (*(volatile unsigned char *)0xffff7B)
/* ジェネラルレジスタA2H */
#define GRA2H (*(volatile unsigned char *)0xffff7c)
/* ジェネラルレジスタA2L */
#define GRA2L (*(volatile unsigned char *)0xffff7d)
/* ジェネラルレジスタB2H */
#define GRB2H (*(volatile unsigned char *)0xffff7e)
/* ジェネラルレジスタB2L */
#define GRB2L (*(volatile unsigned char *)0xffff7f)

/* ８ビットタイマ関連 */
/* チャネル0 */
/* タイマコントロールレジスタ0 */
#define T8TCR0  (*(volatile unsigned char *)0xffff80)
/* タイマコントロール/ステータスレジスタ0 */
#define T8TCSR0 (*(volatile unsigned char *)0xffff82)
/* タイマコンスタントレジスタA0 */
#define TCORA0 (*(volatile unsigned char *)0xffff84)
/* タイマコンスタントレジスタB0 */
#define TCORB0 (*(volatile unsigned char *)0xffff86)
/* タイマカウンタ0 */
#define T8TCNT0 (*(volatile unsigned char *)0xffff88)
/* チャネル1 */
/* タイマコントロールレジスタ1 */
#define T8TCR1 (*(volatile unsigned char *)0xffff81)
/* タイマコントロール/ステータスレジスタ1 */
#define T8TCSR1 (*(volatile unsigned char *)0xffff83)
/* タイマコンスタントレジスタA1 */
#define TCORA1 (*(volatile unsigned char *)0xffff85)
/* タイマコンスタントレジスタB1 */
#define TCORB1 (*(volatile unsigned char *)0xffff87)
/* タイマカウンタ1 */
#define T8TCNT1 (*(volatile unsigned char *)0xffff89)
/* チャネル2 */
/* タイマコントロールレジスタ2 */
#define T8TCR2 (*(volatile unsigned char *)0xffff90)
/* タイマコントロール/ステータスレジスタ2 */
#define T8TCSR2 (*(volatile unsigned char *)0xffff92)
/* タイマコンスタントレジスタA2 */
#define TCORA2 (*(volatile unsigned char *)0xffff94)
/* タイマコンスタントレジスタB2 */
#define TCORB2 (*(volatile unsigned char *)0xffff96)
/* タイマカウンタ2 */
#define T8TCNT2 (*(volatile unsigned char *)0xffff98)
/* チャネル3 */
/* タイマコントロールレジスタ3 */
#define T8TCR3 (*(volatile unsigned char *)0xffff91)
/* タイマコントロール/ステータスレジスタ3 */
#define T8TCSR3 (*(volatile unsigned char *)0xffff93)
/* タイマコンスタントレジスタA3 */
#define TCORA3 (*(volatile unsigned char *)0xffff95)
/* タイマコンスタントレジスタB3 */
#define TCORB3 (*(volatile unsigned char *)0xffff97)
/* タイマカウンタ3 */
#define T8TCNT3 (*(volatile unsigned char *)0xffff99)

/* プログラマブルタイミングパターンコントローラ関連 */
/* ポートAデータディレクションレジスタ */
/* #define PADDR (*(volatile unsigned char *)0xfee009) */
/* ポートAデータレジスタ */
/* #define PADR (*(volatile unsigned char *)0xffffd9) */
/* ポートBデータディレクションレジスタ */
/* #define PBDDR (*(volatile unsigned char *)0xfee00A) */
/* ポートBデータレジスタ */
/* #define PBDR (*(volatile unsigned char *)0xffffdA) */
/* TPC出力モードレジスタ */
#define TPMR (*(volatile unsigned char *)0xffffa0)
/* TPC出力コントロールレジスタ */
#define TPCR (*(volatile unsigned char *)0xffffa1)
/* ネクストデータイネーブルレジスタB */
#define NDERB (*(volatile unsigned char *)0xffffa2)
/* ネクストデータイネーブルレジスタA */
#define NDERA (*(volatile unsigned char *)0xffffa3)
/* ネクストデータレジスタA */
#define NDRA (*(volatile unsigned char *)0xffffa5)
/* (出力トリガが異なるときグループ0に対応) */
#define NDRA0 (*(volatile unsigned char *)0xffffa5)
/* ネクストデータレジスタA(出力トリガが異なるときグループ1) */
#define NDRA1 (*(volatile unsigned char *)0xffffa7)
/* ネクストデータレジスタB */
#define NDRB (*(volatile unsigned char *)0xffffa4)
/* (出力トリガが異なるときグループ2に対応) */
#define NDRB2 (*(volatile unsigned char *)0xffffa4)
/* ネクストデータレジスタB(出力トリガが異なるときグループ3) */
#define NDRB3 (*(volatile unsigned char *)0xffffa6)

/* ウォッチドッグタイマ関連 */
/* タイマコントロール/ステータスレジスタ */
#define TCSR (*(volatile unsigned char *)0xffff8c)
/* タイマカウンタ */
#define TCNT (*(volatile unsigned char *)0xffff8d)
/* リセットコントロール/ステータスレジスタ */
#define RSTCSR (*(volatile unsigned char *)0xffff8e)

/* シリアルコミュニケーションインターフェース 及び */
/* スマートカードインターフェース関連 */
/* チャネル0 */
/* シリアルモードレジスタ */
#define SMR0 (*(volatile unsigned char *)0xffffb0)
/* ビットレートレジスタ */
#define BRR0 (*(volatile unsigned char *)0xffffb1)
/* シリアルコントロールレジスタ */
#define SCR0 (*(volatile unsigned char *)0xffffb2)
/* トランスミットデータレジスタ */
#define TDR0 (*(volatile unsigned char *)0xffffb3)
/* シリアルステータスレジスタ */
#define SSR0 (*(volatile unsigned char *)0xffffb4)
/* レシーブデータレジスタ */
#define RDR0 (*(volatile unsigned char *)0xffffb5)
/* スマートカードモードレジスタ */
#define SCMR0 (*(volatile unsigned char *)0xffffb6)
/* チャネル1 */
/* シリアルモードレジスタ */
#define SMR1 (*(volatile unsigned char *)0xffffb8)
/* ビットレートレジスタ */
#define BRR1 (*(volatile unsigned char *)0xffffb9)
/* シリアルコントロールレジスタ */
#define SCR1 (*(volatile unsigned char *)0xffffba)
/* トランスミットデータレジスタ */
#define TDR1 (*(volatile unsigned char *)0xffffbb)
/* シリアルステータスレジスタ */
#define SSR1 (*(volatile unsigned char *)0xffffbc)
/* レシーブデータレジスタ */
#define RDR1 (*(volatile unsigned char *)0xffffbd)
/* スマートカードモードレジスタ */
#define SCMR1 (*(volatile unsigned char *)0xffffbe)
/* チャネル2(3064にはなくて3069にある) */
/* シリアルモードレジスタ */
#define SMR2 (*(volatile unsigned char *)0xffffc0)
/* ビットレートレジスタ */
#define BRR2 (*(volatile unsigned char *)0xffffc1)
/* シリアルコントロールレジスタ */
#define SCR2 (*(volatile unsigned char *)0xffffc2)
/* トランスミットデータレジスタ */
#define TDR2 (*(volatile unsigned char *)0xffffc3)
/* シリアルステータスレジスタ */
#define SSR2 (*(volatile unsigned char *)0xffffc4)
/* レシーブデータレジスタ */
#define RDR2 (*(volatile unsigned char *)0xffffc5)
/* スマートカードモードレジスタ */
#define SCMR2 (*(volatile unsigned char *)0xffffc6)

/* A/D変換器関連 */
/* A/DデータレジスタAH */
#define ADDRAH (*(volatile unsigned char *)0xffffe0)
/* A/DデータレジスタAL */
#define ADDRAL (*(volatile unsigned char *)0xffffe1)
/* A/DデータレジスタBH */
#define ADDRBH (*(volatile unsigned char *)0xffffe2)
/* A/DデータレジスタBL */
#define ADDRBL (*(volatile unsigned char *)0xffffe3)
/* A/DデータレジスタCH */
#define ADDRCH (*(volatile unsigned char *)0xffffe4)
/* A/DデータレジスタCL */
#define ADDRCL (*(volatile unsigned char *)0xffffe5)
/* A/DデータレジスタDH */
#define ADDRDH (*(volatile unsigned char *)0xffffe6)
/* A/DデータレジスタDL */
#define ADDRDL (*(volatile unsigned char *)0xffffe7)
/* A/Dコントロール/ステータスレジスタ */
#define ADCSR (*(volatile unsigned char *)0xffffe8)
/* A/Dコントロールレジスタ */
#define ADCR (*(volatile unsigned char *)0xffffe9)

/* D/A変換器関連 */
/* D/Aデータレジスタ0 */
#define DADR0 (*(volatile unsigned char *)0xffff9c)
/* D/Aデータレジスタ1 */
#define DADR1 (*(volatile unsigned char *)0xffff9d)
/* D/Aコントロールレジスタ */
#define DACR (*(volatile unsigned char *)0xffff9e)
/* D/Aスタンバイコントロールレジスタ */
#define DASTCR (*(volatile unsigned char *)0xfee01A)
