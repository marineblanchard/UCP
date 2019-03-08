/******************************************************/
/* Fichier solution.h                                 */
/* instancie et affiche les matrice des solution      */
/******************************************************/

#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>

class Solution {

public:

	void setSol(double **mat, int i, int j, double val);
	double** constSol(int n, int p);
	void afficheSol(double **mat);
};

#endif