//! \file
#ifndef Y_LANG_TYPES_INCLUDED
#define Y_LANG_TYPES_INCLUDED 1

#include "y/string.hpp"
#include "y/ptr/arc.hpp"
#include "y/ios/istream.hpp"

namespace upsylon
{
    namespace Lang
    {
        
        //! Base Object
        class Object : public virtual object
        {
        public:
            explicit Object() throw(); //!< constructor
            virtual ~Object() throw(); //!< destructor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Object);
        };

        //! Base Reference Counted Object
        class CountedObject : public Object, public counted
        {
        public:
            explicit CountedObject() throw(); //!< constructor
            virtual ~CountedObject() throw(); //!< destructor
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(CountedObject);
        };

        //! to identify a shared class with the same tag
        typedef arc_ptr<const string> Tag;
        
        //! shared input
        typedef arc_ptr<ios::istream> Input;

    }
}

#endif

