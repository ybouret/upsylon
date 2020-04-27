
#include "y/jargon/pattern.hpp"
#include "y/ios/ostream.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"

namespace upsylon {
    
    namespace Jargon {

        void Pattern:: Express(ios::ostream &fp, const uint8_t code)
        {
            static const char extra[] =
            " !$%&,/;<=>`@";
            if(code<32||code>122) goto ESC;
            
            if((NULL!=strchr(extra,int(code)))  ||
               (code >= '0' && code <= '9') ||
               (code >= 'a' && code <= 'z') ||
               (code >= 'A' && code <= 'Z')
               )
            {
                fp << char(code);
                return;
            }
            
            
            
            
        ESC:
            fp << '\\' << 'x' << hexadecimal::lowercase[code];
            
        }
        
    }
    
}

