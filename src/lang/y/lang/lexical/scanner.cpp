
#include "y/lang/lexical/scanner.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
#define Y_LANG_SCANNER_CTOR() \
label(new string(id)),        \
rules(),                      \
probed(0),                    \
userDict(0)

            Scanner:: Scanner( const string &id ) throw():
            Y_LANG_SCANNER_CTOR()
            {
            }


            Scanner:: Scanner( const char *id ) throw():
            Y_LANG_SCANNER_CTOR()
            {
            }

            Scanner:: ~Scanner() throw() {}

            void Scanner:: checkLabel(const Origin &ruleLabel) const
            {
                for(const Rule *r = rules.head; r; r=r->next )
                {
                    if( *ruleLabel == *(r->label) )
                    {
                        throw exception("[%s] multiple rule '%s'",**label,**ruleLabel);
                    }
                }
            }

        }

    }
}
