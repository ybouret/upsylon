
#include "y/jive/token.hpp"
//#include "y/code/utils.hpp"
#include "y/exception.hpp"

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

        string Token:: toString() const
        {
            string ans(size,as_capacity,false);
            for(const Char *ch=head;ch;ch=ch->next)
            {
                ans += char(ch->code);
            }
            return ans;
        }

        string Token:: toVisible() const
        {
            string ans(4*size,as_capacity,false);
            for(const Char *ch=head;ch;ch=ch->next)
            {
                ans += cchars::visible[ch->code];
            }
            return ans;
        }

        string Token:: toPrintable() const
        {
            string ans(4*size,as_capacity,false);
            for(const Char *ch=head;ch;ch=ch->next)
            {
                ans += cchars::printable[ch->code];
            }
            return ans;
        }

        exception & Token:: cat(exception &excp) const
        {
            const string s = toVisible();
            excp.cat("'%s'",*s);
            return excp;

        }

    }

}
