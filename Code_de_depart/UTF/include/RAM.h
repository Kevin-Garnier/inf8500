///////////////////////////////////////////////////////////////////////////////
//
//	RAM.h
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <systemc.h>
#include "LMBIF.h"

#include <iostream>
#include <iomanip>   // <-- NECESSAIRE pour std::setw, std::setfill, std::hex, std::dec
#include <cstdint>
#include <cstddef>
#include <cstring>

#define	option_file	0

// Variable externe
extern bool m_bError;

///////////////////////////////////////////////////////////////////////////////
//
//	Classe RAM
//
///////////////////////////////////////////////////////////////////////////////
class RAM : public sc_module, public LMBIF
{
	public: 

		// Constructeur
		RAM( sc_module_name name, const char *fileName, unsigned long RAMSize = 0, bool verbose = false );
		
		// Destructeur
		~RAM();

		// Méthode
		virtual unsigned int Read(unsigned int addr);
		virtual void Write(unsigned int addr, unsigned int data)	= 0;

	private:	

		// Variable 
		
	protected:
	
		// Variable 
		void* m_ptrRAM;
		unsigned long m_size;
		unsigned long m_RAMSize;
		bool m_verbose;
		void dump_matrices_ABC32(const void* p, unsigned dim);
		template<std::size_t DIM>
		bool copy_AB_to_ram_loop(void* ptrRAM, std::size_t ram_size_bytes,
                         const int (&A)[DIM][DIM],
                         const int (&B)[DIM][DIM],
                         bool zeroC = false);
};

template<std::size_t DIM>
inline bool RAM::copy_AB_to_ram_loop(void* ptrRAM, std::size_t ram_size_bytes,
                                     const int (&A)[DIM][DIM],
                                     const int (&B)[DIM][DIM],
                                     bool zeroC)
{
    static_assert(sizeof(int)==4, "int doit faire 32 bits");
    const std::size_t N = DIM*DIM;
    const std::size_t need = (zeroC?3:2)*N*sizeof(uint32_t);
    if (!ptrRAM || ram_size_bytes < need) return false;

    auto* w = reinterpret_cast<uint32_t*>(ptrRAM);
    for (std::size_t i=0;i<DIM;++i) {
        const std::size_t row = i*DIM;
        for (std::size_t j=0;j<DIM;++j) {
            const std::size_t idx = row + j;
            w[idx]      = static_cast<uint32_t>(A[i][j]);     // A
            w[N + idx]  = static_cast<uint32_t>(B[i][j]);     // B
        }
    }
    if (zeroC) std::memset(w + 2*N, 0, N*sizeof(uint32_t));
    return true;
}
