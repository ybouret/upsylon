
#include "y/tensor/loops.hpp"


namespace upsylon {

    tensor_loop:: tensor_loop( const core::tensor &t ) :
    tensor_loop_type( t.dimensions, t.lower(), t.upper() )
    {
    }

    tensor_loop:: ~tensor_loop() throw()
    {
    }

    tensor_loop:: tensor_loop( const tensor_loop &other) :
    collection(), tensor_loop_type(other)
    {
    }
    

}
