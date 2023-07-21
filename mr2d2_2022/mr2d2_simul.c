//
//  m2r2.c
//
//  Created by Jacques-Olivier Klein on 10/12/2017.
//  https://youtu.be/97xET8dikII

////////  FICHIERS ENTETE INCLUS ////////

#include <stdio.h>
#include <stddef.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "PTHREAD_BARRIER.H"
#include "mr2d2.h"
#include "mr2d2_0.h"
#include "mr2d2_1.h"
#include "mr2d2_2.h"
#include "mr2d2_3.h"

#define POINT_NORD  {0,1}
#define POINT_SUD {0,-1}
#define POINT_EST  {1,0}
#define POINT_OUEST {-1,0}
#define POINT_NULL  {0,0}

////////  DEFINITION des TYPES ////////

typedef struct {int x, y;} POINT_T;
typedef enum { VIDE, OBSTACLE, ROBOT, TRAJECTOIRE }  CASE_T ;
typedef enum  { ROUTE, ARRIVE, ARRET} STATUS_T;
typedef struct  {int number; POINT_T pos; POINT_T initiale; POINT_T finale;
    STATUS_T statut; DIR_T dir; unsigned long step ; int rang;
    int watchdog_counter;} ROBOT_T;
typedef void *(*ROBOT_ALGORITHME_T)(void *threadid) ;

////////  DECLARATIIONS des PRTOTYPES des FONCTIONS ////////

void MAP_Print (void);
POINT_T POINT_Add (POINT_T pt1, POINT_T pt2);
BOOL POINT_Egaux(POINT_T pt1, POINT_T pt2);
POINT_T ROBOT_Point_devant(int numrob) ;
CASE_T ROBOT_Case_devant(int numrob);
void ROBOT_gare (int numrob);
void ROBOT_Init(int num, int xi, int yi,  int xf, int yf, DIR_T dir);
void MAP_Place_obstacle(int x, int y);
void MAP_Refresh(void);
void  Scheduler(int numrob) ;
char CASE_Print (int i, int j,int mode);
void *Robot_AlgorithmeNE_local(void *threadid) ;
void *Robot_AlgorithmeSO_local(void *threadid) ;
void *Robot_AlgorithmeNO_local(void *threadid) ;
void *Robot_AlgorithmeSE_local(void *threadid) ;
void ROB0_AlgorithmeSO(void);
void ROB1_AlgorithmeNO(void);
void ROB2_AlgorithmeNE(void);
void ROB3_AlgorithmeSE(void);
void ROBOT_Creve_ballon_et_stop(int numrob);
void Watchdog_incremete(int robnum); 
void Watchdog_reset(int robnum); 

//////// DECLARATION  des VARIABLES GLOBALES ////////

ROBOT_T tab_robot [4];
unsigned int NumRobotArrives = 0;
unsigned int NumRobotArrives_synchrone = 0;
unsigned int NumRobotArretes = 0;
pthread_barrier_t barrier1;
pthread_t threads[NUMBER_ROBOTS];
pthread_mutex_t Mutex_print; // pour l'affichage
pthread_mutex_t Mutex_bouge; // déplacement des robots
POINT_T tab_dir[]={POINT_NORD,POINT_EST,POINT_SUD, POINT_OUEST};
DIR_T tab_droite[]={EST, SUD,OUEST,NORD};
DIR_T tab_gauche[]={OUEST, NORD,EST,SUD};
ROBOT_ALGORITHME_T Tab_Robot_Algorithme[]= {
    Robot_AlgorithmeSO_local,
    Robot_AlgorithmeNO_local,
    Robot_AlgorithmeNE_local,
    Robot_AlgorithmeSE_local};
CASE_T map [DIM_PISTE_X+2][DIM_PISTE_Y+2];
char DirSymb[4]={'N','E','S','W'};
char * StatusSym[3]={"En-route","AU BUT !","ARRET: %"};
////////  DEFINITIONS de FONCTIONS ////////

