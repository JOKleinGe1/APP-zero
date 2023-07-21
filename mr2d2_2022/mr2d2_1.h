//
//  mr2d2_1.h
//
//  Created by Jacques-Olivier Klein on 10/12/2017.
//      Simulateur de robot(s)

#ifndef M2R2D2_1_H
#define M2R2D2_1_H

#include "mr2d2.h"

// QUESTIONS :
BOOL ROB1_Peut_avancer(void);
DIR_T ROB1_Direction (void);
int ROB1_Pos_x(void) ;
int ROB1_Pos_y(void) ;
BOOL ROB1_Arrive(void) ;

//ACTIONS :
void ROB1_Avance(void);
void ROB1_Tourne_a_droite(void);
void ROB1_Tourne_a_gauche(void);
void ROB1_Creve_ballon_et_stop(void);

#endif
