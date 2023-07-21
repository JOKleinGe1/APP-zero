#include "mr2d2_1.h"


void ROB1_AlgorithmeNO(void){
    while (ROB1_Direction()!= SUD)
	{
		ROB1_Tourne_a_droite();
	}
    while (1)
	{
        ROB1_Avance();       
        ROB1_Avance();
        ROB1_Tourne_a_gauche();
        ROB1_Tourne_a_gauche();
        ROB1_Avance();
        ROB1_Avance();
        ROB1_Avance();
	}
}