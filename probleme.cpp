#include "probleme.h"

/*-------------------------*/
/* Lecture de l'instance   */
/*-------------------------*/
void Probleme::lit(const std::string & nom)
{
    std::ifstream instance(nom.c_str(), std::ios::in);
    std::string ligne;
    int demande_t;
        if (instance)
        {
            //lecture du nombre de machines et de periodes
            instance >> _N;
            instance >> _T;
            
            //std::getline(instance, ligne); //ligne vide
            for(int i = 0; i != _T; i++)
            {
            	instance >> demande_t;
    			_demande.push_back(demande_t);
    		}
            
            //lecture des paramétres des machines
            while(std::getline(instance, ligne))
                {
                    _matrix.push_back(std::vector<double>());
                    std::stringstream ss(ligne);
                    double val;
                    while(ss >> val)
                    {
                        _matrix.back().push_back(val);
                    }
                }
        }
}

//fonction quadratique : coût lié au carburant de l'unité x pendant la période t
double Probleme::fonctionQuadra(double x, int unit)
{
	return _matrix[3][unit]+(_matrix[4][unit]*x)+(_matrix[5][unit]*x*x);
	
}

/*---------------------------------------------------------------*/
/* Création de la matrice des valeurs de la fonction quadratique */
/*---------------------------------------------------------------*/
void Probleme::remplit_quadra(){
	std::vector<double> val;
	
	double Pmax;
	double Pmin;
	double P1; // point à 1/3
	double P2; // point à 2/3

	double F_Pmax;
	double F_Pmin;
	double F_P1;
	double F_P2;

    double alpha1;
    double alpha2;
    double alpha3;

    double beta1;
    double beta2;
    double beta3;

	for(int i = 0; i<_N; i++)
	{
		Pmax = _matrix[1][i];
		Pmin = _matrix[2][i];
		P1 = Pmin + (Pmax - Pmin) / 3;
		P2 = Pmin + 2 * (Pmax - Pmin) / 3;

		F_Pmax = fonctionQuadra(Pmax,i);
		F_Pmin = fonctionQuadra(Pmin,i);
		F_P1 = fonctionQuadra(P1,i);
		F_P2 = fonctionQuadra(P2,i);

		val.push_back(F_Pmin);
		val.push_back(F_P1);
		val.push_back(F_P2);
		val.push_back(F_Pmax);

		_quadra.push_back(val);
        val.clear();

		// Calcul des coefficients des droites de regressions
            // Coefficient directeur
        alpha1 = (F_P1 - F_Pmin)/(P1 - Pmin);
        alpha2 = (F_P2 - F_P1)/(P2 - P1);
        alpha3 = (F_Pmax - F_P2)/(Pmax - P2);

        val.push_back(alpha1);
        val.push_back(alpha2);
        val.push_back(alpha3);
        
        _coeffDroite.push_back(val);
        val.clear();

            // ordo à l'origine
        beta1 = F_P1 - alpha1*P1;
        beta2 = F_P2 - alpha2*P2;
        beta3 = F_Pmax - alpha3*Pmax;

        val.push_back(beta1);
        val.push_back(beta2);
        val.push_back(beta3);

        _coeffDroite.push_back(val);
        val.clear();

	}

	// Affichage de la matrice des coûts

	std::cout<<std::endl;
    std::cout<<"-----------------------------"<<std::endl<<std::endl;
    std::cout<<"Matrice _quadra"<<std::endl<<std::endl;
   
    for(int i = 0; i < _quadra.size(); i++)
                  {
                    for(int j = 0; j < _quadra[i].size(); j++)
                    {
                      std::cout << _quadra[i][j] << ' ';
                    }
                    std::cout << std::endl;
                  }
    std::cout<<"-----------------------------"<<std::endl<<std::endl;

    std::cout<<"Matrice _coeffDroite"<<std::endl<<std::endl;
   
    for(int i = 0; i < _coeffDroite.size(); i++)
                  {
                    for(int j = 0; j < _coeffDroite[i].size(); j++)
                    {
                      std::cout << _coeffDroite[i][j] << ' ';
                    }
                    std::cout << std::endl;
                  }
    std::cout<<"-----------------------------"<<std::endl<<std::endl;
}



