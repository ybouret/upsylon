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

            Node * Node:: Create(const Rule &r, const string &s)
            {
                return new InternalNode(r,s);
            }

            const Lexeme & Node:: lexeme() const throw()
            {
                const void *addr = inner();
                assert(terminal);
                assert(addr);
                return *static_cast<const Lexeme *>(addr);
            }

            Lexeme & Node:: lexeme() throw()
            {
                void *addr = (void*)inner();
                assert(terminal);
                assert(addr);
                return *static_cast<Lexeme *>(addr);
            }

            const Node::List & Node:: children() const throw()
            {
                const void *addr = inner();
                assert(internal);
                assert(addr);
                return *static_cast<const List *>(addr);
            }

            Node::List & Node:: children() throw()
            {
                void *addr = (void*)inner();
                assert(internal);
                assert(addr);
                return *static_cast<List *>(addr);
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


            void InternalNode:: returnTo(Lexer &lexer) throw()
            {
                while(size>0)
                {
                    tail->returnTo(lexer);
                    delete pop_back();
                }
            }

            void TerminalNode:: returnTo(Lexer &lexer) throw()
            {
                lexer.unget(lx);
                lx=0;
            }

            void   Node:: Unget( Node * &node, Lexer &lexer) throw()
            {
                assert(node);
                node->returnTo(lexer);
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

            const string * TerminalNode:: data() const throw() { return 0; }
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
            Node(r,false),
            Node::List(),
            _data(0)
            {
            }

            InternalNode:: InternalNode(const InternalNode &node) throw() :
            Node(node),
            Node::List(node),
            _data(node._data)
            {

            }

            InternalNode:: InternalNode(const Rule &r, const string &s) :
            Node(r,false),
            Node::List(),
            _data( new string(s) )
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

                //--------------------------------------------------------------
                // internal data
                //--------------------------------------------------------------
                if(_data.is_valid())
                {
                    fp.emit<uint8_t>(1);
                    string_io::save_binary(fp,*_data);
                }
                else
                {
                    fp.emit<uint8_t>(0);
                }

                //--------------------------------------------------------------
                // children
                //--------------------------------------------------------------
                fp.emit_upack(size);
                for(const Node *node=head;node;node=node->next)
                {
                    node->save(fp);
                }


            }

            const string * InternalNode:: data() const throw()
            {
                return ( _data.is_valid() ) ? & *_data : 0;
            }


        }
    }
}

#include "y/lang/syntax/terminal.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            void     TerminalNode::   viz( ios::ostream &fp ) const
            {
                assert(lx);
                string l = string_convert::to_printable( rule.name );
                if(lx->size)
                {
                    const string content = lx->to_print();
                    l <<'=' << '\'' << content << '\'';
                }
                const char *sh =  "box";
                const char *st =  "solid";
                switch( rule.as<Terminal>().attr )
                {
                    case Standard: break;
                    case Operator: sh="triangle"; break;
                    case Semantic: st="dashed";   break;
                }
                graphVizName(fp); fp("[shape=\"%s\",style=\"%s\",label=\"",sh,st); fp << l; fp("\"];\n");
            }

        }
    }
}
