/*
 * File:   micslave.c
 * Author: hsnct
 */


#include <xc.h>
#include <pic16f628a.h>

// PIC16F628A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC =  INTOSCIO  // Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, Resistor and Capacitor on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 4000000
#define MIC_ID 12 //4bit 1100

/*unsigned int Baud_Rate_MASTER(long int baudrate){
     unsigned int x;
     x = (_XTAL_FREQ - baudrate*4)/(baudrate*4); //for master
     return x;
 }*/

void UART_Init(long int Baudrate)
{
    BRGH=1;
    unsigned int x;
    x = (_XTAL_FREQ - Baudrate*16)/(Baudrate*16); 
    SPBRG = x;                                    //Writing SPBRG Register 9600 BAUD RATE
    SYNC = 0;                                     //Setting Asynchronous Mode, ie UART
    SPEN = 1;                                     //Enables Serial Port
    TRISB1 = 1;                                   //As Prescribed in Datasheet
    TRISB2 = 1;                                   //As Prescribed in Datasheet
}
  
  
void Write(char data)
{
  TXEN = 1;                                     //Enables Transmission
  while(!TXIF);
  TXREG = data;
  TXEN = 0;                                   
}
char TSR_Empty()
{
  return TRMT;
}

char ReceiveReady()
{
  return RCIF;
}
char Read()
{
  CREN=1;//Enables Continuous Reception
  if(OERR){
      CREN=0;
      CREN=1;
  }//RESET
  while(!RCIF);
  return RCREG;
  CREN=0;
}

uint8_t rx,tx;
int open1,open2,open3,open4;

void main(void) {
    GIE=1;
    PEIE=1;
    RCIE=1;
    TXIE=1;
    TRISB0=0;
    TRISB4=1;
    TRISB5=1;
    TRISB6=1;
    TRISB7=1;
    UART_Init(9600);
    TRISA=0X0F;
    CMCON=0X07;
    PORTA=0;
    RB0=1;

while(1){
    if(ReceiveReady()){
      rx=Read();
      if((rx>>4)==MIC_ID){
        RA0=(rx%2);
        RA1=((rx>>1)%2);
        RA2=((rx>>2)%2);
        RA3=((rx>>3)%2);
        tx=rx;
        if(RB4&open1){
            RA0=0;
            open1=0;
        }
        if(RB5&open2){
            RA1=0;
            open2=0;
        }
        if(RB6&open3){
            RA2=0;
            open3=0;
        }if(RB7&open4){
            RA3=0;
            open1=0;
        }
        if(!RB4){
            if(!RA0){
                RA0=1;
                open1=1;
                RB0=0;
            }
            else if(open1){
                RB0=0;
            }
            else RB0=1;
        }
        else if(!RB5){
                        if(!RA1){
                RA1=1;
                open2=1;
                RB0=0;
            }
            else if(open2){
                RB0=0;
            }
            else RB0=1;
        }
        else if(!RB6){
                        if(!RA2){
                RA2=1;
                open3=1;
                RB0=0;
            }
            else if(open3){
                RB0=0;
            }
            else RB0=1;
        }
        else if(!RB7){
                        if(!RA3){
                RA3=1;
                open4=1;
                RB0=0;
            }
            else if(open4){
                RB0=0;
            }
            else RB0=1;
        }
        
        tx=0x10*MIC_ID+0x08*RA3+0X04*RA2+0X02*RA1+RA0;
        Write(tx);
      }
    }    
}
}
