all: level0 level1 level2 level3

clean:
	rm  level*

level0 : mr2d2_0.c mr2d2_1.c mr2d2_2.c mr2d2_3.c  mr2d2_simul.c RobConfig.h mr2d2.h \
mr2d2_1.h mr2d2_3.h mr2d2_0.h
	gcc -o level0 *.c -DCONFIG_SIMPLE=0 -DCONFIG_LIGNE=0 -DCONFIG_L=0 -DROB1_ACTIVE=1 \
	-DROB2_ACTIVE=1 -DROB3_ACTIVE=1 -DROB0_ACTIVE=1 
level1 : mr2d2_0.c mr2d2_1.c mr2d2_2.c mr2d2_3.c  mr2d2_simul.c RobConfig.h mr2d2.h \
mr2d2_1.h mr2d2_3.h mr2d2_0.h
	gcc -o level1 *.c -DCONFIG_SIMPLE=1 -DCONFIG_LIGNE=0 -DCONFIG_L=0 -DROB1_ACTIVE=1 \
	-DROB2_ACTIVE=1 -DROB3_ACTIVE=1 -DROB0_ACTIVE=1 
level2 : mr2d2_0.c mr2d2_1.c mr2d2_2.c mr2d2_3.c  mr2d2_simul.c RobConfig.h mr2d2.h \
mr2d2_1.h mr2d2_3.h mr2d2_0.h
	gcc -o level2 *.c -DCONFIG_SIMPLE=1 -DCONFIG_LIGNE=1 -DCONFIG_L=0 -DROB1_ACTIVE=1 \
	-DROB2_ACTIVE=1 -DROB3_ACTIVE=1 -DROB0_ACTIVE=1 
level3 : mr2d2_0.c mr2d2_1.c mr2d2_2.c mr2d2_3.c  mr2d2_simul.c RobConfig.h mr2d2.h \
mr2d2_1.h mr2d2_3.h mr2d2_0.h
	gcc -o level3 *.c -DCONFIG_SIMPLE=1 -DCONFIG_LIGNE=1 -DCONFIG_L=1 -DROB1_ACTIVE=1 \
	-DROB2_ACTIVE=1 -DROB3_ACTIVE=1 -DROB0_ACTIVE=1 
