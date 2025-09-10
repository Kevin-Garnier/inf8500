#include "Processeur.h"
#include <cstdlib> // Pour rand()
#include <ctime>   // Pour time()


// Constructeur
Processeur::Processeur( sc_module_name zName)
: sc_module(zName)
{
    // Method (one shot) with no sensitive list.
    SC_THREAD(process);
    go_o.initialize(false);
}

// Destructeur
Processeur::~Processeur()
{
}

#define DIM 4
#define N (DIM*DIM)

void Processeur::writeMat(int baseAddr, int mat[DIM][DIM]) {
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            int idx = i * DIM + j;
            out->Write((baseAddr + idx) * 4, mat[i][j]);
        }
    }
}

void Processeur::readMat(int baseAddr, int mat[DIM][DIM]) {
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            int idx = i * DIM + j;
            mat[i][j] = in->Read((baseAddr + idx) * 4);
        }
    }
}

void Processeur::process(void) {
    while (true) {
        //generation matrices
        int mat_a[DIM][DIM];
        int mat_b[DIM][DIM];
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                mat_a[i][j] = rand() % 10; // Valeurs aléatoires entre 0 et 9
                mat_b[i][j] = rand() % 10; // Valeurs aléatoires entre 0 et 9
            }
        }
        writeMat(0, mat_a);
        writeMat(N, mat_b);

        // Démarrer le calcul en envoyant le signal 'go'

        go_o.write(true);
        wait(SC_ZERO_TIME); // Assurer la propagation du signal
        go_o.write(false);
        if (!done_i.read()) wait(done_i.posedge_event());


        int mat_out[DIM][DIM];
        readMat(2 * N, mat_out);

        int expected[DIM][DIM] = {0};
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                for (int k = 0; k < DIM; k++) {
                    expected[i][j] += mat_a[i][k] * mat_b[k][j];
                }
            }
        }
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                if (mat_out[i][j] != expected[i][j]) {
                    std::cout << "Error at (" << i << "," << j << "): expected " << expected[i][j] << ", got " << mat_out[i][j] << std::endl;
                    sc_stop();
                }
            }
        }
        std::cout << "Matrix multiplication successful." << std::endl;

        sc_stop();
    }
}
