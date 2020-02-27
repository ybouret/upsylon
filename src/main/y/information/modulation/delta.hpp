//! \file
#ifndef Y_INFORMATION_DELTA_INCLUDED
#define Y_INFORMATION_DELTA_INCLUDED 1

#include "y/information/modulation.hpp"

namespace upsylon {

    namespace Information {

        //! delta modulation base class
        class DeltaModulation : public Modulation
        {
        public:
            virtual        ~DeltaModulation() throw(); //!< cleanup
            virtual void    reset()            throw(); //!< set last=0

        protected:
            explicit DeltaModulation() throw(); //!< setup
            uint8_t  last; //!< last encoded/decoded bytes

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DeltaModulation);
        };

        //! delta encoder
        class DeltaEncoder : public DeltaModulation
        {
        public:
            static const char   CLID[];                       //!< class identifier
            virtual const char *name() const         throw(); //!< CLID
            explicit            DeltaEncoder()       throw(); //!< initialize
            virtual            ~DeltaEncoder()       throw(); //!< destructor
            virtual uint8_t     fetch(const uint8_t) throw(); //!< encode a byte
            virtual Modulation *clone()                const; //!< a new delta with same last

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DeltaEncoder);
        };

        //! delta decoder method
        class DeltaDecoder : public DeltaModulation
        {
        public:
            static const char   CLID[];                       //!< class identifier
            virtual const char *name() const         throw(); //!< CLID
            explicit            DeltaDecoder()       throw(); //!< initialize
            virtual            ~DeltaDecoder()       throw(); //!< destructor
            virtual uint8_t     fetch(const uint8_t) throw(); //!< decode a byte
            virtual Modulation *clone()                const; //!< a new delta with same last

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DeltaDecoder);
        };

    }
}

#endif

