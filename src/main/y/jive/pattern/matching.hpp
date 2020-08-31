//! \file

#ifndef Y_JIVE_PATTERN_MATCHING_INCLUDED
#define Y_JIVE_PATTERN_MATCHING_INCLUDED 1

#include "y/jive/regexp.hpp"

namespace upsylon
{
    namespace Jive
    {

        class Matching : public Motif
        {
        public:
            virtual ~Matching() throw();
            template <typename REGEXP> inline
            Matching(const REGEXP &rx, const Dictionary *dict =0 ) :
            Motif( RegExp(rx,dict) )
            {
            }

            Matching(const Matching &other) throw();


        private:
            Y_DISABLE_ASSIGN(Matching);
        };

    }
}

#endif
