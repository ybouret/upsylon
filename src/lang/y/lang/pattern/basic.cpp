#include "y/lang/pattern/basic.hpp"

namespace upsylon
{
    namespace Lang
    {

        bool Match1:: match( Token &tkn, Source &src ) const
        {
            assert(0==tkn.size);
            Char *ch = src.get();
            if(!ch) return false;
            if( accept_byte(ch->code) )
            {
                tkn.push_back(ch);
                return true;
            }
            else
            {
                src.unget(ch);
                return false;
            }
        }
    }
}
