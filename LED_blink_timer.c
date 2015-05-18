/* ファイル名： LED_blink_timer.c
 * 　作成日時： 2015/05/18
 * 　　　機能： LEDをタイマ1を用いて1秒間隔で点滅させる
 */

#include <xc.h>
#include <timer.h>

/*
 * コンフィギュレーション設定
 * MPLAB XのWindow -> PIC Memory Views -> Configuration Bitsから自動生成
 */
/* OSC（クロック設定）*/
#pragma config FOSFPR = FRC             /* 8[MHz]の内部発信器を使用 */
#pragma config FCKSMEN = CSW_FSCM_OFF   /* クロック切り替えを無効 */

/* FWDT（ウォッチドッグ・タイマ設定）*/
#pragma config FWPSB = WDTPSB_16        /* プリスケーラBの設定（デフォルト） */
#pragma config FWPSA = WDTPSA_512       /* プリスケーラAの設定 （デフォルト）*/
#pragma config WDT = WDT_OFF            /* ウォッチドッグ・タイマを無効 */

/* FBORPOR（ブラウン・アウト・リセット、パワー・オン・リセット設定）*/
#pragma config FPWRT = PWRT_64          /* 64[ms]のパルス幅でリセット*/
#pragma config BODENV = BORV_27         /* 2.7[V]以下になったらリセット*/
#pragma config BOREN = PBOR_ON          /* 自動リセットを有効*/
#pragma config MCLRE = MCLR_EN          /* MCLRピンを有効*/

/* FBS（ブート設定？デフォルト値使用）*/
#pragma config BWRP = WR_PROTECT_BOOT_OFF// Boot Segment Program Memory Write Protect (Boot Segment Program Memory may be written)
#pragma config BSS = NO_BOOT_CODE       // Boot Segment Program Flash Memory Code Protection (No Boot Segment)
#pragma config EBS = NO_BOOT_EEPROM     // Boot Segment Data EEPROM Protection (No Boot EEPROM)
#pragma config RBS = NO_BOOT_RAM        // Boot Segment Data RAM Protection (No Boot RAM)

/* FSS（セキュリティ設定？デフォルト値使用）*/
#pragma config SWRP = WR_PROT_SEC_OFF   // Secure Segment Program Write Protect (Disabled)
#pragma config SSS = NO_SEC_CODE        // Secure Segment Program Flash Memory Code Protection (No Secure Segment)
#pragma config ESS = NO_SEC_EEPROM      // Secure Segment Data EEPROM Protection (No Segment Data EEPROM)
#pragma config RSS = NO_SEC_RAM         // Secure Segment Data RAM Protection (No Secure RAM)

/* FGS（コードプロテクト設定）*/
#pragma config GWRP = GWRP_OFF          /* コード書き込み保護無効 */
#pragma config GCP = GSS_OFF            /* コード保護無効 */

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

/*
 * 関数プロトタイプ
 */
void __attribute__(( interrupt, auto_psv )) _T1Interrupt(void);

/*
 * メイン関数
 */
int main(void){
    /* IO設定 */
    TRISF = 0x00;   /* すべてのRFポートを出力に */
    PORTF = 0x00;   /* PORTFを初期化 */
    LATFbits.LATF5 = 1; /* LED消灯 */

    /* タイマ1設定
     *
     * タイマ1設定関数
     * void OpenTimer1(unsigned int config, unsigned int period)
     * T1_ON           : タイマ1有効
     * T1_GATE_OFF     : ゲート制御無効
     * T1_PS_1_64      : プリスケーラは1/64
     * T1_CYNC_EXT_OFF : 同期カウンタ無効
     * T1_SOURCE_INT   : 内部クロック使用
     *
     * PICはソースクロック4回分でアセンブラ命令を1つ処理する。
     * ただし、クロックを4分割しただけでは16bitに収まらないので、
     * プリスケーラで更に分割する。
     * タイマ1のクロック[Hz] = ソースクロック[Hz] / 4 / プリスケーラ
     *                    　= 8 * 10 ^ 6 / 4 / 64
     *                      = 31250
     * 今回は1秒ごとに点滅させるため、この値を使用
     *
     * タイマ1割り込み設定関数
     * void ConfigIntTimer1( unsigned int config)
     * T1_INT_PRIOR_t   : 割り込み優先順位は5
     * T1_INT_ON        : タイマ1割り込み許可
     */
    OpenTimer1(T1_ON & T1_GATE_OFF & T1_PS_1_64 & T1_SYNC_EXT_OFF & T1_SOURCE_INT,31250);
    ConfigIntTimer1(T1_INT_PRIOR_5 & T1_INT_ON);

    while(1){
        /* 空ループ */
    }

    return 0;
}

/*
 * タイマ1割り込み関数
 */
void __attribute__(( interrupt, auto_psv )) _T1Interrupt(void){
    _T1IF = 0; /* タイマ1割り込みを禁止 */

    LATFbits.LATF5 = ~LATFbits.LATF5;   /* LEDの状態の反転*/
}