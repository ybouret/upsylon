//! \file
#ifndef Y_LANG_PATTERN_INCLUDED
#define Y_LANG_PATTERN_INCLUDED 1

#include "y/lang/token.hpp"
#include "y/ios/ocstream.hpp"

namespace upsylon
{
    namespace Lang
    {
        class Pattern : public Object
        {
        public:
            const uint32_t uuid; //!< unique ID for the class
            Pattern       *next; //!< for List
            Pattern       *prev; //!< for List

            virtual ~Pattern() throw(); //!< destructor

            ios::ostream & emitAddress( ios::ostream &os ) const;

            virtual Pattern *clone() const = 0;
            

            typedef core::list_of_cloneable<Pattern> List;

        protected:
            inline Pattern(const uint32_t id)    throw() : uuid(id), next(0), prev(0) {}
            inline Pattern(const Pattern &other) throw() : uuid(other.uuid), next(0), prev(0) {}

        private:
            Y_DISABLE_ASSIGN(Pattern);
        };
    }
}

#endif

