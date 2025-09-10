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
		sc_event go_i, done_o;
	
	
		// Constructor
		Mult_Matrix( sc_module_name zName);
		
		// Destructor
		~Mult_Matrix();

		
	private:
	
		inline static constexpr int dim = 4;   

		unsigned int mat_a[dim][dim];
		unsigned int mat_b[dim][dim];
		unsigned int mat_out[dim][dim];
	
		// Process SystemC
		SC_HAS_PROCESS(Mult_Matrix);

		void thread(void);
		void compute(void);
	
	
};

