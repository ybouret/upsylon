#include "y/lang/syntax/joker.hpp"


namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Joker::~Joker() throw() {}

            Joker:: Joker(const uint32_t i, const string &n, const Rule &r) :
            Rule(i,n),
            jk(r)
            {
            }

        }
    }
}

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Optional:: Optional(const string &n, const Rule &r) :
            Joker(UUID,n,r)
            {

            }

            Optional:: ~Optional() throw() {}

            const char * Optional:: typeName() const throw()
            {
                return "Optional";
            }

            bool Optional:: isHollow() const throw()
            {
                return true;
            }

            Y_LANG_SYNTAX_ACCEPT_START(Optional)
            {
                assert(NULL==tree||tree->internal);
                if( jk.accept(source, lexer, tree) )
                {
                    return true;
                }
                else
                {
                    return true;
                }
            }

            
        }
    }
}

#include "y/ptr/auto.hpp"
namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            ZeroOrMore:: ~ZeroOrMore() throw()
            {
            }

            ZeroOrMore:: ZeroOrMore(const string &n, const Rule &r) :
            Joker(UUID,n,r)
            {
            }

            bool ZeroOrMore:: isHollow() const throw()
            {
                return true;
            }

            const char *ZeroOrMore:: typeName() const throw()
            {
                return "ZeroOrMore";
            }

            Y_LANG_SYNTAX_ACCEPT_START(ZeroOrMore)
            {
                assert(NULL==tree||tree->internal);

                Node          *subTree = Node::Create(*this);
                auto_ptr<Node> guard(subTree);
                while(jk.accept(source,lexer,subTree))
                    ;

                if(tree)
                {
                    assert(tree->internal);
                    assert(subTree->internal);
                    tree->children().merge_back( subTree->children() );
                    return true;
                }
                else
                {
                    tree = subTree;
                    guard.dismiss();
                    return true;
                }
            }

        }
    }
}

