
//! \file

#ifndef Y_JARGON_PATTERN_MATCHER_INCLUDED
#define Y_JARGON_PATTERN_MATCHER_INCLUDED 1

#include "y/jargon/pattern.hpp"

namespace upsylon {

    namespace Jargon {

        //! helper to match motif in strings
        class Matcher : public Motif, public Token
        {
        public:
            Matcher(const string &);         //!< setup by compiling regular expression
            Matcher(const char   *);         //!< setup by compiling regular expression
            virtual ~Matcher() throw();      //!< cleanup
            Matcher(const Matcher &) throw(); //!< shared copy with empty token!
            
            const Token *exact_match(const string &); //!< this->matches_exactly(*this,str)
            const Token *first_match(const string &); //!< this->matches_partly(*this,str)
            
            
        private:
            Y_DISABLE_ASSIGN(Matcher);
        };


    }

}

#endif

