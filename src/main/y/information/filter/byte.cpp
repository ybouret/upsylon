
#include "y/information/filter/byte.hpp"
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

        void b_filter:: write(char C)
        {
            push_back( transform->fetch(C) );
        }

        void b_filter:: flush()
        {
        }

        void b_filter:: reset() throw()
        {
            transform->reset();
        }

    }

}

