

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
            virtual ~filterXQ() throw();  //!< cleanup

        protected:
            explicit filterXQ() throw(); //!< setup
            void     compile();          //!< io -> list, may leave some bits
            void     zfinish();          //!< io.zpad() -> list, no more bits
            void     free_all() throw(); //!< io.free(), this->free

            qbits    io;                 //!< queue of bits

        private:
            Y_DISABLE_COPY_AND_ASSIGN(filterXQ);
        };

    }

}

#endif
