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
            Internal(i,n,typeid(Joker)),
            jk(r)
            {
                if(jk.isHollow())
                {
                    throw exception("Lang.Syntax.Joker(hollow rule '%s')", *(r.name));
                }
                derived = (const Joker *)this;
            }

            void Joker:: graphVizEpilog(ios::ostream &fp) const
            {
                graphVizLink(fp, &jk, 0);
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
                assert(derived);
            }

            Optional:: ~Optional() throw() {}

            const char * Optional:: typeName() const throw()
            {
                return "Optional  ";
            }

            bool Optional:: isHollow() const throw()
            {
                return true;
            }

            Y_LANG_SYNTAX_ACCEPT_START(Optional)
            {
                assert(NULL==tree||tree->internal);
                Y_LANG_SYNTAX_VERBOSE(std::cerr <<"|_" <<  typeName() << " <" << jk.name << "> : <" << name << ">" << std::endl);

                if( jk.accept(source, lexer, tree) )
                {
                    assert(NULL!=tree);
                    Y_LANG_SYNTAX_VERBOSE(std::cerr <<"|_(+)<" << jk.name << ">" << std::endl);
                    return true;
                }
                else
                {
                    assert(NULL==tree||tree->internal);
                    Y_LANG_SYNTAX_VERBOSE(std::cerr <<"|_(-)<" << jk.name << ">" << std::endl);
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
                assert(derived);
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
                Y_LANG_SYNTAX_VERBOSE(std::cerr <<"|_" <<  typeName() << " <" << jk.name << "> : <" << name << ">" << std::endl);

                size_t count = 0;
                while(jk.accept(source,lexer,subTree))
                {
                    Y_LANG_SYNTAX_VERBOSE(std::cerr <<"|_+<" << jk.name << ">" << std::endl);
                    ++count;
                }

                if(tree)
                {
                    assert(tree->internal);
                    assert(subTree->internal);
                    tree->children().merge_back( subTree->children() );
                    Y_LANG_SYNTAX_VERBOSE(std::cerr <<"|_accepted " << typeName() << " <" << name << "> fusion@count=" << count << std::endl);
                    return true; // subTree will be deleteed
                }
                else
                {
                    tree = subTree;
                    guard.dismiss();
                    Y_LANG_SYNTAX_VERBOSE(std::cerr <<"|_accepted " << typeName() << " <" << name << "> replace@count=" << count << std::endl);
                    return true; // subTree becomes the main tree
                }
            }

            const char * ZeroOrMore:: graphVizShape() const throw()
            {
                return "oval";
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
                assert(derived);
            }

            bool OneOrMore:: isHollow() const throw()
            {
                assert(! jk.isHollow() );
                return false;
            }

            const char *OneOrMore:: typeName() const throw()
            {
                return "OneOrMore ";
            }

            Y_LANG_SYNTAX_ACCEPT_START(OneOrMore)
            {
                assert(NULL==tree||tree->internal);
                Node          *subTree = Node::Create(*this);
                auto_ptr<Node> guard(subTree);

                Y_LANG_SYNTAX_VERBOSE(std::cerr << "|_" <<  typeName() << " <" << jk.name << "> : <" << name << ">" << std::endl);
                size_t count = 0;
                while(jk.accept(source,lexer,subTree))
                {
                    ++count;
                    Y_LANG_SYNTAX_VERBOSE(std::cerr << "|_[count=" << count << "] <" << jk.name << ">" << std::endl);
                }


                if(count<=0)
                {
                    assert(0==subTree->children().size);
                    Y_LANG_SYNTAX_VERBOSE(std::cerr << "|_rejected " << typeName() << " <" << name << ">@count=" << count << std::endl);
                    return false; // empty subTree is deleted
                }
                else
                {
                    if(tree)
                    {
                        assert(tree->internal);
                        assert(subTree->internal);
                        tree->children().merge_back( subTree->children() );
                        Y_LANG_SYNTAX_VERBOSE(std::cerr << "|_accepted " << typeName() << " <" << name << "> fusion@count=" << count << std::endl);
                        return true; // subTree will be deleteed
                    }
                    else
                    {
                        tree = subTree;
                        guard.dismiss();
                        Y_LANG_SYNTAX_VERBOSE(std::cerr << "|_accepted " << typeName() << " <" << name << "> replace@count=" << count << std::endl);
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
