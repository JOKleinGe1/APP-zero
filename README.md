Semaine "APP-zero"
(apprentissage par problèle semaine zero).

Organisation des trois jours de rentrée de S1 dans le département GEii-1 de l'IUT CACHAN : 

Le document principal s'appelle "APP0-MR2D2_2025.odt" 
Il contient un script (macro) pour générer, dans le reprétoire "a_imprimer/", les documents à destination des tuteurs et des étudiants.  

Le document "demi_ponts.pdf" contient les instructions pour le brise glace utilisé lors de la 1ere demi journée. 

Le simulateur de robot, pour simuler la compétition lors de la dernière demi-journée (jour 3) se trouve dans "mr2d2_2022/"

Les algorithmes produits par les étudiants viennent remplacer les codes sources dans les fichiers mr2d2_X.c (X=0…3). 
Un backuop des algos par défaut se trouve dans le répertoire "mr2d2_2022/defaultalgo/".

Le makefile produit des executables pour différents niveaux de difficulté (pas d'obstacle, obstacles isolés, en ligne, en coin, touchant les bords). 

Le simulateur est dans le fichier . 
Les obstacle y sont définis dans la fonction void MAP_Refresh(void) à partir de la ligne 232. 
