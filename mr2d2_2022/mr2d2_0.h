//
//  mr2d2_0.h
//
//  Created by Jacques-Olivier Klein on 10/12/2017.
//      Simulateur de robot(s)

#ifndef M2R2D2_0_H
#define M2R2D2_0_H

#include "mr2d2.h"

// QUESTIONS :
BOOL ROB0_Peut_avancer(void);
DIR_T ROB0_Direction (void);
int ROB0_Pos_x(void) ;
int ROB0_Pos_y(void) ;
BOOL ROB0_Arrive(void) ;

//ACTIONS :
void ROB0_Avance(void);
void ROB0_Tourne_a_droite(void);
void ROB0_Tourne_a_gauche(void);
void ROB0_Creve_ballon_et_stop(void);

#endif
