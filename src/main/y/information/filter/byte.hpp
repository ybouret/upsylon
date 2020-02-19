//! \file
#ifndef Y_INFORMATION_B_FILTER_INCLUDED
#define Y_INFORMATION_B_FILTER_INCLUDED 1

#include "y/information/filter/queue.hpp"
#include "y/information/shift.hpp"

namespace upsylon {

    namespace information {

        //! implement the istream part
        class bytewise_filter : public filterQ
        {
        public:
            explicit bytewise_filter(const shift::pointer &) throw();  //!< setup with a shared shaker
            explicit bytewise_filter(shift *);                         //!< setup with a new shaker, echo_modulation otherwise
            virtual ~bytewise_filter() throw();                        //!< cleanup

            virtual void write( char C ); //!< store transformed char
            virtual void flush();         //!< do nothing
            virtual void reset() throw(); //!< reset transform and free 

        private:
            shift::pointer transform;
            Y_DISABLE_COPY_AND_ASSIGN(bytewise_filter);
        };

    }

}

#endif
