//! \file

#ifndef Y_ALCHEMY_FREEZE_INCLUDED
#define Y_ALCHEMY_FREEZE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        //______________________________________________________________________
        //
        //
        //! small class to freeze built objects
        //
        //______________________________________________________________________
        class Freezable
        {
        public:
            explicit   Freezable() throw();        //!< setup freezed=false
            virtual   ~Freezable() throw();        //!< cleanup
            void       freeze(const char *when=0); //!< freeze
            void       permit() throw();           //!< permit anew
            const bool frozen;                     //!< flag

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Freezable);
        };

        //______________________________________________________________________
        //
        //
        //! freezable manager
        //
        //______________________________________________________________________
        class Freezer
        {
        public:
            explicit Freezer(Freezable &, const char *msg=0); //!< setup/freeze
            virtual ~Freezer() throw();                       //!< cleanup/permit

        private:
            Freezable &host;
            Y_DISABLE_COPY_AND_ASSIGN(Freezer);
        };
    }
}

#endif