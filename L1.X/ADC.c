//ADC
#include <xc.h>
#include "ADC.h"

unsigned char CHADC (unsigned char canal){
    ADCON0bits.CHS = canal;           //cambio de canal
   return ADRESH;                     //valor de la conversión
}