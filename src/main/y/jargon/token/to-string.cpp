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
        
        string Token:: toString() const
        {
            string ans(size,as_capacity,false);
            TokenConvert::To(ans,*this);
            return ans;
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

