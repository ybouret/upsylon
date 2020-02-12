
#include "y/information/filter/xqueue.hpp"

namespace upsylon {

    namespace information {
        
        filterXQ:: filterXQ() throw() : filterQ(), io()
        {
        }

        filterXQ:: ~filterXQ() throw()
        {
        }

        void filterXQ:: compile()
        {
            while(io.size()>=8)
            {
                push_back( io.pop<char>() );
            }
        }

        void filterXQ:: zfinish()
        {
            io.zpad();
            compile();
            assert(0==io.size());
        }

    }

}


