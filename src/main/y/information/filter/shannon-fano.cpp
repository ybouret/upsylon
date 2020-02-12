
#include "y/information/filter/shannon-fano.hpp"

namespace upsylon {

    namespace information {

        ShannonFano:: ~ShannonFano() throw()
        {
        }

        ShannonFano:: ShannonFano(const Mode m) :
        Alphabet(m,0)
        {
        }

    }

}

