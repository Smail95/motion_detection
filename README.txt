# TABULATION: 4 #
##############################################
# PROJET PROGPAR - Sorbonne (UPMC) 2019/2020 #
# * AIDER Smail								 #
# * SERBEY Samy								 #
##############################################


## PROJET : Détection de mouvement sur processeur multi-coeurs SIMD et sur GPU

## TRAVAIL RÉALISÉ

- Algorithme Sigma-Delta 
- Morphologie Mathématique
- Optimisation SIMD

## FONCTIONNEMENT DE MORPHO EN SIMD

 Matrice 3X8
 Vecteur SIMD de taille 4
 Élément Structurant taille 3X3
 
 pre : bords de gauche 
 suf : bords de groite
 
	Schéma: 	
							  +---------------+----------------+
							  |	1	2	3	3 | 12	13	14	15 |
		   <---- pre ---->	  +---------------+----------------+	<---- suf ---->
		  +---------------+	  +---------------+----------------+   +----------------+
	V0	  |	0	0	0	0 |	  |	0	1	2	3 | 12	13	14	15 |   | 15	 15	 15	 15 |
		  +---------------+	  +---------------+----------------+   +----------------+
	V1	  |	4	4	4	4 |	  |	4	5	6	7 | 16	17	18	19 |   | 19	 19	 19	 19 |
		  +---------------+	  +---------------+----------------+   +----------------+
	V2	  |	8	8	8	8 |	  |	8	9	10	11|	20	21	22	23 |   | 23	 23	 23	 23 |
		  +---------------+	  +---------------+----------------+   +----------------+

	Min/Max(V0, V1, V2):
	
				  PRE				 VEC				SUF
		  +---------------+	  +---------------+	 +----------------+
		  |	0	0	0	0 |	  | 0	1	2   3 |	 | 12  13  14  15 |
		  |	4	4	4	4 |	  | 4	5	6	7 |  | 16  17  18  19 |
		  | 8	8	8	8 |	  | 8	9	10	11|	 | 20  21  22  23 | 
		  +---------------+	  +---------------+	 +----------------+
								   /   /   /    /
	VEXT_S(VEC, SUF, 1):		  /	  /   /	   /
							  +---------------+
							  |	1	2	3  12 |
							  | 5	6	7  16 |
							  | 9	10	11 20 |
							  +---------------+
	VEXT_P(PRE, VEC, 3):	
							  +---------------+
							  |	0	0	1  2  |
							  |	4	4	5  6  |
							  |	8	8	9  10 |
							  +---------------+
	MAX(VEC, VEXT_S, VEXT_P):
							
			+-----------+------------+------------+------------+
			| 0   0   1 | 0   1   2  | 1   2   3  | 2   3   12 |
			| 4   4   5 | 4   5   6  | 5   6   7  | 6   7   16 |
			| 8   8	  9 | 8   9   10 | 9   10  11 | 10  11  20 |
			+-----------+------------+------------+------------+
	RES =>
			+-----------+------------+------------+------------+
			| 8   8   9 | 8   9   10 | 9   10  11 | 10  11  20 |
			+-----------+------------+------------+------------+		


## TRAVAIL NON RÉALISÉ

- Optimisation Domain Specific et fusion / factorisation d’opérateurs
- Optimisation logicielles: pipeline d'opérateurs
- Optimisation des formats de calcul et de stockage en mémoire
- Optimisations matérielles combinées SIMD × OpenMP (ou pthread)

## COMPILATION

- Modifier la variable PROJECT_PATH dans le fichier Makefile.
- Compilation : 
`$ make`
- Execution : 
`$ make run`

## BENCHMARK 

-> Mouvement_C			----------------------+
 - Time		446			ms	| ns : 446437344  |
 - debit	34188		pixel/ms			  |
											  |
-> Mouvement_SIMD		----------------------+
 - Time		60			ms	| ns : 60359744	  |
 - debit	254136		pixel/ms			  |
											  |
-> Morpho3x3_C			----------------------+
 - time		6			s	| ns : 6063135520 |
 - debit	2541366		pixel/s 			  |
											  |
-> Morpho5x5_C			----------------------+
 - time		13			s	| ns : 13725035840|
 - debit	1172938		pixel/s			 	  |
											  |
-> Morpho3x3_SIMD		----------------------+
 - time		1			s	| ns : 1231789120 |
 - debit	15248200	pixel/s				  |
											  |
-> Morpho5x5_SIMD		----------------------+
 - time		2			s	| ns : 2539926080 |
 - debit	7624100		pixel/s				  |
----------------------------------------------+

