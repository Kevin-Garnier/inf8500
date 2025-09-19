///////////////////////////////////////////////////////////////////////////////
//
//	Mult_Matrix.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include "Mult_Matrix.h"
include "Settings.h"


///////////////////////////////////////////////////////////////////////////////
//
//	Constructeur
//
///////////////////////////////////////////////////////////////////////////////
Mult_Matrix::Mult_Matrix( sc_module_name zName)
: sc_module(zName)
{
	// Method (one shot) with no sensitive list.
	SC_THREAD(thread);
	done_o.initialize(false);
}


///////////////////////////////////////////////////////////////////////////////
//
//	Destructeur
//
///////////////////////////////////////////////////////////////////////////////
Mult_Matrix::~Mult_Matrix()
{
}


void Mult_Matrix::thread(void)
{

    const unsigned N = dim * dim;

	while (true) {
		if (!go_i.read()) wait(go_i.posedge_event());

		// Attente du signal 'go'
		// Lecture: matrice A (offset 0)
		for (int i = 0; i < dim; ++i) {
			for (int j = 0; j < dim; ++j) {
				int idx = i * dim + j;
//				cout << endl << "Matrice A i, j et index: " << i << " " << j << " " << 0 * N + idx  << endl;
				mat_a[i][j] = in->Read((0 * N + idx) * 4);
//				cout << endl << "et valeur lue: " << mat_a[i][j] << endl;
			}
		}

		// Lecture: matrice B (offset N)
		for (int i = 0; i < dim; ++i) {
			for (int j = 0; j < dim; ++j) {
				int idx = i * dim + j;
//				cout << endl << "Matrice B i, j et index: " << i << " " << j << " " << 1 * N + idx  << endl;
				mat_b[i][j] = in->Read((1 * N + idx) * 4);
//				cout << endl << "et valeur lue: " << mat_b[i][j] << endl;
			}
		}

		// Calcul
		this->compute();

		// Écriture: matrice C (offset 2N)
		for (int i = 0; i < dim; ++i) {
			for (int j = 0; j < dim; ++j) {
				int idx = i * dim + j;
//				cout << endl << "Matrice C i, j et index: " << i << " " << j << " " << 2 * N + idx  << endl;
//				cout << endl << "et valeur calculee : " << mat_out[i][j] << endl;
				out->Write((2 * N + idx) * 4, mat_out[i][j]);
			}
		}

		// Envoi du signal 'done'
		done_o.write(true);
		wait(SC_ZERO_TIME); // Assurer la propagation du signal
		done_o.write(false);
		// sc_stop();
		// wait();
	}

}

void Mult_Matrix::compute(void)
{

	for (int ia = 0; ia < dim; ++ia)
		for (int ib = 0; ib < dim; ++ib)
		{
			unsigned int sum = 0;
			for (int id = 0; id < dim; ++id)
				sum += mat_a[ia][id] * mat_b[id][ib];
			mat_out[ia][ib] = sum;
		};

		#ifdef D128DMA
		wait(D128DMA_EXEC_TIME, SC_NS);
		#endif
		#ifdef D128
		wait(D128_EXEC_TIME, SC_NS);
		#endif

		return;
}




