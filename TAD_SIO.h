/* 
 * File:   TAD_SIO.h
 * Author: marcmiras
 *
 * Created on February 24, 2026, 8:04 AM
 */

#ifndef TAD_SIO_H
#define	TAD_SIO_H

#define FALS 0
#define CERT 1

void SIO_Init(void);
// Post: Configuració de la UART en mode assíncron, 9600 bauds, suposant fOsc=10MHz.

void SIO_InterrupcioRX (void) ;
// Pre: Es crida des de la RSI PIR1bits.RCIF==1 i PIE1bits.RCIE==1.
// Post: Processa la interrupció a la RX d'un char.

unsigned char SIO_RXAvail(void);
//Post: Retorna el nombre de caràcters disponibles a la cua de RX.

unsigned char SIO_GetChar(void);
// Pre: SIO_CharAvail() ha retornat CERT.
// Post: Fa lectura destructiva del caràcter rebut.

void SIO_End (void);
// Post: Destructor del TAD. No fa res, realment...

void SIO_InterrupcioTX (void) ;
// Pre: Es crida des de la RSI PIR1bits.TXIF==1 i PIE1bits.TXIE==1.
// Post: Processa la interrupció a la TX d'un char.

unsigned char SIO_TXAvail(void);
//Post: Retorna el nombre de caràcters disponibles a la cua de TX. 0 si no hi ha espai, i fins a MAX_TX si està buida.

void SIO_PutChar (unsigned char Valor);
// Pre: SIO_TXAvail() ha retornat CERT.
// Post: Posa un nou caràcter a enviament.

void SIO_PutString (unsigned char *LaFrase);
// Pre: SIO_TXAvail() ha retornat >= strlen (LaFrase).
// Post: Posa la frase a la cua d'enviament.

#endif	/* TAD_SIO_H */

