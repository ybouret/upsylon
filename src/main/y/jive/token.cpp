
#include "y/jive/token.hpp"

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

    }

}
