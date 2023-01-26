////******************************************************************************
//   UNIVERSIDAD DEL VALLE DE GUATEMALA
//   IE3054 ELECTRÓNICA DIGITAL 2 
//   AUTOR: MICHELLE SERRANO
//   COMPILADOR: XC8 (v1.41), MPLAB X IDE (v6.00)
//   PROYECTO: LABORATORIO 1
//   HARDWARE: PIC16F887
//   CREADO: 20/01/2023
//   ÚLTIMA MODIFCACIÓN: 25/01/2023

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF             // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF            // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF            // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF               // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF            // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF             // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF            // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF              // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V         // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF              // Flash Program Memory Self Write Enable bits (Write protection off)


#define _XTAL_FREQ 1000000 //4mHz 
#include <xc.h>
#include <stdio.h>
#include "Displays.h"
#include "ADC.h"

void setup(void);
void main (void);

//******************************************************************************
// Variables
//******************************************************************************
unsigned char b0 = 0;
unsigned char b1 = 0;
unsigned char Decenas;                   
unsigned char Unidades;                   
unsigned char NDisplay;                 
unsigned char Var_disp[2];
unsigned char ADC;

//******************************************************************************
// Interrupciones
//******************************************************************************
    
 void __interrupt() isr(void){           //INTERRUPCIÓN DEL ADC
    if (ADIF == 1) {
        ADC = CHADC (8);                //POT 
        __delay_us(100);               
        PIR1bits.ADIF = 0;              
        ADCON0bits.GO = 1;              //CONVERSIÓN DEL ADC 
    }
    
   if(RBIF == 1)  {                      //CONTADOR 8BITS
      if (RB0 == 0) { 
            b0 = 1;
        }
      if (RB1 == 0) { 
            b1 = 1;
        }
         
      if (RB0 == 1 && b0 == 1){
          b0 = 0;
          PORTD++;
       }
      
      if (RB1 == 1 && b1 == 1){
          b1 = 0;
          PORTD--;
          }
      
     INTCONbits.RBIF = 0;                   //Limpieza de banderas 
    }
    
  //INTERRUPCIÓN DEL TMR0
  if (T0IF == 1){ 
      Unidades = ADC & 0x0F;
      Decenas = ADC & 0xF0;                      //ENVIAR DATOS 7SEG
      Decenas = Decenas >> 4;
        Var_disp[1] = tabla_seg(Decenas);        //LIBRERÍA PARA HEXA
        Var_disp[0] = tabla_seg(Unidades);
        
        switch(NDisplay) {                       //MULTIPLEXADO
            case 0:
                PORTE = 0;
                PORTC = Var_disp[1];
                PORTEbits.RE0 = 1;
                NDisplay = 1;
                break;
            case 1:
                PORTE = 0;
                PORTC = Var_disp[0];
                PORTEbits.RE1 = 1;
                NDisplay = 0;
                break;
        }
    
    T0IF = 0;    //LIMPIEZA DE BANDERAS
    TMR0 = 131;
  }
  
}

//******************************************************************************
// CÃ³digo Principal
//******************************************************************************
void main(void) {
    
    setup(); //CONFIGURACIÓN DE PUERTOS
    
    while (1){   //ALARMA
        if (ADC > PORTD){     //SI EL VALOR DEL ADC ES MAYOR AL CONTADOR
            PORTAbits.RA0 = 1;
           }
        else {
            PORTAbits.RA0 = 0;
        }
    }      
}   
        
//******************************************************************************
// FUNCIÓN PARA CONFIGURACIONES GENERALES
//******************************************************************************
void setup(void){
    
    //INPUTS/OUTPUTS
    ANSELH = 0;
    ANSELHbits.ANS8 = 1;                         
    ANSEL  = 0;                     
    TRISB  = 0b00000111;                    //RB0, RB1 y RB2 INPUTS
    TRISA  = 0;                             //PORT A OUT
    TRISC  = 0;                             //PORT C OUT
    TRISD  = 0;                             //PORT D OUT
    TRISE  = 0;                             //PORT E OUT
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;                     //PULL UPS
    WPUBbits.WPUB1 = 1;
    PORTD  = 0;                             //LIMPIEZA DE PUERTOS
    PORTB  = 0;
    PORTC  = 0x3F;
    PORTE  = 1;
    PORTA  = 0;
    
    NDisplay = 1;
    Unidades = 0;
    Decenas = 0; 
    
    //INTERRUPT ON CHANGE - PUERTOB
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
    //CONFIGURACIÓN ADC
    ADCON0bits.ADCS = 0;           
    ADCON0bits.CHS3 = 1;                //AN0
    ADCON0bits.CHS2 = 0;            
    ADCON0bits.CHS1 = 0;            
    ADCON0bits.CHS0 = 0;            
    ADCON0bits.ADON = 1;                //ADC encendido
    ADCON0bits.GO = 1;                  //INICIO DE CONVERSIÓN ADC
    ADCON1bits.VCFG1 = 0;              
    ADCON1bits.VCFG0 = 0;               
    ADCON1bits.ADFM = 0;                //JUSTIFICADO A LA IZQUIERDA  
    
    //TMR0
    OPTION_REGbits.T0CS = 0;            
    OPTION_REGbits.PSA  = 0;        
    OPTION_REGbits.PS2  = 0;               
    OPTION_REGbits.PS1  = 0;
    OPTION_REGbits.PS0  = 1;
    TMR0 = 131;                         //CADA 2MS
    INTCONbits.T0IF = 0;           
    
    //INTERRUPCIONES
    INTCONbits.GIE  = 1;            //INTERRUPCIONES GLOBALES
    INTCONbits.T0IE = 1;            //TMR0
    INTCONbits.RBIE = 1;            //PUERTO B
    INTCONbits.RBIF = 0;  
    INTCONbits.PEIE = 1; 
    
}


    