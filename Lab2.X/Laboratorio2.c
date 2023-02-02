////******************************************************************************
//   UNIVERSIDAD DEL VALLE DE GUATEMALA
//   IE3054 ELECTRÓNICA DIGITAL 2 
//   AUTOR: MICHELLE SERRANO
//   COMPILADOR: XC8 (v1.41), MPLAB X IDE (v6.00)
//   PROYECTO: LABORATORIO 2
//   HARDWARE: PIC16F887
//   CREADO: 26/01/2023
//   ÚLTIMA MODIFCACIÓN: 01/02/2023

#include <xc.h>
#include "ADC.h"
#include "LCD.h"
#include "UART.h"

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#define _XTAL_FREQ 4000000     //frecuencia de 4 MHz
//******************************************************************************
//                              VARIABLES
//******************************************************************************
unsigned char centena;
unsigned char res_cen; 
unsigned char decena;                   
unsigned char unidad;
unsigned char NextADC;                 
unsigned char POT1;
unsigned char POT2;
unsigned char x;
unsigned char contador;
unsigned char operando;
int VOLT1;
int VOLT2;
char frase1 [] = "VOL1: VOL2: CPU";
char ASCII [] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};


//******************************************************************************
//                              FUNCIONES
//******************************************************************************
void Envio_caracter (char a)
{
    while (TXSTAbits.TRMT == 0){
       
    }
    if (PIR1bits.TXIF)
        {
            TXREG = a;
        }  
    return;
}

void Envio_Cadena (char* cadena)
{
    while (*cadena != 0)
    {
      Envio_caracter(*cadena);
      cadena++;
    }
    return;
}

void VoltLCD (int VOLT, unsigned char POT, unsigned char cursor){
        VOLT = POT * 2;
        if (VOLT >= 500){
            VOLT = 500;
        }
        centena = VOLT / 100;  
        res_cen = VOLT % 100;
        decena = res_cen / 10;
        unidad = res_cen % 10;
        
        Lcd_Set_Cursor(2,cursor);
        Lcd_Write_Char(ASCII[centena]);
        Lcd_Write_Char(0x2E);
        Lcd_Write_Char(ASCII[decena]); 
        Lcd_Write_Char(ASCII[unidad]);
        
        Envio_caracter (ASCII[centena]);
        Envio_caracter (0x2E);
        Envio_caracter (ASCII[decena]);
        Envio_caracter (ASCII[unidad]);
 }
 

void main(void) {
//******************************************************************************
//                              CONFIGURACIONES
//******************************************************************************
    Serial_begin(9600);
    operando = Serial_read();
    if (operando == '+'){
            contador++;
        } else if (operando == '-'){
            contador--;
        }
    
    //Oscilador
    OSCCONbits.IRCF2 = 1;               //1MHz
    OSCCONbits.IRCF1 = 0;
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS   = 1;
  
    //INPUTS/OUTPUTS
    ANSELH = 0;  
    ANSEL  = 0x03;                  //ANS0 y 1 analÃ³gicas
    TRISD  = 0;                     //PORTD y E salidas
    TRISE  = 0;                          
    TRISA  = 0x03;                  //PORTA entradas
    
    PORTD  = 0;                     //limpieza de puertos
    PORTE  = 0;
    PORTA  = 0;
    unidad = 0;
    decena = 0;
    NextADC = 0; 
    
    //ADC
    ADCON0bits.ADCS = 0;            // Fosc/2 
    ADCON0bits.CHS = 0;          //se selecciona el canal AN0         
    ADCON0bits.ADON = 1;            //ADC encendido
    __delay_us(200); 
    ADCON0bits.GO = 1;              //se comienza la conversion adc
    ADCON1bits.VCFG1 = 0;           //voltajes referencia default
    ADCON1bits.VCFG0 = 0;           //0V a 5V
    ADCON1bits.ADFM = 0;            //se justifica a la izquierda
    

    //TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 25;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    TXSTAbits.TXEN = 1;
    
    
    //INTERRUPCIONES
    INTCONbits.GIE  = 1;            //Interrupciones globales
    INTCONbits.PEIE = 1; 
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    
    __delay_us(100);   
    Lcd_Init();
    __delay_us(100);   
    Lcd_Set_Cursor(1,1);
    __delay_us(100);   
    Lcd_Write_String(frase1);
    __delay_us(100);   

    
    while (1){
        Envio_Cadena("POT1   POT2");
        Envio_caracter ('\r');
        VoltLCD (VOLT1, POT1, 1);
        Envio_caracter (0x20);
        Envio_caracter (0x20);
        Envio_caracter (0x20);
        
        VoltLCD (VOLT2, POT2, 7);
        Envio_caracter ('\r');
        Envio_caracter ('\r');
       
        
        centena = contador / 100;  
        res_cen = contador % 100;
        decena = res_cen / 10;
        unidad = res_cen % 10;
        Lcd_Set_Cursor(2,13);
        Lcd_Write_Char(ASCII[centena]);
        Lcd_Write_Char(ASCII[decena]); 
        Lcd_Write_Char(ASCII[unidad]);
        
        
        //interrupcion del adc
    if (ADIF == 1) {
        if (NextADC == 0){
            POT2 = CHADC (0);
            NextADC = 1;
        }
        else if (NextADC == 1){
            POT1 = CHADC (1);
            NextADC = 0;
        }
        __delay_us(200);            
        PIR1bits.ADIF = 0;          //interrupcion de adc
        ADCON0bits.GO = 1;          //inicio de la siguiente conversiÃ³n
    }
    }      
}