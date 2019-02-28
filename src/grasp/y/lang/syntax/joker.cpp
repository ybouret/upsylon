#include "y/lang/syntax/joker.hpp"
#include "y/exception.hpp"

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
                if(jk.isHollow())
                {
                    throw exception("Lang.Syntax.Joker(hollow rule '%s')", *(r.name));
                }
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
                derived = static_cast<ZeroOrMore *>(this);
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
                    return true; // subTree will be deleteed
                }
                else
                {
                    tree = subTree;
                    guard.dismiss();
                    return true; // subTree becomes the main tree
                }
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
            OneOrMore:: ~OneOrMore() throw()
            {
            }

            OneOrMore:: OneOrMore(const string &n, const Rule &r) :
            Joker(UUID,n,r)
            {
                derived = static_cast<OneOrMore *>(this);
            }

            bool OneOrMore:: isHollow() const throw()
            {
                assert(! jk.isHollow() );
                return false;
            }

            const char *OneOrMore:: typeName() const throw()
            {
                return "OneOrMore";
            }

            Y_LANG_SYNTAX_ACCEPT_START(OneOrMore)
            {
                assert(NULL==tree||tree->internal);

                assert(NULL==tree||tree->internal);

                Node          *subTree = Node::Create(*this);
                auto_ptr<Node> guard(subTree);
                size_t count = 0;
                while(jk.accept(source,lexer,subTree))
                    ++count;

                if(count<=0)
                {
                    assert(0==tree->children().size);
                    return false; // empty subTree is deleted
                }

                if(tree)
                {
                    assert(tree->internal);
                    assert(subTree->internal);
                    tree->children().merge_back( subTree->children() );
                    return true; // subTree will be deleteed
                }
                else
                {
                    tree = subTree;
                    guard.dismiss();
                    return true; // subTree becomes the main tree
                }
            }

        }
    }
}

