////******************************************************************************
//   UNIVERSIDAD DEL VALLE DE GUATEMALA
//   IE3054 ELECTRÓNICA DIGITAL 2 
//   AUTOR: MICHELLE SERRANO
//   COMPILADOR: XC8 (v1.41), MPLAB X IDE (v6.00)
//   PROYECTO: LABORATORIO 3
//   HARDWARE: PIC16F887
//   CREADO: 02/02/2023

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


#define _XTAL_FREQ 4000000 //4mHz 
#include <xc.h>
#include <stdio.h>
#include "spi.h"
#include "LCD.h"

char buffer[20];
unsigned char dato_1;
unsigned char dato_2;
unsigned char dato_3;

void setup(void);

//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    Lcd_Init();                             // Inicializa la pantalla lcd
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        
        PORTCbits.RC2 = 0;       //Slave Select
       
        spiWrite(1);
        dato_1 = spiRead();      // Lee el dato del ADC 
        __delay_ms(1);
        
        spiWrite(0);
        dato_2 = spiRead();      // Lee el dato del contador
        __delay_ms(1);
       
        Lcd_Set_Cursor(1,1);
        sprintf(buffer, "S1: %u  ", dato_2);
        Lcd_Write_String(buffer);
        __delay_ms(1);

        Lcd_Set_Cursor(1,9);
        sprintf(buffer, "S3: %u  ", dato_1);
        Lcd_Write_String(buffer);
       __delay_ms(1);
       
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
       __delay_ms(250);
        
        PORTCbits.RC1 = 0;       //Slave Select
       
       spiWrite(1);
//     dato_1 = spiRead();      // Lee el dato del ADC
       dato_3 = spiRead();      // Lee el dato del pulsador
        
        Lcd_Set_Cursor (2,1);
        sprintf(buffer, "S2: %u  ", dato_3);
        Lcd_Write_String(buffer);
//        
       __delay_ms(1);
       PORTCbits.RC1 = 1;       //Slave Deselect 
       
       __delay_ms(250);
     
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
        
    ANSEL = 0;
    ANSELH = 0;
    TRISC2 = 0;
    TRISC1 = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}