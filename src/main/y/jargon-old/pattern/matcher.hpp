
//! \file

#ifndef Y_JARGON_PATTERN_MATCHER_INCLUDED
#define Y_JARGON_PATTERN_MATCHER_INCLUDED 1

#include "y/jargon/pattern.hpp"

namespace upsylon {

    namespace Jargon {

        class Dictionary; //!< forward declcaration

        //! helper to match motif in strings
        class Matcher : public Motif, public Token
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            Matcher(const string &, const Dictionary * =NULL);                  //!< setup by compiling regular expression
            Matcher(const char   *, const Dictionary * =NULL);                  //!< setup by compiling regular expression
            virtual ~Matcher() throw();               //!< cleanup
            Matcher(const Matcher &) throw();         //!< shared copy with empty token!
            

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            void ignoreCase(); //!< one way ignore case

            //! exact match
            template <typename CONTENT>
            const Token *matchesExactly(const CONTENT &content)
            {
                release();
                return (**this).exactlyMatchesData(*this,content) ? this : 0;
            }

            //! partial match
            template <typename CONTENT>
            const Token *matchesSomehow(const CONTENT &content)
            {
                release();
                return (**this).somehowMatchesData(*this,content) ? this : 0;
            }


        private:
            Y_DISABLE_ASSIGN(Matcher);
        };


    }

}

#endif

