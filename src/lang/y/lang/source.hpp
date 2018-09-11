//! \file
#ifndef Y_LANG_SOURCE_INCLUDED
#define Y_LANG_SOURCE_INCLUDED 1

#include "y/lang/module.hpp"
#include "y/lang/token.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            class Scanner;
        }

        //! convert Module into a source of tokens
        class Source : public Object
        {
        public:
            explicit Source(Module *m);  //!< initialize
            virtual ~Source() throw();   //!< desctructor, excess token are poured in cached

            Char       *get();                         //!< get next Char
            void        unget( Char *ch ) throw();     //!< unget a Char
            void        unget( Token &t ) throw();     //!< unget a token
            void        ungetCopy( const Token &t );   //!< unget a copy of a token
            void        prefetch(size_t n);            //!< try to prefetch at most n Chars
            void        skip();                        //!< iobuf.size>0, skip last unread char
            size_t      loaded() const throw();        //!< iobuf.size
            void        forward(size_t n) throw();     //!< n<=loaded
            bool        active();                      //!< try to ensure one next char in iobuf
            const Char *peek();                        //!< copy of first char in iobuf
            void        collect( Token &tkn ) throw(); //!< put in cache

            const Module * operator*() const throw();
            
        private:
            Module::Pointer module;
            Char::List      iobuf;
            Char::Pool     &cache;
        };

    }
}

#endif

