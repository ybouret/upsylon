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
                derived = &jk;
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

            const char * Optional:: graphVizShape() const throw()
            {
                return "diamond";
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
                Y_LANG_SYNTAX_VERBOSE("|_" <<  typeName() << " '" << jk.name << "' : '" << name << "'" << std::endl);

                size_t count = 0;
                while(jk.accept(source,lexer,subTree))
                {
                    Y_LANG_SYNTAX_VERBOSE("|_+'" << jk.name << "'" << std::endl);
                    ++count;
                }

                if(tree)
                {
                    assert(tree->internal);
                    assert(subTree->internal);
                    tree->children().merge_back( subTree->children() );
                    Y_LANG_SYNTAX_VERBOSE("|_accepted " << typeName() << " '" << name << "' fusion@count=" << count << std::endl);
                    return true; // subTree will be deleteed
                }
                else
                {
                    tree = subTree;
                    guard.dismiss();
                    Y_LANG_SYNTAX_VERBOSE("|_accepted " << typeName() << " '" << name << "' replace@count=" << count << std::endl);
                    return true; // subTree becomes the main tree
                }
            }

            const char * ZeroOrMore:: graphVizShape() const throw()
            {
                return "circle";
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
                Node          *subTree = Node::Create(*this);
                auto_ptr<Node> guard(subTree);

                Y_LANG_SYNTAX_VERBOSE("|_" <<  typeName() << " '" << jk.name << "' : '" << name << "'" << std::endl);
                size_t count = 0;
                while(jk.accept(source,lexer,subTree))
                {
                    ++count;
                    Y_LANG_SYNTAX_VERBOSE("|_[count=" << count << "] '" << jk.name << "'" << std::endl);
                }


                if(count<=0)
                {
                    assert(0==subTree->children().size);
                    Y_LANG_SYNTAX_VERBOSE("|_rejected " << typeName() << " '" << name << "'@count=" << count << std::endl);
                    return false; // empty subTree is deleted
                }
                else
                {
                    if(tree)
                    {
                        assert(tree->internal);
                        assert(subTree->internal);
                        tree->children().merge_back( subTree->children() );
                        Y_LANG_SYNTAX_VERBOSE("|_accepted " << typeName() << " '" << name << "' fusion@count=" << count << std::endl);
                        return true; // subTree will be deleteed
                    }
                    else
                    {
                        tree = subTree;
                        guard.dismiss();
                        Y_LANG_SYNTAX_VERBOSE("|_accepted " << typeName() << " '" << name << "' replace@count=" << count << std::endl);
                        return true; // subTree becomes the main tree
                    }
                }
            }

            const char * OneOrMore:: graphVizShape() const throw()
            {
                return "octagon";
            }

        }
    }
}

