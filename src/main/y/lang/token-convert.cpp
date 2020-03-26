
#include "y/lang/token-convert.hpp"
#include "y/exceptions.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{
    namespace Lang
    {
        unit_t TokenConvert:: ToUnit(const Token &token, const char *context )
        {
            static const char fn[] = "TokenConvert::ToUnit";
            const char       *ctx  = context ? context : "integer";
            if(token.size<=0) throw exception("%s(empty token for %s)",fn,ctx);
            
            unit_t      ans = 0;
            const Char *ch  = token.head;
            bool        neg = false;
            if(ch->code=='-')
            {
                if(token.size<=1) throw exception("%s(empty token after '-' for %s)",fn,ctx);
                neg = true;
                ch  = ch->next;
            }
            assert(ch);
            for(;ch;ch=ch->next)
            {
                ans *= 10;
                const uint8_t x = ch->code;
                switch(x)
                {
                    case '0':           break;
                    case '1': ++ans;    break;
                    case '2': ans += 2; break;
                    case '3': ans += 3; break;
                    case '4': ans += 4; break;
                    case '5': ans += 5; break;
                    case '6': ans += 6; break;
                    case '7': ans += 7; break;
                    case '8': ans += 8; break;
                    case '9': ans += 9; break;

                    default: throw exception("%s(invalid char '%s' for %s)",fn,cchars::encoded[x],ctx);
                }
            }

            return neg ? -ans : ans;
        }

        size_t TokenConvert:: ToSize(const Token &token, const char *context )
        {
            static const char fn[] = "TokenConvert::ToSize";
            const char       *ctx  = context ? context : "unsigned";
            if(token.size<=0) throw exception("%s(empty token for %s)",fn,ctx);

            size_t      ans = 0;
            const Char *ch  = token.head;

            assert(ch);
            for(;ch;ch=ch->next)
            {
                ans *= 10;
                const uint8_t x = ch->code;
                switch(x)
                {
                    case '0':           break;
                    case '1': ++ans;    break;
                    case '2': ans += 2; break;
                    case '3': ans += 3; break;
                    case '4': ans += 4; break;
                    case '5': ans += 5; break;
                    case '6': ans += 6; break;
                    case '7': ans += 7; break;
                    case '8': ans += 8; break;
                    case '9': ans += 9; break;

                    default: throw exception("%s(invalid char '%s' for %s)",fn,cchars::encoded[x],ctx);
                }
            }
            return ans;
        }


    }

}
