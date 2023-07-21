//
//  RobConfig.h
//  mr2d2
//
//  Created by Jacques-Olivier Klein on 26/01/2018.
//  Copyright © 2018 Jacques-Olivier Klein. All rights reserved.
//

#ifndef RobConfig_h
#define RobConfig_h

#define TEST_ZONE_DEPART 1
#define TEST_COLLISION   1
#define ROBOT_DEBUG      0
#define NUMBER_ROBOTS    4
#define MAX_STEP         180
#define USTIMESTEP         500000

#ifndef CONFIG_SIMPLE
#define CONFIG_SIMPLE    1
#endif

#ifndef CONFIG_LIGNE
#define CONFIG_LIGNE     0
#endif

#ifndef CONFIG_L
#define CONFIG_L         0
#endif

#ifndef ROB0_ACTIVE
#define ROB0_ACTIVE     1
#endif

#ifndef ROB1_ACTIVE
#define ROB1_ACTIVE     1
#endif

#ifndef ROB2_ACTIVE
#define ROB2_ACTIVE     1
#endif

#ifndef ROB3_ACTIVE
#define ROB3_ACTIVE     1
#endif

// equipe 1 v1 : (0123)
// equipe 1 v2 : 01(23)x

// equipe 2 v1 : (0123)x
// equipe 2 v2 : (0123) 

// equipe 3 v1 :  (0123) bug
// equipe 3 v2 : (0123 )arrive presque

// equipe 4 v1 :01-(23)x
// equipe 4 v2 :0-(123) (1: tour du plot, 2 : retour maison)

#endif /* RobConfig_h */
