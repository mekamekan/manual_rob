/*
 * File:   main.c
 * Author: hirot
 *
 * Created on August 18, 2023, 12:40 PM
 */

// PIC16F1827 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#define _XTAL_FREQ 32000000
#include <xc.h>
#include<conio.h>
#include<stdio.h>



unsigned char g_ReadData;


void __interrupt() ISR();
void DataWrite(unsigned char data);
//void DataRead(unsigned char *buf);
void putch(unsigned char data);

void main(void) {
    OSCCON = 0b11110000;
    ANSELA = 0x00;
    ANSELB = 0x00;
    TRISA = 0x00;
    TRISB = 0b00000010;
    LATA = 0x00;
    LATB = 0x00;
    OPTION_REGbits.nWPUEN = 0;
    WPUA = 0x00;
    WPUB = 0x00;
    
    TXSTA = 0b00100100;
    RCSTA = 0b10010000;
    BAUDCON = 0b00001000;
    SPBRG = 68;
    
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    unsigned char *Buffer;
    
    while(1){
     
        DataWrite('a');
        DataWrite(g_ReadData);
        //DataRead(g_ReadData);
        
        __delay_ms(10);
    }
    return;
}

void __interrupt() ISR(){
    if(PIR1bits.RCIF){
        //PIR1bits.RCIF = 0;
        if(RCSTAbits.FERR || RCSTAbits.OERR){
            RCSTA = 0x00;
            RCSTA = 0x90;
        }
        else
            g_ReadData = RCREG;
        
    }
    else;
}

/*void DataRead(unsigned char *buf){
    while(*buf != 0x63){
        if(PIR1bits.RCIF){
            if(RCSTAbits.FERR || RCSTAbits.OERR){
                RCSTA = 0x00;
                RCSTA = 0x90;
            }
            else
                *buf = RCREG;
                buf++;
        }
        else
            ;
    }
}*/

void DataWrite(unsigned char data){
    while(!PIR1bits.TXIF);
    PIR1bits.TXIF = 0;
    TXREG = data;
    
    return;
}

/*void DataRead(unsigned char *str){
    int i = 0;
    
    while(str[i] != '\n'){
        if(PIR1bits.RCIF){
            PIR1bits.RCIF = 0;
            if(RCSTAbits.FERR || RCSTAbits.OERR){
              RCSTA = 0x00;
              RCSTA = 0x90;
              return;
            }
            else{
                str[i] = RCREG;
                i++;
            }
        }
        else
            ;
    }
    
    return;
    
}*/

void putch(unsigned char data){
    DataWrite(data);
}