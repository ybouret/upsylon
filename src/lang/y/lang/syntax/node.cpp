#include "y/lang/syntax/node.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Node:: List:: ~List() throw()
            {

            }

            Node:: List:: List() throw() : Object(), ListType()
            {
            }


            Node:: ~Node() throw()
            {
                assert(impl);
                if(terminal)
                {
                    delete static_cast<Lexeme *>(impl);
                }
                else
                {
                    while( children.size )
                    {
                        delete children.pop_back();
                    }
                    delete static_cast<List *>(impl);
                }
                impl = 0;
            }


            Node:: Node( const Rule &r, Lexeme *lx ) throw() :
            Object(),
            next(0), prev(0),
            terminal(true), internal(false),
            impl(lx),
            lexeme(   *static_cast<Lexeme *>(impl) ),
            children( *static_cast<List   *>(impl) ),
            rule(r)
            {
                assert(lx);
            }

            Node:: Node( const Rule &r ) throw() :
            Object(),
            next(0), prev(0),
            terminal(false), internal(true),
            impl( new List() ),
            lexeme(   *static_cast<Lexeme *>(impl) ),
            children( *static_cast<List   *>(impl) ),
            rule(r)
            {
            }

        }
    }
}

#include "y/lang/syntax/rule.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/graphviz.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            void Node:: viz( ios::ostream &fp ) const
            {
                fp.viz(this);
                if(terminal)
                {

                }
                else
                {
                    for(const Node *sub = children.head;sub;sub=sub->next)
                    {
                        sub->viz(fp);
                    }
                    unsigned i = 0;
                    for(const Node *sub = children.head;sub;sub=sub->next)
                    {
                        ++i;
                        fp.viz(this); fp << " -> "; fp.viz(sub); fp("[label=\"%u\"];\n", i);
                    }
                }
            }

            void Node:: GraphViz( const string &fn, bool keepFile) const
            {
                {
                    ios::ocstream fp(fn);
                    fp << "digraph G {\n";
                    viz(fp);
                    fp << "}\n";
                }
                ios::GraphViz::Render(fn,keepFile);

            }


        }
    }
}