/*-------------------------*/
/* Affichage de l'instance */
/*-------------------------*/
void Probleme::affiche_instance()
{
    std::cout<<"-----------------------------"<<std::endl<<std::endl;
    std::cout<<"affichage de l'instance "<<std::endl<<std::endl;
    std::cout<<"-----------------------------"<<std::endl<<std::endl;
    std::cout<<"Nombre d'unités : "<< _N<<std::endl<<std::endl;
    std::cout<<"Nombre de périodes de temps : "<< _T<<std::endl<<std::endl;
    std::cout<<"-----------------------------"<<std::endl<<std::endl;

    std::cout<<"Demande : ";
    for(int i = 0; i != _demande.size(); i++){
    	std::cout << _demande[i] <<" ";
    }

    std::cout<<std::endl;
    std::cout<<"-----------------------------"<<std::endl<<std::endl;
   
    for(int i = 1; i < _matrix.size(); i++)
                  {
                    for(int j = 0; j < _matrix[i].size(); j++)
                    {
                      std::cout << _matrix[i][j] << ' ';
                    }
                    std::cout << std::endl;
                  }
    std::cout<<"-----------------------------"<<std::endl<<std::endl;    

}

/*-------------------------*/
/* Génération du LP        */
/*-------------------------*/
void Probleme::ecritLP (const std::string & nom) const
{
    std::ofstream of;

    // ouverture/creation du fichier
    of.open(nom.c_str());
    if (of.good() == false)
    {
        std::cerr << "Erreur : impossible d'ouvrir " << nom << std::endl;
        exit(EXIT_FAILURE);
    }

    // ecriture de la fonction objectif
    of << "\\problem " << nom << std::endl << std::endl << "Min" << std::endl << "obj:";
    for (uint t = 0; t<_T; t++) // boucle sur les periodes de temps
    {
        for (uint u = 0; u<_N;u++) // boucle sur les colonnes ou unites
        {
            of<<"+ Z_"<<u<<"_"<<t; //cout de l'unité u pendant t (surestimation de F(pu_t))
            of<<" + "<<_matrix[8][u]<<"sHot_"<<u<<"_"<<t<<" + "<<_matrix[9][u]<<"sCold_"<<u<<"_"<<t; //cout de démarrage de u pendant t 
            //of<<"+ H_"<<u<<"_"<<t; //cout d'extinction de u
        }
    }
    of<<std::endl;

    // écriture des contraintes
    of << std::endl << "Subject To" << std::endl;

    	// 1. Linéarisation par morceaux (3 droites par unité)
    for (uint t = 0 ; t<_T ; t++)
    {
        for (uint u = 0 ; u<_N ; u++)
        {
    	   of<<"Z_"<<u<<"_"<<t<<" - "<<_coeffDroite[(u*2)][0]<<" p_"<<u<<"_"<<t<<" >= "<<_coeffDroite[(u*2)+1][0]<<std::endl; // droite 1
           of<<"Z_"<<u<<"_"<<t<<" - "<<_coeffDroite[u][1]<<" p_"<<u<<"_"<<t<<" >= "<<_coeffDroite[u+1][1]<<std::endl; // droite 2
           of<<"Z_"<<u<<"_"<<t<<" - "<<_coeffDroite[u][2]<<" p_"<<u<<"_"<<t<<" >= "<<_coeffDroite[u+1][2]<<std::endl; // droite 3
           
        }
	}

    	// 2. Contrainte du système
    		// 2.1 Bilan de puissance (1)
    for (uint t = 0 ; t<_T ; t++) 
    {
    	for (uint u = 0 ; u<_N ; u++)
        {
        	of<<"+ p_"<<u<<"_"<<t;
        }
     	of<<" = "<<_demande[t]<<std::endl; 		
    }
    		// 2.2 Reserves tournantes requisent (10% de la demande selon Kazarlis) (2)
    for (int t = 0; t<_T ; t++) 
    {
    	for (int u = 0 ; u<_N ; u++) 
        {
        	of<<"+ pmax_"<<u<<"_"<<t;
        }
     	of<<" >= "<<1.10*_demande[t]<<std::endl; 	
    }
           // conditions sur pmax
    for (int t = 1; t<_T ; t++) 
    {
        for (int u = 0 ; u<_N ; u++) 
        {
            of<<"pmax_"<<u<<"_"<<t<<" - "<<_matrix[1][u]<<"y_"<<u<<"_"<<t<<" <= "<<0<<std::endl;
        }
    }
    for (int t = 1; t<_T ; t++) 
    {
        for (int u = 0 ; u<_N ; u++) 
        {
            of<<"pmax_"<<u<<"_"<<t<<" - "<<"p_"<<u<<"_"<<t-1<<" - "<<_matrix[2][u]<<"y_"<<u<<"_"<<t-1;
            of<<" - "<<"y_"<<u<<"_"<<t-1<<" <= "<<_matrix[1][u]<<std::endl;
        }
    }

    	// 3. Temps minimum de fonctionnement et d'arret (min up/down)
    		// Etat initial - période 0 (3) (4)
    double theta_ON;
    double theta_OFF;

    for (int u = 0 ; u<_N ; u++) 
    {
        theta_ON = std::max(0.0,_matrix[6][u]-_matrix[11][u]);
        theta_OFF = std::max(0.0,_matrix[7][u]+_matrix[11][u]);

        if(_matrix[11][u]>0) // la machine est démarrée avant le début de la plannification
        {
            for (uint t = 0; t<theta_ON ; t++)
            {
                of<<"y_"<<u<<"_"<<t<<"="<<1<<std::endl;
            }
        }else // la machine est éteinte
        {
            for (uint t = 0; t<theta_OFF ; t++)
            {
                of<<"y_"<<u<<"_"<<t<<"="<<0<<std::endl;
            } 
        }   
    }

            // Reste de la planification (5) (6)
    double tau_ON;
    double tau_OFF;

    for (uint u = 0 ; u<_N ; u++) 
    {
        for (uint t = 0 ; t<_T ; t++)
        {
            tau_ON = std::max(t-_matrix[6][u],.0);
            tau_OFF = std::max(t-_matrix[7][u],0.0);

            for (uint i = tau_ON ; i<=t ; i++)
            {
                of<<"+ x_ON_"<<u<<"_"<<i;
            }
            of<<" - "<<"y_"<<u<<"_"<<t<<" <= "<<0<<std::endl;
            for (uint i = tau_OFF ; i<=t ; i++)
            {
                of<<"+ x_OFF_"<<u<<"_"<<i;
            }
            of<<" + "<<"y_"<<u<<"_"<<t<<" <= "<<1<<std::endl;
        }
    }
    

    	// 4. Limite de production (7)
    for (uint t = 0 ; t<_T ; t++) 
    {
    	for (uint u = 0 ; u<_N;u++) 
        {
            of<<"p_"<<u<<"_"<<t<<"-"<<_matrix[1][u]<<"y_"<<u<<"_"<<t<<"<="<<0<<std::endl;
        }
    }
    for (uint t = 0 ; t<_T ; t++) 
    {
    	for (uint u = 0; u<_N ; u++) 
        {
        	of<<"p_"<<u<<"_"<<t<<"-"<<_matrix[2][u]<<"y_"<<u<<"_"<<t<<">="<<0<<std::endl;
        }
    }

    	// 5. Rampes (8) (9) (coefficient de montée et de descente égales à Pmin suivant l'article de Viana)
    for (uint t = 1 ; t<_T ; t++) 
    {
        for (uint u = 0 ; u<_N ; u++) 
        {
            of<<"p_"<<u<<"_"<<t<<" - "<<"p_"<<u<<"_"<<t-1<<"<="<<_matrix[2][u]<<std::endl;
            of<<"p_"<<u<<"_"<<t-1<<" - "<<"p_"<<u<<"_"<<t<<"<="<<_matrix[2][u]<<std::endl;
        }
    }

    	//6. Cout de démarrage 
            // 6.1 initialisation des variables sHot et sCold (10)
    for (int t = 0 ; t<_T ; t++) 
    {
        for (int u = 0 ; u<_N ; u++) 
        {
            of<<"sHot_"<<u<<"_"<<t<<" + "<<"sCold_"<<u<<"_"<<t<<" - "<<"x_ON_"<<u<<"_"<<t<<" = "<<0<<std::endl;
        }
    }

           // 6.2 Determination du coup de démarrage (à chaud ou à froid) (11)
   for (uint t = 0; t<_T ; t++) 
    {
        for (uint u = 0; u<_N ; u++) 
        {
            double dep = t-_matrix[10][u]-1;
			of<<"y_"<<u<<"_"<<t<<" - "<<"sCold_"<<u<<"_"<<t;
            if (dep>=0)
            {
                for (uint i = dep ; i<=(t-1) ; i++)
                {
                    of<<" - y_"<<u<<"_"<<i;
                }
                of<<"<="<<0<<std::endl;  
            }
            else 
            {
            	if (t>0){
            		for (uint i = 0 ; i<=(t-1) ; i++)
                	{
                    	of<<" - y_"<<u<<"_"<<i;
                	}
                	of<<"<="<<0<<std::endl;
            	}
            	else
            	{
            		of<<"<="<<_matrix[12][u]<<std::endl;
            	}
            }
        }
    }
            // 6.3 parametrage des variables x_ON et x_OFF (12) (13)
    		// état inital
    
	for (uint u = 0; u<_N;u++) 
    {
    	of<<"y_"<<u<<"_"<<0<<" - "<<"x_ON_"<<u<<"_"<<0<<" <= "<<_matrix[12][u]<<std::endl;
    	of<<"x_OFF_"<<u<<"_"<<0<<" - "<<"x_ON_"<<u<<"_"<<0<<" + "<<"y_"<<u<<"_"<<0<<" = "<<_matrix[12][u]<<std::endl;

    	for (uint t = 1; t<_T ; t++) 
    	{
            of<<"y_"<<u<<"_"<<t<<" - "<<"y_"<<u<<"_"<<t-1<<" - "<<"x_ON_"<<u<<"_"<<t<<" <= "<<0<<std::endl; // determine le switch ON
            of<<"y_"<<u<<"_"<<t-1<<" - "<<"y_"<<u<<"_"<<t  <<" + "<<"x_ON_"<<u<<"_"<<t<<" - "<<"x_OFF_"<<u<<"_"<<t<<" = "<<0<<std::endl; // determine le switch OFF
        }
    }

    // Variables binaires
    of << std::endl << "Binaries" << std::endl;
    for (uint t = 0; t<_T ; t++) 
    {
        for (uint u = 0; u<_N;u++) 
        {
            of<<"y_"<<u<<"_"<<t<<std::endl;
            of<<"sHot_"<<u<<"_"<<t<<std::endl;
            of<<"sCold_"<<u<<"_"<<t<<std::endl;
        }
    }

    of << "End"; 

    // fermeture du fichier
    of.close();
}


