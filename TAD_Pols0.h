/* 
 * File:   TAD_Pols0.h
 * Author: marcmiras
 *
 * Created on February 24, 2026, 8:01 AM
 */

#ifndef TAD_POLS0_H
#define	TAD_POLS0_H

#define CERT 1
#define FALS 0

void POLS0_Init (void);
// Post: Inicialitza el TAD. Pre condició de la resta de funcions
    
void POLS0_Motor (void);
// Post: Farà avançar el motor del TAD.

unsigned char POLS0_HiHaPulsacio (void);
// Post: Retorna CERT si s'ha premut el polsador, FALS en cas contrari-

void POLS0_End (void);
// Destructor del TAD

#endif	/* TAD_POLS0_H */

