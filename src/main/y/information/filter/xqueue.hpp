

//! \file
#ifndef Y_INFORMATION_XQ_FILTER_INCLUDED
#define Y_INFORMATION_XQ_FILTER_INCLUDED 1

#include "y/information/filter/queue.hpp"
#include "y/information/qbits.hpp"

namespace upsylon {

    namespace information {

        //! implement the istream part
        class filterXQ : public filterQ
        {
        public:
            explicit filterXQ() throw();
            virtual ~filterXQ() throw();

            qbits io;
            void  compile();
            void  zfinish();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(filterXQ);
        };

    }

}

#endif
