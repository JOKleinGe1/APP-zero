//
//  mr2d2_3.h
//
//  Created by Jacques-Olivier Klein on 10/12/2017.
//      Simulateur de robot(s)

#ifndef M2R2D2_3_H
#define M2R2D2_3_H

#include "mr2d2.h"

// QUESTIONS :
BOOL ROB3_Peut_avancer(void);
DIR_T ROB3_Direction (void);
int ROB3_Pos_x(void) ;
int ROB3_Pos_y(void) ;
BOOL ROB3_Arrive(void) ;

//ACTIONS :
void ROB3_Avance(void);
void ROB3_Tourne_a_droite(void);
void ROB3_Tourne_a_gauche(void);
void ROB3_Creve_ballon_et_stop(void);

#endif
