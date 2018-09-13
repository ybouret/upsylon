
#include "y/lang/syntax/joker.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            
            Joker:: ~Joker() throw()
            {
            }

            Joker:: Joker(const uint32_t t, const string &id, const Rule &jk ):
            Rule(t,id),
            rule(jk)
            {
            }
            

            Optional:: ~Optional() throw()
            {
            }

            Optional:: Optional( const string &id, const Rule &jk ) :
            Joker(UUID,id,jk)
            {
            }

            bool Optional:: accept( Y_LANG_SYNTAX_RULE_ARGS ) const
            {
                Node *node = NULL;
                if( rule.accept(node,lexer,source) )
                {
                    Grow(tree,node);
                }
                return true;
            }

        }

    }

}
