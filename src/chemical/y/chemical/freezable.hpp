//! \file

#ifndef Y_CHEMICAL_FREEZABLE_INCLUDED
#define Y_CHEMICAL_FREEZABLE_INCLUDED 1

#include "y/chemical/object.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! information about a (unique) species
        //
        //______________________________________________________________________
        class Freezable : public Object
        {
        public:
            //__________________________________________________________________
            //
            //! internal latch
            //__________________________________________________________________
            class Latch
            {
            public:
                explicit Latch(Freezable &) throw(); //!< freeze
                virtual ~Latch()            throw(); //!< release
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Latch);
                Freezable &host;
            };
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Freezable() throw(); //!< setup
            virtual ~Freezable() throw(); //!< cleanup
            const bool frozen;            //!< status
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Freezable);
        };
        
        
    }
}

#endif

