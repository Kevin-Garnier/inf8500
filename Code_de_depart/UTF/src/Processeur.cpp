#include "Processeur.h"
#include <cstdlib> // Pour rand()
#include <ctime>   // Pour time()

#define DIM CDIM
#define N (DIM*DIM)


// Constructeur
Processeur::Processeur( sc_module_name zName, int iterations)
: sc_module(zName), iterations(iterations)
{
    // Method (one shot) with no sensitive list.
    SC_THREAD(process);
    go_o.initialize(false);
}

// Destructeur
Processeur::~Processeur()
{
}

void Processeur::writeMat(int baseAddr, int *mat) {
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            int idx = i * dim + j;
            out->Write((baseAddr + idx) * 4, mat[idx]);
        }
    }
}

void Processeur::readMat(int baseAddr, int *mat) {
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            int idx = i * dim + j;
            mat[idx] = in->Read((baseAddr + idx) * 4);
        }
    }
}

void Processeur::process(void) {
    std::cout << "Processeur started, dim=" << dim << ", iterations=" << iterations << std::endl;
    while (iterations-- > 0) {
        printf("Iteration restante %d\n", iterations + 1);
        //generation matrices
        int* mat_a = new int[dim * dim];
        int* mat_b = new int[dim * dim];
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                mat_a[i * dim + j] = i * dim + j;
                mat_b[i * dim + j] = i * dim + j;
            }
        }
        writeMat(0, mat_a);
        writeMat(N, mat_b);

        // Démarrer le calcul en envoyant le signal 'go'

        std::cout << "Matrices A et B ecrites dans la RAM" << std::endl;
        go_o.write(true);
        wait(SC_ZERO_TIME); // Assurer la propagation du signal
        go_o.write(false);
        if (!done_i.read()) wait(done_i.posedge_event());


        int *mat_out = new int[dim * dim];
        readMat(2 * N, mat_out);

        std::cout << "Matrice C lue dans la RAM" << std::endl;

        int *expected = new int[dim * dim]();
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                for (int k = 0; k < dim; k++) {
                    expected[i * dim + j] += mat_a[i * dim + k] * mat_b[k * dim + j];
                }
            }
        }
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                if (mat_out[i * dim + j] != expected[i * dim + j]) {
                    std::cout << "Error at (" << i << "," << j << "): expected " << expected[i * dim + j] << ", got " << mat_out[i * dim + j] << std::endl;
                    sc_stop();
                }
            }
        }
        std::cout << "Matrix multiplication successful." << std::endl;
        delete[] mat_a;
        delete[] mat_b;
        delete[] mat_out;
        delete[] expected;
    }
   // sc_stop();
}
