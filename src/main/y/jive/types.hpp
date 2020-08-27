//! \file

#ifndef Y_JIVE_TYPES_INCLUDED
#define Y_JIVE_TYPES_INCLUDED 1

#include "y/ptr/arc.hpp"
#include "y/string.hpp"

namespace upsylon {

    namespace Jive
    {

        typedef arc_ptr<const string> Tag;     //!< reusable tag
        typedef object                Object;  //!< base class
        typedef counted               Counted; //!< alias

        class   CountedObject : public Object, public Counted
        {
        public:
            explicit CountedObject() throw();
            virtual ~CountedObject() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(CountedObject);
        };
    }

}

#endif
