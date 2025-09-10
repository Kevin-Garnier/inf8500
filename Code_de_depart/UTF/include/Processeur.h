
#include <systemc.h>
#include <stdio.h>
#include "Wrapper.h"
#include "InterfaceRead.h"
#include "InterfaceWrite.h"

class Processeur : public sc_module {
    public:
    sc_in<bool> done_i;
    sc_out<bool> go_o;
	sc_port<InterfaceRead>	in;
	sc_port<InterfaceWrite> out;

    Processeur(sc_module_name zName);
    ~Processeur();
    void process(void);


    private:
    SC_HAS_PROCESS(Processeur);
    void writeMat(int baseAddr, int mat[4][4]);
    void readMat(int baseAddr, int mat[4][4]);
};
