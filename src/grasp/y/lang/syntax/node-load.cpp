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

                const string   rule_name = string_io::load_binary(source); //!< name of the rule
                const Rule    &the_rule  = G.getRuleByName(rule_name);     //!< get the rule from grammar
                const unsigned magic     = source.read<uint8_t>();         //!< then read the type of node

                switch( magic )
                {
                    case TerminalNode::MAGIC_BYTE:
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

                    case InternalNode::MAGIC_BYTE:
                    {
                        const size_t   nch  = source.read_upack<size_t>();
                        auto_ptr<Node> node = Node::Create(the_rule);
                        List          &ch   = node->children();
                        for(size_t i=nch;i>0;--i)
                        {
                            ch.push_back( Load(source,G) );
                        }
                        return node.yield();
                    }

                    default:;
                }

                throw exception("Node::Load(unknown MAGIC=0x%02x)", magic );
            }

        }
    }
}

