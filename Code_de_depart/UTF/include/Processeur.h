
#include <systemc.h>
#include <stdio.h>
#include "Settings.h"
#include "Wrapper.h"
#include "InterfaceRead.h"
#include "InterfaceWrite.h"


class Processeur : public sc_module {
    public:
    sc_in<bool> done_i;
    sc_out<bool> go_o;
	sc_port<InterfaceRead>	in;
	sc_port<InterfaceWrite> out;

    Processeur(sc_module_name zName, int iterations = 1);
    ~Processeur();
    void process(void);


    private:
    int iterations;
    inline static constexpr int dim = CDIM;
    SC_HAS_PROCESS(Processeur);
    void writeMat(int baseAddr, int *mat);
    void readMat(int baseAddr, int *mat);
};
