
#include "y/jive/token.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"

namespace upsylon {

    namespace Jive
    {
        Token:: Token() throw() : CountedObject(), Char::List()
        {
        }

        Token:: ~Token() throw()
        {
        }


        Token:: Token( const Token &other ) : CountedObject(), Char::List(other)
        {
        }

        Token & Token:: operator=(const Token &other)
        {
            Token tmp(other);
            swap_with(tmp);
            return *this;
        }

        string Token:: toString(size_t nskip) const
        {
            if(nskip>=size)
            {
                return string();
            }
            else
            {
                const Char *ch=head; while(nskip--) ch=ch->next;

                string ans(size-nskip,as_capacity,false);
                for(;ch;ch=ch->next)
                {
                    ans += char(ch->code);
                }
                return ans;
            }
        }

        string Token:: translate(const char * table[]) const
        {
            string ans(4*size,as_capacity,false);
            for(const Char *ch=head;ch;ch=ch->next)
            {
                ans += table[ch->code];
            }
            return ans;
        }


        string Token:: toVisible() const
        {
            return translate(cchars::visible);
        }

        string Token:: toPrintable() const
        {
            return translate(cchars::printable);
        }

        string Token:: toEncoded() const
        {
            return translate(cchars::encoded);
        }



        exception & Token:: cat(exception &excp) const throw()
        {
            for(const Char *ch=head;ch;ch=ch->next)
            {
                excp << cchars::encoded[ch->code];
            }
            return excp;
        }

        Token & Token:: skip(size_t n) throw()
        {
            if(n>=size)
            {
                release();
            }
            else
            {
                while(n-- >0 )
                {
                    Char::Release(pop_front());
                }
            }
            return *this;
        }

        Token & Token:: trim(size_t n) throw()
        {
            if(n>=size)
            {
                release();
            }
            else
            {
                while(n-- >0 )
                {
                    Char::Release(pop_back());
                }
            }
            return *this;
        }

        Token & Token:: chop(size_t n) throw()
        {
            return trim(n).skip(n);
        }

    }

}