int main(int argc, const char * argv[]) {
    int rc;
    int t;
    rc = pthread_barrier_init(&barrier1, NULL, NUMBER_ROBOTS);
    if(pthread_mutex_init(&Mutex_print, 0) < 0)
    {
        return -1;
    }
    if(pthread_mutex_init(&Mutex_bouge, 0) < 0)
    {
        return -1;
    }
    // 0:SW, 1:NW, 2,NE, 3:SE
    ROBOT_Init(0,1,1, DIM_PISTE_X, DIM_PISTE_Y, OUEST );
    ROBOT_Init(1,1,DIM_PISTE_Y, DIM_PISTE_X, 1, NORD );
    ROBOT_Init(2,DIM_PISTE_X,DIM_PISTE_Y, 1, 1, EST );
    ROBOT_Init(3,DIM_PISTE_X,1, 1, DIM_PISTE_Y, SUD );
    
    MAP_Refresh();
    MAP_Print();
    getchar();
   for (t = 1; t < NUMBER_ROBOTS; t++) {
    	rc = pthread_create(threads + t, NULL, Tab_Robot_Algorithme[t], (void *) &tab_robot[t]); if (rc) {
        	printf("ERROR; return code from pthread_create() is %d\n", rc);
    	}
    }
    ROB0_AlgorithmeSO();
    ROBOT_Creve_ballon_et_stop( 0);
    getchar();  
    return 0;
}

char CASE_Print (int i, int j,int mode){
    CASE_T thecase = map[i][j];
    const char tab_map  []={'^','>','V','<'};
    char oc;
    int k;
    int r = -1;
    switch(thecase){
        case VIDE : oc = ' ';
        break;
        case TRAJECTOIRE : oc = '.';
        break;
        case OBSTACLE : oc = 'o';
        break;
        case ROBOT:
            for(k = 0; k<NUMBER_ROBOTS; k++){
                if ((tab_robot[k].pos.x==i) && (tab_robot[k].pos.y==j)) r=k;
            }
            if (mode) oc =tab_robot[r].number+'a';
            else if (tab_robot[r].statut==ROUTE){
            oc = tab_map[tab_robot[r].dir];
            }else if (tab_robot[r].statut==ARRIVE){
                oc = '0'+tab_robot[r].rang;
            } else if(tab_robot[r].statut==ARRET) {
                oc = '%';
            }
            else oc ='?';
            break;
        default : oc = 'W';
    }
    return oc;
}

void MAP_Print (void){
    static int mode = 0;
    int j,i;
    static int Index_print_map = NUMBER_ROBOTS;
    if (++Index_print_map>=NUMBER_ROBOTS){
        Index_print_map=0;
        NumRobotArrives_synchrone = NumRobotArrives ;
        pthread_mutex_lock(&Mutex_print);
        mode = (mode + 1)%2;
        for (j=DIM_PISTE_Y+2-1;j>=0; j--) {
            for(i=0;i<DIM_PISTE_X+2;i++) {printf("%c ",CASE_Print(i,j,mode));}
            printf("\n");
        }
        for (i=0;i<NUMBER_ROBOTS;i++)
        printf("\n rob[%d]-step[%03ld]-pos[%02d-%02d] statut[%s] rang [%02d] dir[%c]", tab_robot[i].number,tab_robot[i].step,
           tab_robot[i].pos.x,tab_robot[i].pos.y,StatusSym[tab_robot[i].statut], tab_robot[i].rang, DirSymb[ tab_robot[i].dir]);
        printf("\n");
        pthread_mutex_unlock(&Mutex_print);
        usleep(USTIMESTEP/2);
        pthread_mutex_lock(&Mutex_print);
        mode = (mode + 1)%2;
        for (j=DIM_PISTE_Y+2-1;j>=0; j--) {
            for(i=0;i<DIM_PISTE_X+2;i++) printf("%c ",CASE_Print(i,j,mode));
            printf("\n");
        }
        for (i=0;i<NUMBER_ROBOTS;i++)
            printf("\n rob[%d]-step[%03ld]-pos[%02d-%02d] statut[%s] rang [%02d] dir[%c]", tab_robot[i].number,tab_robot[i].step,
                   tab_robot[i].pos.x,tab_robot[i].pos.y,StatusSym[tab_robot[i].statut], tab_robot[i].rang,DirSymb[ tab_robot[i].dir]);
        printf("\n");
        pthread_mutex_unlock(&Mutex_print);
        usleep(USTIMESTEP/2);
        pthread_mutex_lock(&Mutex_print);
        if (NumRobotArrives+NumRobotArretes>=NUMBER_ROBOTS){
            printf("  *** FIN DE SIMULATION : Robots arrives/arretes ***  ");
            getchar(); exit(1);
        }
        pthread_mutex_unlock(&Mutex_print);
        if (ROBOT_DEBUG) getchar();
    }
}

