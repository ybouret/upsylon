#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/string/io.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            namespace
            {
                static inline
                void LoadChildren( Node::List &ch, Source &source, Grammar &G)
                {
                    const size_t nch  = source.read_upack<size_t>();
                    for(size_t i=nch;i>0;--i)
                    {
                        ch.push_back( Node::Load(source,G) );
                    }
                }
            }

            Node  * Node:: Load(Source &source, Grammar &G)
            {
                static const char fn[] = "Node.Load";

                //--------------------------------------------------------------
                //
                // read and get the rule
                //
                //--------------------------------------------------------------
                const string      rule_name = string_io::load_binary(source); //!< name of the rule
                const Rule    &   the_rule  = G.getRuleByName(rule_name);     //!< get the rule from grammar

                //--------------------------------------------------------------
                //
                // read MAGIC to know if internal or terminal node
                //
                //--------------------------------------------------------------
                const unsigned    magic     = source.read_net<uint8_t>();         //!< then read the type of node

                switch( magic )
                {
                        //------------------------------------------------------
                        //
                    case TerminalNode::MAGIC_BYTE:
                        //
                        //------------------------------------------------------
                    {
                        const Tag       &tag = (*source)->origin; //!< common shallow tag
                        auto_ptr<Lexeme> plx = new Lexeme( tag ); //!< create lexeme
                        {
                            const size_t     nch = source.read_upack<size_t>();
                            const CharInfo   info(tag,0,0);
                            for(size_t i=nch;i>0;--i)
                            {
                                plx->push_back( new Char(source.read_net<uint8_t>(),info) );
                            }
                        }
                        return Node::Create(the_rule,plx.yield());
                    }
                        //------------------------------------------------------
                        //
                    case InternalNode::MAGIC_BYTE:
                        //
                        //------------------------------------------------------
                    {
                        auto_ptr<Node> node    = Node::Create(the_rule);
                        LoadChildren(node->children(),source,G);
                        return node.yield();
                    }

                        //------------------------------------------------------
                        //
                    case ExtendedNode::MAGIC_BYTE:
                        //
                        //------------------------------------------------------
                    {
                        const string   s       = string_io::load_binary(source);
                        auto_ptr<Node> node    = Node::Create(the_rule,s);
                        LoadChildren(node->children(),source,G);
                        return node.yield();
                    }
                    default: break;
                }

                throw exception("%s(unknown MAGIC=0x%02x for <%s>)", fn, magic, *(the_rule.name) );
            }

            Node  * Node:: Load( Module *m, Grammar &G)
            {
                Source source(m);
                return Load(source,G);
            }

        }
    }
}

