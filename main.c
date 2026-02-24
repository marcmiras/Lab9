/*
 * File:   main.c
 * Author: marcmiras
 *
 * Created on February 24, 2026, 7:50 AM
 */


#include <xc.h>
#include "TAD_Timer.h"
#include "TAD_Pols0.h"
#include "TAD_Out.h"
#include "TAD_SIO.h"
#include "pic18f4321.h"

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config PBADEN = DIG

// Capçaleres de les funcions locals
static void __interrupt (high_priority) LaRSI (void);
void main (void);

// Variables locals
static char HelloW[] = "Hello world!!";
static char Salt[] = "\r\n";

//Definició de la interrupció d'alta prioritat
static void __interrupt (high_priority) LaRSI (void){
    if ( PIR1bits.RCIF&&PIE1bits.RCIE )
        SIO_InterrupcioRX();
    if ( PIR1bits.TXIF&&PIE1bits.TXIE )
        SIO_InterrupcioTX();
    if ( TMR0IF && TMR0IE )
        RSI_Timer0();
}


void main(){

    unsigned char ElChar;

    // Inicialització dels TADs
    TI_Init();
    POLS0_Init();
    SIO_Init();
    OUT_Init();
    ei();
    SIO_PutString ((unsigned char *)Salt);
    SIO_PutString ((unsigned char *)HelloW);
    SIO_PutString ((unsigned char *)Salt);

    // Bucle principal
    while(1){
        // Crida als motors
        POLS0_Motor();
        // Lògica relacional entre mòduls estàndard
        if (SIO_RXAvail()) {
            OUT_7SegmentsPinta (SIO_RXAvail());
            }
        if (POLS0_HiHaPulsacio()) {
            if (SIO_RXAvail()) {
                ElChar = SIO_GetChar();
                OUT_7SegmentsPinta (SIO_RXAvail());
                if (SIO_TXAvail()!=0)
                    SIO_PutChar(ElChar);
            }
        }
    }

    // No hi arribarem mai, però és de bona educació :-)
    SIO_End();
    POLS0_End();
    TI_End();
}