/*
 * File:   TAD_SIO.c
 * Author: marcmiras
 *
 * Created on February 24, 2026, 8:03 AM
 */

#include <xc.h>
#include "TAD_SIO.h"
#include "pic18f4321.h"

// Definicions privades del TAD
#define CONFIGURACIO_TXSTA 0x24     // Dades de 8 bits, TX Enable, Asíncron, BRGH=1
#define CONFIGURACIO_RCSTA 0x90     // Enable canal serie, 8 bits, asíncron, RX enable
#define DIVISOR_BAUDRATE 64

// El TAD dimensiona una cua de recepció de valors de 32 chars.
#define MAX_RX 32

// La màscara per al final de la cua. Ull, si la mida no és potència de 2 no funcionarà!
#define MASK_RX 0x1F

// Variables locals, cua de RX
static unsigned char CuaRX [MAX_RX] ;
static unsigned char IniciRX, FiRX, QuantsRX;

// Constructor del TAD
void SIO_Init(void){
    // Post: Configuració de la UART en mode assíncron, 9600 bauds, suposant fOsc=10MHz.
    IniciRX=FiRX=QuantsRX=0;
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
    BAUDCONbits.BRG16=0;
    TXSTA=CONFIGURACIO_TXSTA;
    RCSTA=CONFIGURACIO_RCSTA;
    SPBRG=DIVISOR_BAUDRATE;
    PIE1bits.RCIE=1;
    }

void SIO_InterrupcioRX (void) {
    // Esborrem el IF de recepció.
    PIR1bits.RCIF=0;
    // Llegim el caràcter rebut i el posem a la cua.
    CuaRX[IniciRX++]=RCREG;
    IniciRX&=MASK_RX;
    QuantsRX++;
}

unsigned char SIO_RXAvail() {
    // A COMPLETAR


}

unsigned char SIO_GetChar() {
    unsigned char ElValor;
    di();

    // A COMPLETAR
    // Treure el valor de la cua i posar-lo a ElValor

    ei();
    return ElValor
}

// Destructor del TAD
void SIO_End (){
//  No fa res, realment...
}
