#include "mr2d2_0.h"

void ROB0_AlgorithmeSO(void) {
    while (ROB0_Direction()!= EST)
	{
		ROB0_Tourne_a_droite();
	}
    while (!ROB0_Arrive())
	{
        ROB0_Avance();       
        ROB0_Avance();
        ROB0_Tourne_a_gauche();
        ROB0_Tourne_a_gauche();
        ROB0_Avance();
        ROB0_Avance();
        ROB0_Avance();
	}
}
