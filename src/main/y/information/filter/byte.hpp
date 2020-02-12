//! \file
#ifndef Y_INFORMATION_B_FILTER_INCLUDED
#define Y_INFORMATION_B_FILTER_INCLUDED 1

#include "y/information/filter/queue.hpp"
#include "y/information/shaker.hpp"

namespace upsylon {

    namespace information {

        //! implement the istream part
        class bytewise_filter : public filterQ
        {
        public:
            explicit bytewise_filter(const shaker::pointer &) throw();
            explicit bytewise_filter(shaker *);
            virtual ~bytewise_filter() throw();

            virtual void write( char C );
            virtual void flush();
            virtual void reset() throw();

        private:
            shaker::pointer transform;
            Y_DISABLE_COPY_AND_ASSIGN(bytewise_filter);
        };

    }

}

#endif
