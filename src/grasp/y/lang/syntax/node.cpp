#include "y/lang/syntax/rule.hpp"
#include "y/exception.hpp"
#include "y/ios/graphviz.hpp"
#include "y/string/convert.hpp"
#include "y/string/io.hpp"
#include "y/ios/osstream.hpp"
#include "y/ios/imstream.hpp"

#include "y/codec/base64.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Node:: ~Node() throw()
            {
            }

            Node:: Node(const Rule &r,
                        const bool term) throw() :
            Object(), Base(), rule(r), terminal(term), internal(!terminal)
            {
            }

            Node:: Node(const Node &other) throw() :
            Object(),
            Base(),
            rule(other.rule),
            terminal(other.terminal),
            internal(other.internal)
            {
            }


            Node * Node::Create(const Rule &r, Lexeme *l)
            {
                assert(l);
                try
                {
                    return new TerminalNode(r,l);
                }
                catch(...)
                {
                    delete l;
                    throw;
                }
            }

            Node * Node::Create(const Rule &r)
            {
                return new InternalNode(r);
            }

            const Lexeme & Node:: lexeme() const throw()
            {
                const void *data = inner();
                assert(terminal);
                assert(data);
                return *static_cast<const Lexeme *>(data);
            }

            Lexeme & Node:: lexeme() throw()
            {
                void *data = (void*)inner();
                assert(terminal);
                assert(data);
                return *static_cast<Lexeme *>(data);
            }

            const Node::List & Node:: children() const throw()
            {
                const void *data = inner();
                assert(internal);
                assert(data);
                return *static_cast<const List *>(data);
            }

            Node::List & Node:: children() throw()
            {
                void *data = (void*)inner();
                assert(internal);
                assert(data);
                return *static_cast<List *>(data);
            }

            void  Node:: Grow( Node * &tree, Node *leaf ) throw()
            {
                assert(NULL!=leaf);
                if(NULL == tree )
                {
                    tree = leaf;
                }
                else
                {
                    assert(tree->internal);
                    tree->children().push_back(leaf);
                }
            }

            void   Node:: Unget( Node * &node, Lexer &lexer) throw()
            {
                assert(node);
                if(node->terminal)
                {
                }
                else
                {
                    Node *scan = node->children().tail;
                    while(scan)
                    {
                        Node *prev = scan->prev;
                        Unget(scan,lexer); assert(0==scan);
                        scan = prev;
                    }
                }
                delete node;
                node = 0;
            }

            void Node:: graphVizName( ios::ostream &fp) const
            {
                fp.viz(this);
            }

            
            void Node:: graphViz( const string &dotfile) const
            {
                {
                    ios::ocstream fp(dotfile);
                    fp << "digraph G {\n";
                    viz(fp);
                    fp << "}\n";
                }
                ios::GraphViz::Render(dotfile);
            }

            void Node:: graphViz( const char *dotfile) const
            {
                const string _(dotfile); graphViz(_);
            }

            void Node:: save( ios::ostream &fp ) const
            {
                string_io::save_binary(fp,rule.name);
                emit(fp);
            }

            string Node:: toBinary() const
            {
                string ans;
                {
                    ios::osstream fp(ans);
                    save(fp);
                }
                return ans;
            }

            string Node:: toBase64() const
            {
                const string bin = toBinary();
                ios::base64::encoder b64;
                string       ans = b64.to_string(bin);
                return ans;
            }

            void Node:: save( const string &binfile) const
            {
                ios::ocstream fp(binfile);
                save(fp);
            }

            void Node:: save( const char *binfile) const
            {
                const string _(binfile);
                save(_);
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
            TerminalNode:: ~TerminalNode() throw()
            {
                if(lx)
                {
                    delete lx;
                    lx = 0;
                }
            }

            TerminalNode:: TerminalNode(const Rule &r, Lexeme *l) throw() :
            Node(r,true),
            lx(l)
            {
                assert(NULL!=lx);
            }

            Node * TerminalNode:: clone() const
            {
                return Node::Create(rule, new Lexeme(*lx) );
            }

            const void  * TerminalNode:: inner() const throw()
            {
                assert(lx);
                return lx;
            }

            void     TerminalNode::   viz( ios::ostream &fp ) const
            {
                assert(lx);
                string l = string_convert::to_printable( rule.name );
                if(lx->size)
                {
                    const string content = lx->to_print();
                    l <<'=' << '\'' << content << '\'';
                }
                graphVizName(fp); fp("[shape=box,label=\""); fp << l; fp("\"];\n");
            }

            void TerminalNode:: emit(ios::ostream &fp) const
            {
                assert(lx);
                fp.emit(MAGIC_BYTE);

                fp.emit_upack(lx->size);
                for(const Char *ch = lx->head;ch;ch=ch->next)
                {
                    fp.emit(ch->code);
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
            InternalNode:: ~InternalNode() throw()
            {
            }

            InternalNode:: InternalNode(const Rule &r) throw() :
            Node(r,false), Node::List()
            {
            }

            InternalNode:: InternalNode(const InternalNode &node) :
            Node(node),
            Node::List(node)
            {

            }

            Node * InternalNode:: clone() const
            {
                return new InternalNode(*this);
            }

            const void  * InternalNode:: inner() const throw()
            {
                return static_cast<const List *>(this);
            }

            void     InternalNode::   viz( ios::ostream &fp ) const
            {
                const string l = string_convert::to_printable(rule.name);
                graphVizName(fp); fp("[shape=house,label=\""); fp << l; fp("\"];\n");
                const bool multiple = size>1;
                unsigned  idx=1;
                for(const Node *node = head; node; node=node->next, ++idx)
                {
                    node->viz(fp);
                    graphVizName(fp); fp << "->"; node->graphVizName(fp);
                    if(multiple) fp("[label=\"%u\"]",idx);
                    fp << ";\n";
                }
            }

            void InternalNode:: emit(ios::ostream &fp) const
            {
                fp.emit(MAGIC_BYTE);
                fp.emit_upack(size);
                for(const Node *node=head;node;node=node->next)
                {
                    node->save(fp);
                }
            }

        }
    }
}

