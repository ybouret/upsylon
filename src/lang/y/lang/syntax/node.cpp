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


            static inline
            void * __hard_copy( const Node &other )
            {
                if(other.terminal)
                {
                    return new Lexeme( other.lexeme );
                }
                else
                {
                    auto_ptr<Node::List> L = new Node::List();
                    try
                    {
                        for(const Node *node = other.children.head;node;node=node->next)
                        {
                            L->push_back( new Node(*node) );
                        }
                    }
                    catch(...)
                    {
                        while(L->size) delete L->pop_back();
                        throw;
                    }
                    return L.yield();
                }
            }

            Node:: Node(const Node &other) :
            Object(),
            next(0),prev(0),
            terminal(other.terminal),
            internal(other.internal),
            impl( __hard_copy(other) ),
            lexeme(   *static_cast<Lexeme *>(impl) ),
            children( *static_cast<List   *>(impl) ),
            rule(other.rule)
            {

            }



            Lexeme * Node:: YieldLexeme(Node *node) throw()
            {
                assert(node);
                Lexeme *lx = & (node->lexeme);
                object::release1(node);
                return lx;
            }

            void Node:: BackTo( Lexical::Translator &lexer, Node *node ) throw()
            {
                assert(node);
                if(node->internal)
                {
                    while(node->children.size)
                    {
                        BackTo(lexer,node->children.pop_back());
                    }
                    delete node;
                }
                else
                {
                    lexer.unget( YieldLexeme(node) );
                }
            }

        }
    }
}

#include "y/lang/syntax/rule.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/graphviz.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            void Node:: viz( ios::ostream &fp ) const
            {
                fp.viz(this);
                const string RuleID = string_convert::to_printable(rule.name);
                if(terminal)
                {
                    const string LexID = lexeme.to_print();
                    if(LexID.size()>0)
                    {
                        fp("[label=\"%s='%s'\",shape=box,style=bold];\n",*RuleID,*LexID);
                    }
                    else
                    {
                        fp("[label=\"%s\",shape=box,style=rounded];\n",*RuleID);
                    }
                }
                else
                {
                    fp("[label=\"%s\",shape=house];\n",*RuleID);
                    for(const Node *sub = children.head;sub;sub=sub->next)
                    {
                        sub->viz(fp);
                    }
                    unsigned i = 0;
                    for(const Node *sub = children.head;sub;sub=sub->next)
                    {
                        ++i;
                        fp.viz(this); fp << " -> "; fp.viz(sub);
                        if(children.size>1)
                        {
                            fp("[label=\"%u\"];\n", i);
                        }
                        else
                        {
                            fp <<";\n";
                        }
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

#include "y/string/io.hpp"
namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            void Node:: save( ios::ostream &fp ) const
            {
                //! creator
                string_io::save_binary(fp,rule.name);
                if(terminal)
                {
                    fp.write(0);
                    fp.emit<uint32_t>( lexeme.size );
                    for(const Char *ch=lexeme.head;ch;ch=ch->next)
                    {
                        fp.write(ch->code);
                    }
                }
                else
                {
                    fp.write(1);
                    fp.emit<uint32_t>( children.size );
                    for(const Node *ch=children.head;ch;ch=ch->next)
                    {
                        ch->save(fp);
                    }
                }
            }
        }
    }
}

