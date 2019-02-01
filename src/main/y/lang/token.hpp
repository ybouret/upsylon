//! \file
#ifndef Y_LANG_TOKEN_INCLUDED
#define Y_LANG_TOKEN_INCLUDED 1

#include "y/lang/char.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace Lang
    {


        //! full fledge token
        class Token : public Object, public Char::List
        {
        public:
            //! constructor
            inline Token() throw() : Object(), Char::List() {}

            //! desctructor
            inline virtual ~Token() throw()
            {
            }

            //! direct copy
            inline Token(const Token &other) : Object(), Char::List(other){}

            //! assign
            inline Token & operator=( const Token &other )
            {
                Token tmp(other);
                this->swap_with(tmp);
                return *this;
            }

            //! output
            inline friend std::ostream & operator<<( std::ostream &os, const Token &t )
            {
                for(const Char *ch=t.head;ch;ch=ch->next)
                {
                    os << char(ch->code);
                }
                return os;
            }

            //! convert to string with skipped and trimmed possibility
            string to_string(const size_t nskip=0, const size_t ntrim=0) const;

            //! convert to printable string
            string to_print()  const;

            //! write content as GraphViz
            void   viz( ios::ostream &fp ) const;


        };
    }
}

#endif

