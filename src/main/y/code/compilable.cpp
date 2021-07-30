#include "y/code/compilable.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    compilable:: compilable() throw() : compiled(false)
    {
    }
    
    compilable:: ~compilable() throw()
    {}
    
    
    void compilable:: compile()
    {
        assert(!compiled);
        on_compile();
        aliasing::_(compiled) = true;
    }


}
