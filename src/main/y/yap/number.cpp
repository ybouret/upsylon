#include "y/yap/number.hpp"
#include "y/code/round.hpp"

namespace upsylon {

    namespace yap {

        number:: ~number() throw()
        {
        }

        number:: number() throw()
        {
        }

        sign_type number:: sign_of(const itype i) throw()
        {
            if(i<0)
            {
                return __negative;
            }
            else if(0<i)
            {
                return __positive;
            }
            else
                return __zero;
        }

        number::utype number::  iabs_of(const itype i) throw()
        {
            return (i<0) ? utype(-i) : utype(i);
        }



    }

}

