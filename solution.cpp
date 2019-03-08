#include "solution.h"

void Solution::setSol(double **mat, int i, int j, double val)
{
		mat[i][j]=val;
}

double** Solution::constSol(int n, int p)
{
		//alocation matrice resultat
		double** result;
		result = new double* [n];
 		
  		for (int i = 0; i < n; i++) result[i] = new double[p];

    	return result;
}

void Solution::afficheSol(double **mat)
{
	for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 24; j++)
        {
          	if(mat[i][j]>0)
          	{
          		std::cout.width(5);
      			std::cout.fill(' ');
          		std::cout << mat[i][j];
          	}
          	else
          	{
          		std::cout.width(5);
      			std::cout.fill(' ');
          		std::cout << 0;
          	}          		
        }
        std::cout << std::endl;
     }
}