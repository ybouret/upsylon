#include "y/lang/token.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{
    namespace Lang
    {
        
        Token:: Token() throw() : Object(), Char::List() {}
        
        Token:: ~Token() throw()
        {
        }
        
        Token:: Token(const Token &other) :
        object(), Object(), Char::List(other)
        {}

        Token & Token::operator=( const Token &other )
        {
            Token tmp(other);
            this->swap_with(tmp);
            return *this;
        }
        
        string Token::toString(const size_t nskip, const size_t ntrim) const
        {
            const size_t ndrop = nskip+ntrim;
            if(ndrop>=size)
            {
                return string();
            }
            else
            {
                const size_t n = size-ndrop;
                string       ans(n,as_capacity,false);
                const Char  *ch = head;

                for(size_t i=0;i<nskip;++i)
                {
                    ch=ch->next;
                }

                for(size_t i=n;i>0;--i,ch=ch->next)
                {
                    ans << char(ch->code);
                }

                return ans;
            }
        }

        string Token:: toPrintable() const
        {
            string ans(2*size,as_capacity,false);
            for(const Char *ch=head;ch;ch=ch->next)
            {
                ans << cchars::printable[ch->code];
            }
            return ans;
        }

        string Token::toVisible() const
        {
            string ans(3*size,as_capacity,false);
            for(const Char *ch=head;ch;ch=ch->next)
            {
                ans << cchars::visible[ch->code];
            }
            return ans;
        }

        void Token::viz( ios::ostream &fp ) const
        {
            fp << '\'';
            for(const Char *ch=head;ch;ch=ch->next)
            {
                fp << cchars::printable[ch->code];
            }
            fp << '\'';
        }
        
        std::ostream & operator<<( std::ostream &os, const Token &t )
        {
            return t.write(os);
        }
    }
}
