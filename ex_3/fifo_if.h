#ifndef __FIFO_IF
#define __FIFO_IF
// Interface declares functions which can be called from external in a hierarchical channel
// Hierachical channel implements the interface functions.

#include "systemc.h"

class fifo_if : public sc_interface {
    public:
        virtual bool write_fifo(unsigned char *data, unsigned int &count) = 0;
        virtual bool read_fifo(unsigned char *data, unsigned int &count) = 0;
};

#endif // __FIFO_IF

// An interface is a class derived from sc_interface, it declares the functions that can be 
// called via the interface, 'pure virtual' and 'implementation has to be contained in the class'
// derived from the interface, i.e. the hierarchical channel.
// interface is an abstract class, no instances are allowed.