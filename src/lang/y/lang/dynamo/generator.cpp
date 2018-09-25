
#include "y/lang/dynamo/generator.hpp"

namespace upsylon
{

    namespace Lang
    {

        DynamoGenerator:: DynamoGenerator() :
        parser(0)
        {
        }

        DynamoGenerator:: ~DynamoGenerator() throw()
        {

        }

        Syntax::Parser * DynamoGenerator:: create( Node &dynamo )
        {
            modules.clear();
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

        void DynamoGenerator:: collectTopLevel(const Node *node)
        {
            if(node->internal)
            {
                for(const Node *sub=node->children.head;sub;sub=sub->next)
                {
                    if(sub->rule.name=="RULE")
                    {
                        std::cerr << "New Rule " << std::endl;
                        assert(sub->internal);
                        assert(sub->children.size>1);
                        
                    }
                    else
                    {
                        collectTopLevel(sub);
                    }
                }
            }

        }

    }

}
