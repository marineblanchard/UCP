/******************************************************/
/* Fichier main.cpp                                   */
/* Fonction principale                                */
/******************************************************/

#include "probleme.h"

int main(int argc, char * argv[]){
	std::string nom_fich;
    Probleme pb;
    Solution s;

	if(argc > 1)
	{
		nom_fich = argv[1];
	}
	else
	{
		nom_fich = "data10.txt"; // fichier par defaut 
	}

    // lecture de l'instance
    pb.lit(nom_fich);
    pb.affiche_instance();

    // creation du fichier LP
  	nom_fich.replace(nom_fich.rfind('.'), 4, ".lp");
    pb.remplit_quadra(); // remplit la matrice des valeurs de la fonction quadratique
  	pb.ecritLP(nom_fich);

  	// resolution du probleme LP généré et affichage de la solution
    pb.optimise(nom_fich,s);

    return 0;
}

