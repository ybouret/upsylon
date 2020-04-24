#include "y/jargon/token/convert.hpp"
#include "y/code/utils.hpp"

namespace upsylon {
    
    
    namespace Jargon {

        void TokenConvert:: To( string &target, const Token &token )
        {
            target.clear();
            for(const Char *ch=token.head;ch;ch=ch->next)
            {
                target << char( ch->code );
            }
        }
        
        void TokenConvert:: ToEncoded( string &target, const Token &token )
        {
            target.clear();
            for(const Char *ch=token.head;ch;ch=ch->next)
            {
                target << cchars::encoded[ ch->code ];
            }
        }
    }
    
}

