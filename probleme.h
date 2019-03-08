/******************************************************/
/* Fichier problem.h                                  */
/* Lecture, affichage de l'instance et création du LP */
/******************************************************/

#ifndef PROBLEME_H
#define PROBLEME_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "solution.h"

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

typedef unsigned int uint;

const double EPSILON = 1.0E-6;

class Probleme {
private:
    int _N; //nombre de générateur
    int _T; //nombre de période de temps
    std::vector<int> _demande; //vecteur des demandes
    std::vector<std::vector<double> > _matrix; //matrice des paramètres
    std::vector<std::vector<double> > _quadra; //matrice des valeurs de la fonction quadratique
    std::vector<std::vector<double> > _coeffDroite; // matrice des coordonnées des morceaux
    std::vector<std::vector<double> > _origine; // matrice des coordonnées des morceaux

public:
    //Constructeurs
    Probleme() : _N(0), _T(0){};
    Probleme(int N, int P) : _N(0), _T(0){};  

    //Getteurs
    int getN(void) const 
    {
        return _N;
    };
    int getT(void) const 
    {
        return _T;
    };
    std::vector<int>& getDemande(void){
    	return _demande;
    }
    std::vector<std::vector<double> >& getMatrix(void) {
        return _matrix;
    };

    //fonctions
        void lit (const std::string & nom); // lecture des instances
        void affiche_instance (); // affichage des instances
        void remplit_quadra (); // remplissage de la matrice des coefficients des droites de regression linéaire 
        double fonctionQuadra (double x, int unit); //fonction quadratique décrivant le cout de production d'une unité
        void ecritLP (const std::string & nom) const; // écrit le LP dans un fichier texte
        void optimise (const std::string & nom, Solution & s); // lance la resolution
};
#endif
