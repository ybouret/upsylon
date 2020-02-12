
//! \file
#ifndef Y_INFORMATION_Q_FILTER_INCLUDED
#define Y_INFORMATION_Q_FILTER_INCLUDED 1

#include "y/information/filter.hpp"

namespace upsylon {

    namespace information {

        //! implement the istream part
        class filterQ : public filter, public list<char>
        {
        public:
            explicit filterQ() throw();
            virtual ~filterQ() throw();

            virtual bool query( char &C );
            virtual void store( char  C );
            virtual bool is_active() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(filterQ);
        };
        
    }

}

#endif
