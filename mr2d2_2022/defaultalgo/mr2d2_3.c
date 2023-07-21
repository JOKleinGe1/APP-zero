#include "mr2d2_3.h"

void ROB3_AlgorithmeSE(void) {
    while (ROB3_Direction()!= NORD)
	{
		ROB3_Tourne_a_droite();
	}
    while (1)
	{
        ROB3_Avance();       
        ROB3_Avance();
        ROB3_Tourne_a_gauche();
        ROB3_Tourne_a_gauche();
        ROB3_Avance();
        ROB3_Avance();
        ROB3_Avance();
	}
}
