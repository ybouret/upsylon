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
            const uint32_t uuid;
            Pattern       *next;
            Pattern       *prev;

            inline virtual ~Pattern() throw() {}

            ios::ostream & emitAddress( ios::ostream &os ) const;
            
        protected:
            inline Pattern(const uint32_t id)    throw() : uuid(id), next(0), prev(0) {}
            inline Pattern(const Pattern &other) throw() : uuid(other.uuid), next(0), prev(0) {}

        private:
            Y_DISABLE_ASSIGN(Pattern);
        };
    }
}

#endif

