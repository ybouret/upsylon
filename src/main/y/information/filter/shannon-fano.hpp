
//! \file
#ifndef Y_INFORMATION_SHANNON_FANO_INCLUDED
#define Y_INFORMATION_SHANNON_FANO_INCLUDED 1

#include "y/information/qbits.hpp"
#include "y/information/filter/alphabet.hpp"

namespace upsylon {

    namespace information {


        class ShannonFano : public Alphabet
        {
        public:
            static const size_t Nodes = 2*Codes+1;

            explicit ShannonFano(const Mode m);
            virtual ~ShannonFano() throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ShannonFano);
        };

    }


}

#endif
