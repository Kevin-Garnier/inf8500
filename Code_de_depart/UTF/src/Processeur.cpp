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

void Processeur::writeMat(int baseAddr, int mat[dim][dim]) {
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            int idx = i * dim + j;
            out->Write((baseAddr + idx) * 4, mat[i][j]);
        }
    }
}

void Processeur::readMat(int baseAddr, int mat[dim][dim]) {
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            int idx = i * dim + j;
            mat[i][j] = in->Read((baseAddr + idx) * 4);
        }
    }
}

void Processeur::process(void) {
    const unsigned N = dim * dim;
    while (true) {
        //generation matrices
        int mat_a[dim][dim];
        int mat_b[dim][dim];
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                mat_a[i][j] = i * dim + j;
                mat_b[i][j] = i * dim + j;
            }
        }
        writeMat(0, mat_a);
        writeMat(N, mat_b);

        // Démarrer le calcul en envoyant le signal 'go'

        go_o.write(true);
        wait(SC_ZERO_TIME); // Assurer la propagation du signal
        go_o.write(false);
        if (!done_i.read()) wait(done_i.posedge_event());


        int mat_out[dim][dim];
        readMat(2 * N, mat_out);

        int expected[dim][dim] = {0};
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                for (int k = 0; k < dim; k++) {
                    expected[i][j] += mat_a[i][k] * mat_b[k][j];
                }
            }
        }
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
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
