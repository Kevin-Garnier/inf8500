///////////////////////////////////////////////////////////////////////////////
//
//	Mult_Matrix.h
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <systemc.h>
#include <stdio.h>
#include "Wrapper.h"
#include "InterfaceRead.h"
#include "InterfaceWrite.h"
#include "Settings.h"


///////////////////////////////////////////////////////////////////////////////
//
//	Class Mult_Matrix
//
///////////////////////////////////////////////////////////////////////////////
class Mult_Matrix : public sc_module
{
	public:
		// Ports
		sc_port<InterfaceRead>	in;
		sc_port<InterfaceWrite> out;
		sc_in<bool> go_i;
		sc_out<bool> done_o;


		// Constructor
		Mult_Matrix( sc_module_name zName);

		// Destructor
		~Mult_Matrix();


	private:

		inline static constexpr int dim = CDIM;

		unsigned int mat_a[dim][dim];
		unsigned int mat_b[dim][dim];
		unsigned int mat_out[dim][dim];

		// Process SystemC
		SC_HAS_PROCESS(Mult_Matrix);

		void thread(void);
		void compute(void);


};

