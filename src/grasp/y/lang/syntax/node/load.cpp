#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/string/io.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
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
                const unsigned    magic     = source.read<uint8_t>();         //!< then read the type of node

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
                                const uint8_t code = source.read<uint8_t>();
                                plx->push_back( new Char(code,info) );
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
                        auto_ptr<Node> node    = 0;

                        //------------------------------------------------------
                        // check data and create node accordingly
                        //------------------------------------------------------
                        const unsigned  hasData = source.read<uint8_t>();

                        switch(hasData)
                        {
                            case 0x00: node = Node::Create(the_rule); break;
                            case 0x01: {
                                const string s = string_io::load_binary(source);
                                node           = Node::Create(the_rule,s);
                            } break;

                            default:
                                throw exception("%s(invalid hasData=0x%02x for <%s>)",fn, hasData, *(the_rule.name) );
                        }

                        //------------------------------------------------------
                        // read children
                        //------------------------------------------------------
                        const size_t   nch  = source.read_upack<size_t>();
                        List          &ch   = node->children();
                        for(size_t i=nch;i>0;--i)
                        {
                            ch.push_back( Load(source,G) );
                        }

                        //------------------------------------------------------
                        // done
                        //------------------------------------------------------
                        return node.yield();
                    }

                    default:
                        ;
                }

                throw exception("%s(unknown MAGIC=0x%02x for <%s>)", fn, magic, *(the_rule.name) );
            }

        }
    }
}

