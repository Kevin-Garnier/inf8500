///////////////////////////////////////////////////////////////////////////////
//
//	main.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include <systemc.h>
#include <iostream>
#include "Mult_Matrix.h"
#include "Wrapper.h"
#include "Processeur.h"
#include "DataRAM.h"
#include "Settings.h"

using namespace sc_core;
using namespace std;

#define RAMSIZE (3 * CDIM * CDIM * sizeof(int))  // espace pour A, B et C

// Global variables
bool m_bError = false;

///////////////////////////////////////////////////////////////////////////////
//
//	Main
//
///////////////////////////////////////////////////////////////////////////////
int sc_main(int arg_count, char **arg_value)
{
	// Variables
//	int sim_units = 2; //SC_NS

	// Signaux pour les connexions
	sc_signal<bool> go_signal;
	sc_signal<bool> done_signal;

	// Instanciation des composants
	Processeur iProcesseur("Processeur");
	Mult_Matrix iMult_Matrix("Mult_Matrix");
	Wrapper iWrapper("Wrapper");
	DataRAM iDataRAM("DataRAM", "memory/mem.hex", RAMSIZE, false);

	// Connexion des ports
	iWrapper.dataPortInRAM(iDataRAM);
	iWrapper.dataPortOutRAM(iDataRAM);
	iMult_Matrix.in(iWrapper);
	iMult_Matrix.out(iWrapper);

	iProcesseur.in(iWrapper);
	iProcesseur.out(iWrapper);

	// Connexion des signaux de contrôle
	iProcesseur.go_o(go_signal);
	iMult_Matrix.go_i(go_signal);
	iMult_Matrix.done_o(done_signal);
	iProcesseur.done_i(done_signal);
	// Démarrage de l'application


	#ifdef D40
	std::cout << "Simulation avec D40" << std::endl;
	#endif
	#ifdef D128DMA
	std::cout << "Simulation avec D128DMA" << std::endl;
	#endif
	#ifdef D128
	std::cout << "Simulation avec D128" << std::endl;
	#endif

	if (!m_bError)
	{
		cout << "Demarrage de la simulation." << endl;
		sc_start();
		cout << endl << "Simulation s'est terminee a " << sc_time_stamp() << " ns" << endl;
	}
	// Fin du programme
	return 0;
}
