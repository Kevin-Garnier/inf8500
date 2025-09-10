///////////////////////////////////////////////////////////////////////////////
//
//	Wrapper.h
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <systemc.h>
#include "LMBIF.h"
#include "InterfaceRead.h"
#include "InterfaceWrite.h"
#include "DataRAM.h"

///////////////////////////////////////////////////////////////////////////////
//
//	Class Wrapper
//
///////////////////////////////////////////////////////////////////////////////
class Wrapper : public sc_module, public InterfaceRead, public InterfaceWrite
{
	public:
		// Ports
		sc_port <DataRAM>	dataPortInRAM;
		sc_port <DataRAM>	dataPortOutRAM;
		
		// Constructor
		Wrapper( sc_module_name zName );

		// Destructor
		~Wrapper();

	private:
	
	// Read - InterfaceRead
	virtual unsigned int Read(unsigned int uiAddr);
	// Write - InterfaceWrite
	virtual void Write(unsigned int addr, unsigned int data);
};