POINT_T POINT_Add (POINT_T pt1, POINT_T pt2){
    POINT_T thepoint;
    thepoint.x = pt1.x+pt2.x;
    thepoint.y = pt1.y+pt2.y;
    return thepoint;
}

BOOL POINT_Egaux(POINT_T pt1, POINT_T pt2){
    return ((pt1.x==pt2.x)&&(pt1.y==pt2.y));
}

POINT_T ROBOT_Point_devant(int numrob) {
    return POINT_Add (tab_robot[numrob].pos, tab_dir[tab_robot[numrob].dir]);
}

CASE_T ROBOT_Case_devant( int numrob ){
    return (map[ROBOT_Point_devant(numrob).x][ROBOT_Point_devant(numrob).y]);
}

void ROBOT_Init(int num, int xi, int yi,  int xf, int yf, DIR_T dir){
    tab_robot [num].number = num;
    tab_robot [num].pos.x = xi;
    tab_robot [num].pos.y = yi;
    tab_robot [num].initiale.x = xi;
    tab_robot [num].initiale.y = yi;
    tab_robot [num].finale.x = xf;
    tab_robot [num].finale.y = yf;
    tab_robot [num].statut = ROUTE;
    tab_robot [num].dir = dir;
    tab_robot [num].step = 0;
    tab_robot [num].rang = -1;
    tab_robot [num].watchdog_counter = 0;
}

void MAP_Place_obstacle(int x, int y){
    map[x][y]=OBSTACLE;
    map[DIM_PISTE_Y+2-1-y][x]=OBSTACLE;
    map[DIM_PISTE_X+2-1-x][DIM_PISTE_Y+2-1-y]=OBSTACLE;
    map[y][DIM_PISTE_Y+2-1-x]=OBSTACLE;
}

void MAP_Refresh(void){
    int i,j;
#if  CONFIG_SIMPLE
    MAP_Place_obstacle(1,5);
#endif
#if CONFIG_LIGNE
    MAP_Place_obstacle(2,5);
    MAP_Place_obstacle(3,5);
    MAP_Place_obstacle(4,5);
#endif
#if CONFIG_L
    MAP_Place_obstacle(4,4);
    MAP_Place_obstacle(4,3);
    MAP_Place_obstacle(4,2);
#endif
#if  CONFIG_SIMPLE
    MAP_Place_obstacle(6,6);
#endif
#if CONFIG_LIGNE
    MAP_Place_obstacle(6,7);
#endif
#if CONFIG_L
    MAP_Place_obstacle(7,6);
#endif

    for(i=0;i<DIM_PISTE_X+2;i++) {
        MAP_Place_obstacle(i,0);
    }
    for (i=1;i<DIM_PISTE_X+2-1;i++)for(j=1;j<DIM_PISTE_Y+2-1;j++){
     if ((map[i][j] !=OBSTACLE)&&(map[i][j] !=TRAJECTOIRE))  map[i][j] = VIDE;
    }
    for(i=0;i<NUMBER_ROBOTS;i++)
        map[tab_robot[i].pos.x][tab_robot[i].pos.y] = ROBOT;
}

void ROBOT_Avance(int numrob){
 	Watchdog_reset(numrob);
    pthread_mutex_lock(&Mutex_bouge);
    int therobnum;
    if(tab_robot[numrob].statut != ARRET)
        switch (ROBOT_Case_devant(numrob)){
            case VIDE :
            case TRAJECTOIRE : 
            	map[tab_robot[numrob].pos.x][tab_robot[numrob].pos.y]=TRAJECTOIRE;
                tab_robot[numrob].pos = POINT_Add (tab_robot[numrob].pos,tab_dir[tab_robot[numrob].dir]);
                break;
            case OBSTACLE :
                break;
            case ROBOT :
                if (TEST_COLLISION){
                    tab_robot[numrob].statut = ARRET ;NumRobotArretes++;
                    // on chercher le robot en question pour le mettre aussi en statut ARRET.
                    for (therobnum=0; therobnum<NUMBER_ROBOTS; therobnum++){
                        if ((numrob!= therobnum) &&         POINT_Egaux(ROBOT_Point_devant(numrob),tab_robot[therobnum].pos )){
                            tab_robot[therobnum].statut = ARRET ;NumRobotArretes++;}
                    }
                }
                break;
        }
    pthread_mutex_unlock(&Mutex_bouge);
    Scheduler(numrob);
}

