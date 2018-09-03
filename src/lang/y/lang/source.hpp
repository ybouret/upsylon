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


        class Source : public Object
        {
        public:
            explicit Source(const Input &inp);
            virtual ~Source() throw();

            Char *get(); //!< get next Char
            void  unget( Char *ch ) throw(); //!< unget a Char
            void  unget( Token &t ) throw(); //!< unget a token
            void  unget_copy( const Token &t ); //!< unget a copy of a token
            void  prefetch(size_t n);

        private:
            Input       input;
            Char::List  iobuf;
            Char::Pool &cache;
        };
    }
}

#endif

