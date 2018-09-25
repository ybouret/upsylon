#include "y/lang/dynamo/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {
        DynamoCompiler:: ~DynamoCompiler() throw()
        {
        }

        DynamoCompiler:: DynamoCompiler() :
        parser( new DynamoParser() )
        {
            
        }

        Syntax::Node * DynamoCompiler:: format( Module *module )
        {
            assert(module);
            Source source(module);
            parser->reset();
            auto_ptr<Node> ast = parser->parse(source,false);

            processCommands(*ast);

            return ast.yield();
        }


        void DynamoCompiler:: processCommands( Node &dynamo )
        {
            assert(dynamo.internal);
            assert(dynamo.rule.name=="dynamo");
            Node::List    &source = dynamo.children;
            Node::SaveList target;
            while(source.size)
            {
                auto_ptr<Node> node = source.pop_front();
                if(node->rule.name=="COMMAND")
                {
                    std::cerr << "Found Command!" << std::endl;
                    assert(node->internal);
                    const Node::List &args = node->children; assert(args.size>0);
                    const Node       *curr = args.head;      assert(curr); assert(curr->terminal); assert("CMD"==curr->rule.name);
                    const string      CMD  = curr->lexeme.to_string(1,0);
                    std::cerr << "CMD=<" << CMD << ">" << std::endl;

                    


                }
                else
                {
                    target << node.yield();
                }
            }
            target.swap_with(source);
        }

    }
}
