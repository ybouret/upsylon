

#include "y/chemical/types.hpp"

namespace upsylon
{
    namespace Chemical
    {
        bool Verbosity = true;
        
        Object:: Object() throw() : object(), counted()
        {
        }
        
        Object:: ~Object() throw() 
        {
        }
        
        Indexed:: ~Indexed() throw() { aliasing::_(indx) = 0; }
        Indexed::  Indexed() throw() : indx(0) {}
        
    }
}
