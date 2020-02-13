
#include "y/information/filter/shannon-fano.hpp"

namespace upsylon {

    namespace information {


        ShannonFano:: Context:: ~Context() throw()
        {}


        ShannonFano:: Context:: Context(const Mode m) :
        Alphabet(m,0)
        {
        }

    }

}

