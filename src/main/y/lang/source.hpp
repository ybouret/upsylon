//! \file
#ifndef Y_LANG_SOURCE_INCLUDED
#define Y_LANG_SOURCE_INCLUDED 1

#include "y/lang/module.hpp"
#include "y/lang/token.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! convert Module into a source of tokens
        class Source : public Object, public ios::istream
        {
        public:
            explicit Source(Module *m);  //!< initialize
            virtual ~Source() throw();   //!< destructor

            Char          *get();                         //!< get next Char
            void           unget(Char  *) throw();        //!< unget a Char
            void           unget(Token &) throw();        //!< unget a token
            void           ungetCopy(const Token &);      //!< unget a copy of a token
            void           prefetch(size_t);              //!< try to prefetch some Chars
            void           skip();                        //!< iobuf.size>0, skip last unread char
            size_t         loaded() const throw();        //!< iobuf.size
            void           forward(size_t n) throw();     //!< n<=loaded
            bool           active();                      //!< try to ensure one next char in iobuf
            const Char *   peek();                        //!< address of first char in iobuf
            const Module * operator*() const throw();     //!< access the module address

            virtual bool query(char &); //!< ios::istream interface
            virtual void store(char);   //!< ios::istream interface
            
            
        private:
            Module::Pointer module;
            Char::List      iobuf;
        };

    }
}

#endif

