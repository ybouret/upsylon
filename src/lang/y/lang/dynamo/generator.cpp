
#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {

        static const char *top_kw[] =
        {
            "RULE",
            "ALIAS",
            "LXR",
            "PLUGIN"
        };


        DynamoGenerator:: DynamoGenerator() :
        parser(0),
        htop( YOCTO_MPERF_FOR(top_kw) )
        {
        }

        DynamoGenerator:: ~DynamoGenerator() throw()
        {

        }

        Syntax::Parser * DynamoGenerator:: create( Node &dynamo )
        {
            //modules.clear();
            top.free();
            parser = 0;
            
            const string parserName = getModuleName(dynamo);
            std::cerr << "..creating Parser '" << parserName << "'" << std::endl;
            parser = new Syntax::Parser(parserName);

            collectTopLevel( &dynamo );


            return parser.yield();
        }

        string DynamoGenerator:: getModuleName( const Node &dynamo ) const
        {
            assert(dynamo.internal);
            assert(dynamo.rule.name=="dynamo");
            assert(dynamo.children.size>0);
            const Node *node = dynamo.children.head;
            assert(node);
            assert(node->terminal);
            assert(node->rule.name=="Module");
            return node->lexeme.to_string(1,0);
        }

#if 0
        void DynamoGenerator:: collectSubModules(Node *node) throw()
        {
            assert(node);
            if(node->terminal)
            {
                return;
            }
            else
            {
                Node::List &source = node->children;
                Node::List  target;
                while(source.size)
                {
                    Node *sub = source.pop_front();
                    if(sub->rule.name=="dynamo")
                    {
                        modules << sub;
                    }
                    else
                    {
                        target << sub;
                    }
                }
                target.swap_with(source);
            }
        }
#endif


        string DynamoGenerator:: getNodeName( const Node &node, const char *label, const size_t nskip ) const
        {
            assert(parser.is_valid());
            assert(node.internal);
            assert(node.children.size>0);
            const Node *head = node.children.head;
            assert(head);
            assert(head->terminal);

            if(label != *(head->lexeme.label) )
            {
                throw exception("{%s} unexpected label '%s' for '%s'", ** (parser->name), **(head->lexeme.label), label );
            }

            return head->lexeme.to_string(nskip,0);
        }


        void DynamoGenerator:: collectTopLevel(Node *node)
        {
            if(node->internal)
            {
                Node::List    &source = node->children;
                Node::SaveList target;
                while(source.size)
                {
                    auto_ptr<Node> sub = source.pop_front();
                    const string   rid = sub->rule.name;
                    switch( htop(rid) )
                    {
                        case 0: assert(top_kw[0]==rid); assert("RULE"==rid);
                            std::cerr << "+" << top_kw[0] << " " << getNodeName(*sub,"ID",0) <<  std::endl;
                            target << sub.yield();
                            break;

                        case 1: assert(top_kw[1]==rid);assert("ALIAS"==rid);
                            std::cerr << "+" << top_kw[1] << " " << getNodeName(*sub,"ID",0) <<  std::endl;
                            break;

                        case 2: assert(top_kw[2]==rid);assert("LXR"==rid);
                            std::cerr << "+" << top_kw[2] << " " << getNodeName(*sub,"L_ID",0) <<  std::endl;
                            break;

                        case 3: assert(top_kw[3]==rid);assert("PLUGIN"==rid);
                            std::cerr << "+" << top_kw[3] << " " << getNodeName(*sub,"L_ID",0) <<  std::endl;
                            break;

                        default:
                            collectTopLevel( & *sub );
                            target << sub.yield();
                    }
                }
                source.swap_with(target);
            }

        }

    }

}
