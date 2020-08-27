
//! \file

#ifndef Y_JIVE_SOURCE_INCLUDED
#define Y_JIVE_SOURCE_INCLUDED 1

#include "y/jive/module.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! smart, buffering pointer for a module
        //
        //______________________________________________________________________
        class Source : public CountedObject
        {
        public:
            explicit Source(Module *) throw();
            virtual ~Source() throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Source);
            class Item : public Object
            {
                
            };

            Module::Handle  handle;
            Char::List      cache;
        };

    }

}

#endif
