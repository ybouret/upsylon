#include "y/chemical/indexed.hpp"

namespace upsylon
{
    namespace Chemical
    {
        
        Indexed:: ~Indexed() throw() { aliasing::_(indx) = 0; }
        Indexed::  Indexed() throw() : indx(0) {}
        
    }
}
