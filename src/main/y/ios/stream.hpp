//! \file
#ifndef Y_IOS_STREAM_INCLUDED
#define Y_IOS_STREAM_INCLUDED 1

#include "y/ptr/counted.hpp"
#include "y/os/endian.hpp"

namespace upsylon
{
    
    //! information about comms
    struct comms
    {
        enum manner
        {
            constant, //!< assuming host and peer know the size
            variable  //!< the size must be exchanged
        };
        
        enum medium
        {
            homogeneous = 0x01, //!< same binary layouts
            distributed = 0x02  //!< binary layouts may change
        };
        
        static const char *manner_text(const manner) throw();
        static const char *medium_text(const medium) throw();
        
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

