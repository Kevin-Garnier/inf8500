///////////////////////////////////////////////////////////////////////////////
//
//	main.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include <systemc.h>
#include "Mult_Matrix.h"
#include "Wrapper.h"
#include "DataRAM.h"

#define RAMSIZE 0xFFFF

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

	// Instanciation des composants
	Mult_Matrix iMult_Matrix("Mult_Matrix");
	Wrapper iWrapper("Wrapper");
	DataRAM iDataRAM("DataRAM", "memory/mem.hex", RAMSIZE, false);

	// Connexion des ports
	iWrapper.dataPortInRAM(iDataRAM);
	iWrapper.dataPortOutRAM(iDataRAM);
	iMult_Matrix.in(iWrapper);
	iMult_Matrix.out(iWrapper);

	// Démarrage de l'application
	if (!m_bError)
	{
		cout << "Demarrage de la simulation." << endl;
		sc_start(20, SC_MS);		
		cout << endl << "Simulation s'est terminee a " << sc_time_stamp() << " ns" << endl;
	}
	// Fin du programme
	return 0;
}
