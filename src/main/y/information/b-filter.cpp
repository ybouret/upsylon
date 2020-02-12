
#include "y/information/b-filter.hpp"
#include "y/information/echo.hpp"

namespace upsylon {

    namespace information {

        b_filter:: ~b_filter() throw()
        {
        }

        b_filter:: b_filter( const shaker::pointer &p ) throw() :
        transform(p)
        {
        }

        b_filter:: b_filter( shaker *p )  :
        transform(p?p:new echo_modulation())
        {

        }



    }

}

