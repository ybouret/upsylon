#include "y/mkl/io/data-set.hpp"

namespace upsylon {
    
    namespace mkl {
        
        data_set_:: ~data_set_() throw()
        {

        }

        data_set_:: data_set_() throw() : labels()
        {
        }

        const char data_set_:: fn[] = "data_set: ";

        bool data_set_:: separator(const int C) throw()
        {
            return ' '==C || '\t' == C;
        }

    }
}

