/*
 * File:   main.c
 * Author: marcmiras
 *
 * Created on February 24, 2026, 7:50 AM
 */



#include <xc.h>
#include "TAD_TIMER.H"
#include "TAD_POLS0.h"
#include "TAD_OUT.h"

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config PBADEN = DIG

// Capçaleres de les funcions locals
static void __interrupt (high_priority) LaRSI (void);
void main (void);

// Variables del mòdul
static unsigned char Ences;
static unsigned char Comptador;

//Definició de la interrupció d'alta prioritat
static void __interrupt (high_priority) LaRSI (void){
    if ( TMR0IF ) TI_RSITimer0();
}


void main(){

    // Inicialització dels TADs
    TI_Init();
    POLS0_Init();
    OUT_Init();
    ei();

    OUT_EncenLed (LED0);

    // Bucle principal
    while(1){
        // Crida als motors
        POLS0_Motor();
        // Lògica relacional entre mòduls estàndard
        if (POLS0_HiHaPulsacio()) {
            OUT_7SegmentsPinta ((++Comptador)%10);
            if (Ences==0) {
                OUT_EncenLed (LED1);
                Ences=1;
            }
            else {
                OUT_ApagaLed (LED1);
                Ences=0;
            }
        }
    }

    // No hi arribarem mai, però és de bona educació :-)
    POLS0_End();
    TI_End();
}
