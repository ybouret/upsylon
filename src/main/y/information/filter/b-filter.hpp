//! \file
#ifndef Y_INFORMATION_B_FILTER_INCLUDED
#define Y_INFORMATION_B_FILTER_INCLUDED 1

#include "y/information/q-filter.hpp"
#include "y/information/shaker.hpp"

namespace upsylon {

    namespace information {

        //! implement the istream part
        class b_filter : public q_filter
        {
        public:
            explicit b_filter(const shaker::pointer &) throw();
            explicit b_filter(shaker *);
            virtual ~b_filter() throw();
            
        private:
            shaker::pointer transform;
            Y_DISABLE_COPY_AND_ASSIGN(b_filter);
        };

    }

}

#endif
