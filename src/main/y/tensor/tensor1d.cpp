
#include "y/tensor/tensor1d.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace core
    {

        

        tensor1d:: ~tensor1d() throw()
        {
        }
        
        tensor1d:: tensor1d(const size_t c) :
        tensor()
        {
            if(c<=0) throw exception("tensor1d #cols=0");
            record(c);
        }
        
    }
}

