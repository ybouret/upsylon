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
            inline Token(const Token &other) : object(), Object(), Char::List(other){}

            //! assign
            inline Token & operator=( const Token &other )
            {
                Token tmp(other);
                this->swap_with(tmp);
                return *this;
            }

            //! generic output of token
            template <typename OUTPUT> inline
            OUTPUT & write( OUTPUT &os ) const
            {
                for(const Char *ch=head;ch;ch=ch->next)
                {
                    os << char(ch->code);
                }
                return os;
            }
            
            //! output
            friend std::ostream & operator<<( std::ostream &os, const Token &t );

            //! convert to string with skipped and trimmed possibility
            string toString(const size_t nskip=0, const size_t ntrim=0) const;

            //! convert to printable string
            string toPrintable()  const;

            //! convert to visible string
            string toVisible()  const;

            //! write content as GraphViz
            void   viz( ios::ostream &fp ) const;


        };
    }
}

#endif

