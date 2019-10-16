//! \file
#ifndef Y_IOS_STREAM_INCLUDED
#define Y_IOS_STREAM_INCLUDED 1

#include "y/ptr/counted.hpp"
#include "y/os/endian.hpp"

namespace upsylon
{
    //! communication mode
    enum comm_mode
    {
        comm_constant_size, //!< assuming host and peer know the size
        comm_variable_size  //!< the size must be exchanged
    };

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

