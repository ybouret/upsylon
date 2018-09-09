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
        class Object : public object
        {
        public:
            explicit Object() throw(); //!< constructor
            virtual ~Object() throw(); //!< desctructor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Object);
        };


        //! to identify a source/module
        typedef arc_ptr<const string> Origin;
        
        //! shared input
        typedef arc_ptr<ios::istream> Input;

    }
}

#endif