BOOL ROBOT_Peut_avancer(int numrob){
	Watchdog_incremete(numrob);
    return ((ROBOT_Case_devant(numrob)==VIDE )||(ROBOT_Case_devant(numrob)==TRAJECTOIRE));
}

BOOL ROBOT_Arrive(int numrob) {
	Watchdog_incremete(numrob);
    return POINT_Egaux(tab_robot[numrob].pos,tab_robot[numrob].finale);
}
DIR_T ROBOT_Direction(int numrob){
	Watchdog_incremete(numrob);
    return tab_robot[numrob].dir;
}

void ROBOT_gare (int numrob){
    map[tab_robot[numrob].pos.x ][tab_robot[numrob].pos.y]=TRAJECTOIRE;
    tab_robot[numrob].pos.x = (tab_robot[numrob].initiale.x <8  )?0:DIM_PISTE_X+1;
    tab_robot[numrob].pos.y = (tab_robot[numrob].initiale.y <8  )?0:DIM_PISTE_Y+1;
    if (tab_robot[numrob].statut != ARRET){tab_robot[numrob].statut = ARRET;NumRobotArretes++;}
    map[tab_robot[numrob].pos.x ][tab_robot[numrob].pos.y]=ROBOT;
}

void  Scheduler(int numrob){
    int i;
    if(tab_robot[numrob].statut==ROUTE){
        tab_robot[numrob].step++;
        if( (tab_robot[numrob].step>9)&&(POINT_Egaux(tab_robot[numrob].pos,tab_robot[numrob].initiale)) ){
            ROBOT_gare(numrob);
         }
#if TEST_ZONE_DEPART
        for (i=0;i<NUMBER_ROBOTS;i++)
            if (POINT_Egaux(tab_robot[numrob].pos,tab_robot[i].initiale) && (i!=numrob)&&
                !POINT_Egaux(tab_robot[numrob].pos,tab_robot[numrob].finale) ){
                ROBOT_gare(numrob);
        }
#endif
    MAP_Refresh();
    }
    MAP_Print();
    if (tab_robot[numrob].step>MAX_STEP){
        printf(" FIN DE SIMULATION : Nombre d'iteration maximal atteint (%d)",MAX_STEP);
        getchar(); exit(1);
    }
    if (NUMBER_ROBOTS>1) pthread_barrier_wait(&barrier1);
}

void ROBOT_Tourne_a_droite( int numrob ){
 	Watchdog_reset(numrob);
    if(tab_robot[numrob].statut != ARRET)
        tab_robot[numrob].dir = tab_droite [tab_robot[numrob].dir];
    Scheduler(numrob);
}

void ROBOT_Tourne_a_gauche( int numrob ){
 	Watchdog_reset(numrob);
    if(tab_robot[numrob].statut != ARRET)
        tab_robot[numrob].dir = tab_gauche [tab_robot[numrob].dir];
    Scheduler(numrob);
}

int ROBOT_Pos_x(int numrob) {
	Watchdog_incremete(numrob);
    return tab_robot[numrob].pos.x;
}

int ROBOT_Pos_y(int numrob) {
	Watchdog_incremete(numrob);
    return tab_robot[numrob].pos.y;
}

void ROBOT_Creve_ballon_et_stop(int numrob){
 	Watchdog_reset(numrob);
    if (ROBOT_Arrive( numrob)) {
        NumRobotArrives++;
        tab_robot[numrob].rang=NumRobotArrives_synchrone+1;
        tab_robot[numrob].statut=ARRIVE;
    }
    else  {tab_robot[numrob].statut = ARRET;NumRobotArretes++;}
    if(POINT_Egaux(tab_robot[numrob].pos,tab_robot[numrob].initiale))
        ROBOT_gare(numrob);
    while(1)Scheduler( numrob);
}

