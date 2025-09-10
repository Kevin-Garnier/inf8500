///////////////////////////////////////////////////////////////////////////////
//
//	DataRAM.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include "DataRAM.h"

///////////////////////////////////////////////////////////////////////////////
//
//	Constructeur
//
///////////////////////////////////////////////////////////////////////////////
DataRAM::DataRAM( sc_module_name name, const char *dataFileName, unsigned long RAMSize, bool verbose )
:RAM(name, dataFileName, RAMSize, verbose)
{
}


///////////////////////////////////////////////////////////////////////////////
//
//	Destructeur
//
///////////////////////////////////////////////////////////////////////////////
DataRAM::~DataRAM()
{
}


///////////////////////////////////////////////////////////////////////////////
//
//	Read
//
///////////////////////////////////////////////////////////////////////////////
unsigned int DataRAM::Read(unsigned int addr)
{
	// On appele la classe parent
	return RAM::Read(addr);
}


///////////////////////////////////////////////////////////////////////////////
//
//	Write
//
///////////////////////////////////////////////////////////////////////////////
void DataRAM::Write(unsigned int addr, unsigned int data)
{
	if ( addr >= m_RAMSize )
	{
		cout << "�criture hors de la plage de la RAM" << endl;

		// Fin de la simulation
		sc_stop();
	}
	else
	{
		// On copie le contenu de la m�moire
//		memcpy( (void*)((long)(m_ptrRAM) + addr), &data, 4);  // on peut aussi utilise uintptr_t
		memcpy( (void*)((uintptr_t)(m_ptrRAM) + addr), &data, 4);
	}
}


///////////////////////////////////////////////////////////////////////////////
//
//	end_of_simulation
//
///////////////////////////////////////////////////////////////////////////////
void DataRAM::end_of_simulation()
{
	// Variable
	FILE * file;
	unsigned long size;

	// Ouverture du fichier de sortie
	file = fopen( this->name() , "wb" );

	// Validation de l'ouverture du fichier
	if (file == 0)
		cout << "Erreur lors de l'ouverture du fichier de sortie" << endl;
	else
	{
		// �criture
		size = fwrite(m_ptrRAM , 1 , m_RAMSize , file );

		cout << "Apres execution du calcul de multiplication " << endl;
		dump_matrices_ABC32(m_ptrRAM, /*dim=*/4);     // affiche A, B, C en grille

		if ( m_RAMSize != size )
			cout << "La RAM n'a pas �t� toute sauvegard�e dans le fichier de sortie" << endl;

		// Fermeture du fichier
		fclose(file);
	}
}
