//Displays

#include <xc.h>
#include "Displays.h"

unsigned char tabla_seg(unsigned char valor){
    switch(valor) {
            case 0:                     //0
                return 0x3F;
                break;
            case 1:                     //1
                return 0x06;
                break;
            case 2:                     //2
                return 0x5B;
                break;
            case 3:                     //3
                return 0x4F;
                break;   
            case 4:                     //4
                return 0x66;
                break; 
            case 5:                     //5
                return 0x6D;
                break;
            case 6:                     //6
                return 0x7D;
                break;
            case 7:                     //7
                return 0x07;
                break;
            case 8:                     //8
                return 0x7F;
                break;
            case 9:                     //9
                return 0x6F;
                break;
            case 10:                    //A
                return 0x77;
                break;  
            case 11:                    //B
                return 0x7C;
                break;
            case 12:                    //C
                return 0x39;
                break;
            case 13:                    //D
                return 0x5E;
                break;
            case 14:                    //E
                return 0x79;
                break;
            case 15:                    //F
                return 0x71;
                break;
            
        }
}