//-----------------------------------------------------------------------------
// lit le fichier lp, lance l'optimisation, recupere et affiche les resultats
//-----------------------------------------------------------------------------

void Probleme::optimise (const std::string & nom, Solution & s)
{
    // creation de l'environnement
    IloEnv env;
    try
    {
        // creation du modele dans l'environnement
        IloModel model(env);

        // creation d'un solveur
        IloCplex cplex(env);

        // passe en mode silencieux
        cplex.setOut(env.getNullStream());

        // chargement du fichier LP dans le solveur
        IloObjective obj;
        IloNumVarArray var(env); // NOTE: NOM DE CHAQUE VARIABLE
        IloRangeArray rng(env);

        cplex.importModel(model, nom.c_str(), obj, var, rng);

        // optimisation
        cplex.extract(model);
        if (cplex.solve() == false)
        {
        env.error() << "Impossible d'optimiser le LP" << std::endl;
        throw (-1);
        }

        // affiche la solution = valeur de chaque variable
        IloNumArray vals(env);
        cplex.getValues(vals, var); // NOTE : i = indice, var[i] = nom, vals[i] = valeur

        env.out() << "Etat de la solution       = " << cplex.getStatus() << endl;
        env.out() << "Cout total de production  = " << cplex.getObjValue() << " $" << endl;
    	env.out() << "temps CPU                 = " << cplex.getTime() << " s " << std::endl;

        // on recupere la valeur optimale
        //g.setValeur(cplex.getObjValue());

        cplex.getObjValue();

        double** res; // instanciation de la matrice

        res = s.constSol(_N,_T); //allocation de la matrice des résultats

        for (int i = 0; i < vals.getSize(); ++i)
        {
                uint a, b;
                a = b = 0;

                if (sscanf(var[i].getName(), "y_%u_%u", &a, &b) == 2)
                { 
                	// var[i].getName() : permet de récupérer le nom de la variable
                	// sscanf : permet ici d'extraire les indices
                    s.setSol(res,a, b, vals[i]); // remplissage de la matrice avec les valeurs de le variable y
                }
        }
        std::cout <<"--------"<<std::endl;
        std::cout <<"| y_u_t |"<<std::endl;
        std::cout <<"--------"<<std::endl;
        s.afficheSol(res); // affichage de la matrice
        
        for (int i = 0; i < vals.getSize(); ++i)
        {
            //std::cout << var[i].getName() << std::endl;

            uint a, b;
            a = b = 0;

            if (sscanf(var[i].getName(), "p_%u_%u", &a, &b) == 2)
            {
                s.setSol(res, a, b, vals[i]);
            }
        }
        std::cout <<"--------"<<std::endl;
        std::cout <<"| p_u_t |"<<std::endl;
        std::cout <<"--------"<<std::endl;
        s.afficheSol(res);

        for (int i = 0; i < vals.getSize(); ++i)
        {
            uint a, b;
            a = b = 0;

            if (sscanf(var[i].getName(), "x_ON_%u_%u", &a, &b) == 2)
            {
                s.setSol(res, a, b, vals[i]);
            }
        }
        std::cout <<"-----------"<<std::endl;
        std::cout <<"| x_ON_u_t |"<<std::endl;
        std::cout <<"-----------"<<std::endl;
        s.afficheSol(res);

        for (int i = 0; i < vals.getSize(); ++i)
        {
            uint a, b;
            a = b = 0;

            if (sscanf(var[i].getName(), "x_OFF_%u_%u", &a, &b) == 2)
            {
                s.setSol(res, a, b, vals[i]);
            }
        }
        std::cout <<"-----------"<<std::endl;
        std::cout <<"| x_OFF_u_t |"<<std::endl;
        std::cout <<"-----------"<<std::endl;
        s.afficheSol(res);

        for (int i = 0; i < vals.getSize(); ++i)
        {
            uint a, b;
            a = b = 0;

            if (sscanf(var[i].getName(), "sHot_%u_%u", &a, &b) == 2)
            {
                s.setSol(res, a, b, vals[i]);
            }
        }
        std::cout <<"-----------"<<std::endl;
        std::cout <<"| sHot_u_t |"<<std::endl;
        std::cout <<"-----------"<<std::endl;
        s.afficheSol(res);

        for (int i = 0; i < vals.getSize(); ++i)
        {
        	if (vals[i] > EPSILON)
      		{
      			uint a, b;
	            a = b = 0;

	            if (sscanf(var[i].getName(), "sCold_%u_%u", &a, &b) == 2)
	            {
	                s.setSol(res, a, b, vals[i]);
	            }
      		}
            
        }
        std::cout <<"-----------"<<std::endl;
        std::cout <<"| sCold_u_t |"<<std::endl;
        std::cout <<"-----------"<<std::endl;
        s.afficheSol(res);

		for (int i = 0; i < vals.getSize(); ++i)
        {
            uint a, b;
            a = b = 0;

            if (sscanf(var[i].getName(), "pmax_%u_%u", &a, &b) == 2)
            {
                s.setSol(res, a, b, vals[i]);
            }
        }
        std::cout <<"-----------"<<std::endl;
        std::cout <<"| pmax_u_t |"<<std::endl;
        std::cout <<"-----------"<<std::endl;
        s.afficheSol(res);

        std::cout <<"-----  FIN  ------"<<std::endl;
    }
    catch (IloException & e)
    {
        std::cerr << "Exception levee dans l'env : " << e << std::endl;
    }   
    catch (...)
    {
        std::cerr << "Erreur inattendue" << std::endl;
    }

    // fermeture de l'environnement
    env.end();
}






