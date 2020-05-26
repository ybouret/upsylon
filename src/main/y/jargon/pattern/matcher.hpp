
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
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            Matcher(const string &);                  //!< setup by compiling regular expression
            Matcher(const char   *);                  //!< setup by compiling regular expression
            virtual ~Matcher() throw();               //!< cleanup
            Matcher(const Matcher &) throw();         //!< shared copy with empty token!
            

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------

            //! exact match
            template <typename CONTENT>
            const Token *matches_exactly(const CONTENT &content)
            {
                release();
                return (**this).exactly_matches_data(*this,content) ? this : 0;
            }

            //! partial match
            template <typename CONTENT>
            const Token *matches_somehow(const CONTENT &content)
            {
                release();
                return (**this).somehow_matches_data(*this,content) ? this : 0;
            }


        private:
            Y_DISABLE_ASSIGN(Matcher);
        };


    }

}

#endif

