/* 
 * File:   TAD_Out.h
 * Author: marcmiras
 *
 * Created on February 24, 2026, 8:02 AM
 */

#ifndef TAD_OUT_H
#define	TAD_OUT_H

#define LED0 0
#define LED1 1

void OUT_Init (void);
// Post: Inicialitza el mòdul, i configura entrades i sortides

void OUT_7SegmentsPinta (unsigned char Valor);
// Post: Pinta al 7S (LATD), la conversió a 7S del Valor.

void OUT_EncenLed (unsigned char Led);
// Post: Encen el LED0 (RA4), o LED1 (RA3)
                
void OUT_ApagaLed (unsigned char Led);
// Post: Apaga el LED0 (RA4), o LED1 (RA3)

#endif	/* TAD_OUT_H */

