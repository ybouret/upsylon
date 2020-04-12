#include "y/jargon/token/convert.hpp"
#include "y/exception.hpp"
//#include "y/mpl/natural.hpp"
#include "y/code/utils.hpp"

namespace upsylon {
    
    namespace Jargon {

        void TokenConvert:: To(double &target, const Token &token)
        {
            static const char fn[] = "TokenConvert::To(double)";
            
            target    = 0;
            Char *ch  = token.head;
            bool  neg = false;
            if(!ch) throw exception("%s empty token",fn);
            
            double ip = 0;
            switch(ch->code)
            {
                case '-': neg=true; // FALLTHRU
                case '+': ch=ch->next; break;
                default: break;
            }
            
            size_t ip_size = 0;
            for(;ch;ch=ch->next,++ip_size)
            {
                ip *= 10.0;
                switch(ch->code)
                {
                    case '0': break;
                    case '1': ++ip; break;
                    case '2': ip+=2; break;
                    case '3': ip+=3; break;
                    case '4': ip+=4; break;
                    case '5': ip+=5; break;
                    case '6': ip+=6; break;
                    case '7': ip+=7; break;
                    case '8': ip+=8; break;
                    case '9': ip+=9; break;
                    case '.': goto FP;
                    default:
                        throw exception("%s invalid char '%s' in integer part", fn, cchars::encoded[ch->code]);
                }
            }
            
            // integer part only
            target = neg ? -ip : ip;
            return;
            
            FP:
            target = neg ? -ip : ip;
            return;
        }

    }
    
}

