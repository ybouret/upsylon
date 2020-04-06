#include "y/jargon/pattern/dictionary.hpp"
#include "y/jargon/pattern/all.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/ios/ocstream.hpp"
#include <cstring>

using namespace upsylon;
using namespace Jargon;

static inline void fmtesc( char *ch, const char C)
{
    ch[0] = ch[1] = '\\';
    ch[2] = C;
    ch[3] = 0;
}
Y_UTEST(jargon_types)
{
    if(false)
    {
        const size_t lmax = 5;
        ios::ocstream fp("encoded.hxx");
        char enc[256][8] = { {0} };
        memset( &enc[0][0],0,sizeof(enc));
        for(unsigned i=0;i<256;++i)
        {
            char *ch = enc[i];
            ch[0] = '\\';
            ch[1] = '\\';
            ch[2] = 'x';
            memcpy( ch+3, hexadecimal::lowercase[i], 2 );
            
            if( i<32 )
            {
                
                switch(i)
                {
                    case '\n': fmtesc(ch,'n'); break;
                    case '\r': fmtesc(ch,'r'); break;
                    case '\t': fmtesc(ch,'t'); break;
                    default:
                        break;
                        
                }
            }
            else
            {
               if(i<127)
               {
                   ch[0] = i; ch[1] = 0;
                   switch(i)
                   {
                       case '\\':
                       case '"':
                       case '\'':
                           ch[0] = '\\';
                           ch[1] = char(i);
                           ch[2] = 0;
                           break;
                       default: break;
                   }
               }
               else
               {
                   // default
               }
            }
            const size_t l = strlen(ch);
            fp(" \"%s\"", ch);
            
            if(i<255) {
                fp << ',';
                fp.repeat(lmax-l,' ');
            }
            if( ((1+i)%16) == 0)
            {
                fp << '\n';
            }
        }
        fp << '\n';
    }
    
    std::cerr << "I/O" << std::endl;
    Y_UTEST_SIZEOF(Context);
    Y_UTEST_SIZEOF(Char);
    Y_UTEST_SIZEOF(Token);
    Y_UTEST_SIZEOF(Source);
    Y_UTEST_SIZEOF(Module);
    Y_UTEST_SIZEOF(Char::Pool);
    std::cerr << std::endl;
    
    Y_UTEST_SIZEOF(Pattern);
    std::cerr << std::endl;

    std::cerr << "Basic" << std::endl;
    Y_UTEST_SIZEOF(Basic);
    Y_UTEST_SIZEOF(Any1);
    Y_UTEST_SIZEOF(Single);
    Y_UTEST_SIZEOF(Range);
    Y_UTEST_SIZEOF(Excluded);
    std::cerr << std::endl;

    std::cerr << "Logic" << std::endl;
    Y_UTEST_SIZEOF(Logical);
    Y_UTEST_SIZEOF(AND);
    Y_UTEST_SIZEOF(OR);
    Y_UTEST_SIZEOF(NONE);
    std::cerr << std::endl;
    
    std::cerr << "Joker" << std::endl;
    Y_UTEST_SIZEOF(Joker);
    Y_UTEST_SIZEOF(Optional);
    Y_UTEST_SIZEOF(Repeating);
    Y_UTEST_SIZEOF(Counting);
    std::cerr << std::endl;
    
}
Y_UTEST_DONE()

