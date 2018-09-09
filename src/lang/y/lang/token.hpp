//! \file
#ifndef Y_LANG_TOKEN_INCLUDED
#define Y_LANG_TOKEN_INCLUDED 1

#include "y/lang/char.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! default fields init
#define Y_LANG_TOKEN_BASE() Object(), Char::List()

        //! default fields and memory init
#define Y_LANG_TOKEN_CTOR() Y_LANG_TOKEN_BASE(), cache( Manager::instance() )

        //! full fledge token
        class Token : public Object, public Char::List
        {
        public:
            //! constructor
            inline Token() : Y_LANG_TOKEN_CTOR() {}

            //! desctructor
            inline virtual ~Token() throw()
            {
                while(size) cache.store( pop_back() );
            }

            //! direct copy
            inline Token(const Token &other) : Y_LANG_TOKEN_BASE(), cache( other.cache )
            {
                for(const Char *ch = other.head;ch;ch=ch->next)
                {
                    push_back( cache.make(ch->code,*ch) );
                }
            }

#if 0
            //! from string
            inline Token( const string &s ) : Y_LANG_TOKEN_CTOR()
            {
                const size_t n = s.size();
                for(size_t i=0;i<n;++i)
                {
                    push_back( cache.make( uint8_t(s[i]) ) );
                }
            }

            //! from text
            inline Token(const char *s) : Y_LANG_TOKEN_CTOR()
            {
                const size_t n = length_of(s);
                for(size_t i=0;i<n;++i)
                {
                    push_back( cache.make( uint8_t(s[i]) ) );
                }
            }
#endif


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

            string to_string() const; //!< convert to string
            string to_print()  const; //!< convert to printable string
            void   viz( ios::ostream &fp ) const;

        private:
            Char::Pool &cache;

        };
    }
}

#endif

