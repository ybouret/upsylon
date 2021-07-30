#include "y/code/compilable.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    compilable:: compilable() throw() : compiled(false)
    {
    }
    
    compilable:: ~compilable() throw()
    {}
    
    void compilable:: restart() throw()
    {
        aliasing::_(compiled) = false;
    }
    
    void compilable:: compile()
    {
        on_compile();
        aliasing::_(compiled) = true;
    }


}
