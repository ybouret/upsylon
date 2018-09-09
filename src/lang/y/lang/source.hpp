//! \file
#ifndef Y_LANG_SOURCE_INCLUDED
#define Y_LANG_SOURCE_INCLUDED 1

#include "y/lang/token.hpp"
#include "y/ios/istream.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace Lang
    {
        typedef arc_ptr<ios::istream> Input; //!< shared input

#if 0
        //! convert input stream into a source of tokens
        class Source : public Object
        {
        public:
            explicit Source(const Input &inp); //!< initialize
            virtual ~Source() throw();         //!< desctructor, excess token are poured in cached

            Char *get();                        //!< get next Char
            void  unget( Char *ch ) throw();    //!< unget a Char
            void  unget( Token &t ) throw();    //!< unget a token
            void  unget_copy( const Token &t ); //!< unget a copy of a token
            void  prefetch(size_t n);           //!< try to prefetch at most n Chars

        private:
            Input       input;
            Char::List  iobuf;
            Char::Pool &cache;
        };
#endif
        
    }
}

#endif

