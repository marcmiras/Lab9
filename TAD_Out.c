/*
 * File:   TAD_Out.c
 * Author: marcmiras
 *
 * Created on February 24, 2026, 8:02 AM
 */

#include <xc.h>
#include "pic18f4321.h"
#include "TAD_Out.h"

// Codificació 7 segments - Càtode comú, actiu a '1'
// Bit: 6=g, 5=f, 4=e, 3=d, 2=c, 1=b, 0=a
//       _
//      |_|   a=top, b=upper-right, c=lower-right
//      |_|   d=bottom, e=lower-left, f=upper-left, g=middle

const unsigned char Taula7S[10] = {
    0x3F,  // 0: a,b,c,d,e,f
    0x06,  // 1: b,c
    0x5B,  // 2: a,b,d,e,g
    0x4F,  // 3: a,b,c,d,g
    0x66,  // 4: b,c,f,g
    0x6D,  // 5: a,c,d,f,g
    0x7D,  // 6: a,c,d,e,f,g
    0x07,  // 7: a,b,c
    0x7F,  // 8: a,b,c,d,e,f,g
    0x6F   // 9: a,b,c,d,f,g
};

void OUT_Init() {
    // PORTD com a sortida per al display 7 segments
    TRISD = 0x00;
    LATD  = 0x00;

    // RA3 (LED1) i RA4 (LED0) com a sortida
    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA3 = 0;
    LATAbits.LATA4 = 0;
}

void OUT_7SegmentsPinta (unsigned char Valor) {
    // Escrivim al LATD la codificació del dígit
    LATD = Taula7S[Valor];
}

void OUT_EncenLed (unsigned char Led) {
    if (Led == LED0) LATAbits.LATA4 = 1;
    else             LATAbits.LATA3 = 1;
}

void OUT_ApagaLed (unsigned char Led) {
    if (Led == LED0) LATAbits.LATA4 = 0;
    else             LATAbits.LATA3 = 0;
}
