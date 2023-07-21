//
//  mr2d2_2.h
//
//  Created by Jacques-Olivier Klein on 10/12/2017.
//      Simulateur de robot(s)

#ifndef M2R2D2_2_H
#define M2R2D2_2_H

#include "mr2d2.h"

// QUESTIONS :
BOOL ROB2_Peut_avancer(void);
DIR_T ROB2_Direction (void);
int ROB2_Pos_x(void) ;
int ROB2_Pos_y(void) ;
BOOL ROB2_Arrive(void) ;

//ACTIONS :
void ROB2_Avance(void);
void ROB2_Tourne_a_droite(void);
void ROB2_Tourne_a_gauche(void);
void ROB2_Creve_ballon_et_stop(void);

#endif
