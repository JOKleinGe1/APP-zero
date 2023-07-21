#include "mr2d2_2.h"

void ROB2_AlgorithmeNE(void) {
    while (ROB2_Direction()!= OUEST)
	{
		ROB2_Tourne_a_droite();
	}
    while (1)
	{
        ROB2_Avance();       
        ROB2_Avance();
        ROB2_Tourne_a_gauche();
        ROB2_Tourne_a_gauche();
        ROB2_Avance();
        ROB2_Avance();
        ROB2_Avance();
	}
}
