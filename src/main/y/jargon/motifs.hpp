
//! \file

#ifndef Y_JARGON_MOTIFS_INCLUDED
#define Y_JARGON_MOTIFS_INCLUDED 1

#include "y/jargon/pattern.hpp"

namespace upsylon {

    namespace Jargon {

        //! helper to match motifs in strings
        class Motifs : public Motif, public Token
        {
        public:
            Motifs(const string &);         //!< setup by compiling regular expression
            Motifs(const char   *);         //!< setup by compiling regular expression
            virtual ~Motifs() throw();      //!< cleanup
            Motifs(const Motifs &) throw(); //!< shared copy with empty token!
            
            const Token *exact_match(const string &); //!< this->matches_exactly(*this,str)
            const Token *first_match(const string &); //!< this->matches_partly(*this,str)
            
            
        private:
            Y_DISABLE_ASSIGN(Motifs);
        };


    }

}

#endif

