/* ファイル名： LED_blink_timer.c
 * 　作成日時： 2015/05/18
 * 　　　機能： LEDをタイマ1を用いて1秒間隔で点滅させる
 */

#include <xc.h>
#include <timer.h>

/*
 * コンフィギュレーション設定
 */
// FOSC（クロック設定）
#pragma config FOSFPR = FRC             /* 8[MHz]の内部発信器を使用 */
#pragma config FCKSMEN = CSW_FSCM_OFF   /* クロック切り替えを無効 */

// FWDT（ウォッチドッグ・タイマ設定）
#pragma config FWPSB = WDTPSB_16        /* プリスケーラBの設定（デフォルト） */
#pragma config FWPSA = WDTPSA_512       /* プリスケーラAの設定 （デフォルト）*/
#pragma config WDT = WDT_OFF            /* ウォッチドッグ・タイマを無効化 */

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV_27         // Brown Out Voltage (2.7V)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FBS
#pragma config BWRP = WR_PROTECT_BOOT_OFF// Boot Segment Program Memory Write Protect (Boot Segment Program Memory may be written)
#pragma config BSS = NO_BOOT_CODE       // Boot Segment Program Flash Memory Code Protection (No Boot Segment)
#pragma config EBS = NO_BOOT_EEPROM     // Boot Segment Data EEPROM Protection (No Boot EEPROM)
#pragma config RBS = NO_BOOT_RAM        // Boot Segment Data RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WR_PROT_SEC_OFF   // Secure Segment Program Write Protect (Disabled)
#pragma config SSS = NO_SEC_CODE        // Secure Segment Program Flash Memory Code Protection (No Secure Segment)
#pragma config ESS = NO_SEC_EEPROM      // Secure Segment Data EEPROM Protection (No Segment Data EEPROM)
#pragma config RSS = NO_SEC_RAM         // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = GSS_OFF            // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

void __attribute__(( interrupt, auto_psv )) _T1Interrupt(void);

int main(void){
    //volatile int i;
    /* IO設定 */
    TRISF = 0x00;   /* すべてのRFポートを出力に */
    PORTF = 0x00;
    LATFbits.LATF5 = 1;

    /* タイマー設定 */

    /* ソースクロックFRC = 8[MHz]*/
    /* クロック = 8[MHz]（ソースクロック） / 4 / 64（プリスケーラー） */
    OpenTimer1(T1_ON & T1_GATE_OFF & T1_PS_1_64 & T1_SYNC_EXT_OFF & T1_SOURCE_INT,31250);
    ConfigIntTimer1(T1_INT_PRIOR_5 & T1_INT_ON);
    DISICNT = 0x0000;

    while(1){
        /* 空ループ */
    }

    return 0;
}

void __attribute__(( interrupt, auto_psv )) _T1Interrupt(void){
    _T1IF = 0; /* タイマーの割り込みオフ */

    LATFbits.LATF5 = ~LATFbits.LATF5;   /* LEDの状態の反転*/
}