void Watchdog_incremete(int robnum){
	tab_robot[robnum].watchdog_counter++; 
	if (tab_robot[robnum].watchdog_counter > MAX_WATCHDOG ) {
		Watchdog_reset(robnum);
		Scheduler( robnum );
	}
}
void Watchdog_reset(int robnum){
	tab_robot[robnum].watchdog_counter = 0; 
}


// fonctions crochet pour les algorithmes des 4 robots

void *Robot_AlgorithmeSO_local(void *threadid){
    ROB0_AlgorithmeSO();
    ROBOT_Creve_ballon_et_stop( 0);
    return NULL;
}

void *Robot_AlgorithmeNO_local(void *threadid){
    ROB1_AlgorithmeNO();
    ROBOT_Creve_ballon_et_stop( 1);
    return NULL;
}

void *Robot_AlgorithmeNE_local(void *threadid){
    ROB2_AlgorithmeNE();
    ROBOT_Creve_ballon_et_stop( 2);
    return NULL;
}

void *Robot_AlgorithmeSE_local(void *threadid){
    ROB3_AlgorithmeSE();
    ROBOT_Creve_ballon_et_stop( 3);
    return NULL;
}
BOOL ROB0_Peut_avancer(void){return ROBOT_Peut_avancer(0);}
DIR_T ROB0_Direction (void){return ROBOT_Direction (0);}
int ROB0_Pos_x(void) {return ROBOT_Pos_x(0);}
int ROB0_Pos_y(void) {return ROBOT_Pos_y(0);}
BOOL ROB0_Arrive(void)  {return ROBOT_Arrive(0);}
void ROB0_Avance(void) { ROBOT_Avance(0);}
void ROB0_Tourne_a_droite(void) { ROBOT_Tourne_a_droite(0);}
void ROB0_Tourne_a_gauche(void){ ROBOT_Tourne_a_gauche(0);}
void ROB0_Creve_ballon_et_stop(void){ ROBOT_Creve_ballon_et_stop(0);}

BOOL ROB1_Peut_avancer(void){return ROBOT_Peut_avancer(1);}
DIR_T ROB1_Direction(void) {return ROBOT_Direction (1);}
int ROB1_Pos_x(void) {return ROBOT_Pos_x(1);}
int ROB1_Pos_y(void)  {return ROBOT_Pos_y(1);}
BOOL ROB1_Arrive(void) {return ROBOT_Arrive(1);}
void ROB1_Avance(void){ ROBOT_Avance(1);}
void ROB1_Tourne_a_droite(void){ ROBOT_Tourne_a_droite(1);}
void ROB1_Tourne_a_gauche(void){ ROBOT_Tourne_a_gauche(1);}
void ROB1_Creve_ballon_et_stop(void){ ROBOT_Creve_ballon_et_stop(1);}

BOOL ROB2_Peut_avancer(void){return ROBOT_Peut_avancer(2);}
DIR_T ROB2_Direction (void){return ROBOT_Direction (2);}
int ROB2_Pos_x(void) {return ROBOT_Pos_x(2);}
int ROB2_Pos_y(void) {return ROBOT_Pos_y(2);}
BOOL ROB2_Arrive(void)  {return ROBOT_Arrive(2);}
void ROB2_Avance(void){ ROBOT_Avance(2);}
void ROB2_Tourne_a_droite(void){ ROBOT_Tourne_a_droite(2);}
void ROB2_Tourne_a_gauche(void){ ROBOT_Tourne_a_gauche(2);}
void ROB2_Creve_ballon_et_stop(void){ ROBOT_Creve_ballon_et_stop(2);}

BOOL ROB3_Peut_avancer(void){return ROBOT_Peut_avancer(3);}
DIR_T ROB3_Direction (void){return ROBOT_Direction (3);}
int ROB3_Pos_x(void) {return ROBOT_Pos_x(3);}
int ROB3_Pos_y(void) {return ROBOT_Pos_y(3);}
BOOL ROB3_Arrive(void)  {return ROBOT_Arrive(3);}
void ROB3_Avance(void){ ROBOT_Avance(3);}
void ROB3_Tourne_a_droite(void){ ROBOT_Tourne_a_droite(3);}
void ROB3_Tourne_a_gauche(void){ ROBOT_Tourne_a_gauche(3);}
void ROB3_Creve_ballon_et_stop(void){ ROBOT_Creve_ballon_et_stop(3);}
