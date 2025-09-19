///////////////////////////////////////////////////////////////////////////////
//
//	RAM.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include "RAM.h"
#include "Settings.h"

///////////////////////////////////////////////////////////////////////////////
//
//	Constructeur
//
///////////////////////////////////////////////////////////////////////////////
RAM::RAM( sc_module_name name, const char *fileName, unsigned long RAMSize, bool verbose )
: sc_module(name), m_RAMSize(RAMSize), m_verbose(verbose)
{
	// Variable
	FILE * file;
	size_t result;

	// Initialisation
	m_ptrRAM = NULL;

	// Allocation de la m�moire
	m_ptrRAM = malloc( m_RAMSize );

	// On valide si la m�moire est allou�e
	if ( m_ptrRAM == NULL )
	{
		cout << "Erreur lors de l'allocation de " << m_RAMSize << " octets" << endl;

		// Fin de la simulation
		m_bError = true;
	}

	// On initialise la m�moire
	memset(m_ptrRAM, 0, m_RAMSize);

#if option_file == 1
	// Ouverture du fichier
	file = fopen( fileName, "rb" );

	// Validation
	if (file == 0)
	{
		cout << "Erreur lors de l'ouverture du fichier d'entr�" << endl;

		// Fin de la simulation
		m_bError = true;
	}
	else
	{
		// On va chercher la taille du fichier
		fseek( file , 0 , SEEK_END );
		m_size = ftell(file);
		rewind(file);

		// On s'assure que le fichier n'est pas trop volumineux
		if ( m_size > m_RAMSize )
		{
			cout << "Fichier d'entr�e trop volumineux pour la RAM" << endl;

			// Fin de la simulation
			m_bError = true;
		}
		else
		{
			// On remplie le contenu de la m�moire avec le fichier d'entr�
			result = fread( m_ptrRAM, 1, m_size, file );

			dump_matrices_ABC32(m_ptrRAM, /*dim=*/4);     // affiche A, B, C en grille

			// On valide que tout s'est bien d�roul�
			if (result != m_size)
			{
				cout << "Erreur lors de l'�criture du fichier d'entr�e en m�moire" << endl;

				// Fin de la simulation
				m_bError = true;
			}
		}
	}

}

#else	/* pour la création de plus grosse matrices */

constexpr std::size_t DIM = CDIM;
int A[DIM][DIM], B[DIM][DIM];

// Remplissage
for (std::size_t i=0;i<DIM;i++)
  for (std::size_t j=0;j<DIM;j++) {
    A[i][j] = int(i + j);
    B[i][j] = int(i * 2 + j);
  }

// Écriture dans la RAM (A || B || C=0)
copy_AB_to_ram_loop(m_ptrRAM, m_RAMSize, A, B, /*zeroC=*/true);
//dump_matrices_ABC32(m_ptrRAM, DIM);

}

#endif


///////////////////////////////////////////////////////////////////////////////
//
//	Read
//
///////////////////////////////////////////////////////////////////////////////
unsigned int RAM::Read(unsigned int addr)
{
	// Variable
	unsigned int uiData = 0;

	if ( addr >= m_RAMSize )
	{
		cout << "Lecture hors de la plage de la RAM" << endl;

		// Fin de la simulation
		sc_stop();
	}
	else
	{
		// On avertie l'usager
		if ( ( addr >= m_size ) && m_verbose )
			cout << "Lecture de la m�moire hors de la plage du fichier d'entr�e" << endl;

		// On copie le contenu de la m�moire
//		memcpy( &uiData, (void*)((long)(m_ptrRAM) + addr), 4); // on peut aussi utilise uintptr_t
		memcpy( &uiData, (void*)((uintptr_t)(m_ptrRAM) + addr), 4);
	}

	// Retourne le contenu de la m�moire
	return uiData;
}

void RAM::dump_matrices_ABC32(const void* p, unsigned dim) {
    const uint8_t* base = static_cast<const uint8_t*>(p);
    const size_t N = dim * dim;

    auto getw = [&](size_t k) {
        uint32_t v; std::memcpy(&v, base + 4*k, 4); return v;
    };

    auto dump_one = [&](const char* name, size_t baseW) {
        std::cout << name << ":\n";
        for (unsigned i = 0; i < dim; ++i) {
            for (unsigned j = 0; j < dim; ++j) {
                uint32_t v = getw(baseW + i*dim + j);
                std::cout << std::setw(10) << std::dec << v << " ";
            }
            std::cout << "\n";
        }
    };

    dump_one("A", 0);
    dump_one("B", N);
    dump_one("C", 2*N);
}

///////////////////////////////////////////////////////////////////////////////
//
//	Destructeur
//
///////////////////////////////////////////////////////////////////////////////
RAM::~RAM()
{
	// On vide la m�moire
	if ( m_ptrRAM != NULL )
		free( m_ptrRAM );
}
