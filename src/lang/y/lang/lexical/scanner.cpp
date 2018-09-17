
#include "y/lang/lexical/scanner.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
#define Y_LANG_SCANNER_CTOR(ID) \
label(ID),                      \
rules(),                        \
probed(0),                      \
userDict(0)

            Scanner:: Scanner( const string &id ) :
            Y_LANG_SCANNER_CTOR(new string(id))
            {
            }


            Scanner:: Scanner( const char *id ) :
            Y_LANG_SCANNER_CTOR(new string(id))
            {
            }

            Scanner:: Scanner( const Origin &id ) throw() :
            Y_LANG_SCANNER_CTOR(id)
            {
            }
            
            const string & Scanner:: key() const throw()
            {
                return *label;
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

            const Rule * Scanner:: getRuleByLabel(const string &id) const throw()
            {
                for(const Rule *r = rules.head; r; r=r->next )
                {
                    if(id==*(r->label))
                    {
                        return r;
                    }
                }
                return 0;
            }

            string Scanner::CallLabel( const string &target )
            {
                return "=>" + target;
            }


        }
    }
}
