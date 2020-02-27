//! \file
#ifndef Y_INFORMATION_ECHO_INCLUDED
#define Y_INFORMATION_ECHO_INCLUDED 1

#include "y/information/modulation.hpp"

namespace upsylon {

    namespace Information {

        //! base class for simple echoing
        class EchoModulation : public Modulation
        {
        public:
            static const char   CLID[];                       //!< class identifier
            virtual const char *name() const         throw(); //!< CLID
            explicit            EchoModulation()    throw(); //!< setup
            virtual            ~EchoModulation()    throw(); //!< cleanup
            virtual void        reset()              throw(); //!< do nothing
            virtual uint8_t     fetch(const uint8_t) throw(); //!< do nothing
            virtual Modulation *clone() const;                //!< clone with status
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(EchoModulation);
        };

    }

}

#endif

