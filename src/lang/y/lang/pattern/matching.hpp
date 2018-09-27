//! \file
#ifndef Y_LANG_PATTERN_MATCHING
#define Y_LANG_PATTERN_MATCHING 1

#include "y/lang/pattern/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {
        class Matching : public Token
        {
        public:
            virtual ~Matching() throw();
            Matching( Pattern *p ) throw();
            Matching( const string &rx, const Dictionary *dict=NULL);
            Matching( const char   *rx, const Dictionary *dict=NULL);

            bool exactly( const string &s );
            bool partly(const string &s);

            

        private:
            const arc_ptr<const Pattern> motif;
            Y_DISABLE_COPY_AND_ASSIGN(Matching);
        };
    }
}

#endif

