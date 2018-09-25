#include "y/lang/dynamo/compiler.hpp"
#include "y/exception.hpp"

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
            //__________________________________________________________________
            //
            //
            // scanning top-level nodes
            //
            //__________________________________________________________________
            Node::List    &source = dynamo.children;
            Node::SaveList target;
            while(source.size)
            {
                auto_ptr<Node> node = source.pop_front();
                if(node->rule.name=="COMMAND")
                {
                    std::cerr << "Found Command!" << std::endl;

                    //__________________________________________________________
                    //
                    // process the command
                    //__________________________________________________________
                    assert(node->internal);
                    const Node::List &args = node->children; assert(args.size>0);
                    const Node       *curr = args.head;      assert(curr); assert(curr->terminal); assert("CMD"==curr->rule.name);
                    const Lexeme     &lxm  = curr->lexeme;
                    const string      CMD  =lxm.to_string(1,0);
                    std::cerr << "CMD=<" << CMD << ">" << std::endl;

                    switch( parser->commands(CMD) )
                    {

                        default:
                            throw exception("%s:%d unknown command '%s'", lxm.origin(), lxm.line(), *CMD);
                    }
                    


                }
                else
                {
                    //__________________________________________________________
                    //
                    // regular node, leave untouched
                    //__________________________________________________________
                    target << node.yield();
                }
            }
            target.swap_with(source);
        }

    }
}
