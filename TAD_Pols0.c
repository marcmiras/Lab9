/*
 * File:   TAD_Pols0.c
 * Author: marcmiras
 *
 * Created on February 24, 2026, 8:01 AM
 */

#include <xc.h>
#include "pic18f4321.h"
#include "TAD_Pols0.h"

// Pin del polsador: RB0, actiu baix (pull-up intern)
#define PIN_POLS PORTBbits.RB0

// Estats interns del motor
#define REPOS  0
#define PREMUT 1

// Variables locals
static unsigned char Estat;
static unsigned char Pulsacio;

void POLS0_Init() {
    // RB0 com a entrada, habilitem pull-up intern
    TRISBbits.TRISB0 = 1;
    INTCON2bits.RBPU = 0;   // Habilita pull-ups del PORTB

    Estat    = REPOS;
    Pulsacio = FALS;
}

void POLS0_Motor() {
    if (Estat == REPOS && PIN_POLS == 0) {
        // Botó acabat de prémer
        Estat    = PREMUT;
        Pulsacio = CERT;
    } else if (Estat == PREMUT && PIN_POLS == 1) {
        // Botó alliberat, tornem a esperar
        Estat = REPOS;
    }
}

unsigned char POLS0_HiHaPulsacio() {
    // Retorna si hi ha pulsació i neteja el flag
    unsigned char tmp = Pulsacio;
    Pulsacio = FALS;
    return tmp;
}

void POLS0_End() {
    // No cal fer res
}
