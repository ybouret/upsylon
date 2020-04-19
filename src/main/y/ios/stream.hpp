//! \file
#ifndef Y_IOS_STREAM_INCLUDED
#define Y_IOS_STREAM_INCLUDED 1

#include "y/ptr/counted.hpp"

namespace upsylon
{
    
    namespace ios
    {

        //! base class for streams
        class stream : public virtual counted_object
        {
        public:
            //! destructor
            virtual ~stream() throw();

        protected:
            //! constructor
            explicit stream() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(stream);
        };
    }
}

#endif

