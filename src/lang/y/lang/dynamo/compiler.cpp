#include "y/lang/dynamo/compiler.hpp"
#include "y/exception.hpp"
#include "y/fs/vfs.hpp"

namespace upsylon
{
    namespace Lang
    {
        DynamoCompiler:: ~DynamoCompiler() throw()
        {
        }

        DynamoCompiler:: DynamoCompiler() :
        parser(  ),
        depth( 0 )
        {
            
        }

        Syntax::Node * DynamoCompiler:: format( Module *module )
        {
            assert(module);
            Source source(module);
            parser.reset();
            auto_ptr<Node> ast = parser.parse(source,false);

            processCommands(*ast,*(module->origin));

            return ast.yield();
        }

        Syntax::Node *DynamoCompiler:: process( Module *module )
        {
            depth = 0;
            return format(module);
        }

        void DynamoCompiler:: processCommands( Node &dynamo, const string &origin )
        {
            assert(dynamo.internal);
            assert(dynamo.rule.name=="dynamo");
            //__________________________________________________________________
            //
            //
            // scanning top-level nodes for "COMMAND"
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
                    //
                    // process the command
                    //
                    //__________________________________________________________
                    assert(node->internal);
                    const Node::List &args = node->children; assert(args.size>0);
                    const Node       *curr = args.head;      assert(curr); assert(curr->terminal); assert("CMD"==curr->rule.name);
                    const Lexeme     &lxm  = curr->lexeme;
                    const string      CMD  = lxm.to_string(1,0);

                    switch( parser.commands(CMD) )
                    {
                        case DynamoParser::include:
                            std::cerr << "Processing include..." << std::endl;
                            if(args.size!=2)
                            {
                                throw exception("%s:%d: include must have exactly one argument",lxm.origin(),lxm.line());
                            }
                            else
                            {
                                const Node *incl = curr->next;
                                assert( "RX" == incl->rule.name || "RS" == incl->rule.name );
                                const string filename = vfs::get_file_dir(origin) + incl->lexeme.to_string(1,1);
                                std::cerr << "..include '" << filename << "'" << std::endl;
                                ++depth;
                                target << format( Module::OpenFile(filename) );
                                --depth;
                            }
                            break;

                        default:
                            throw exception("%s:%d unknown command '%s'", lxm.origin(), lxm.line(), *CMD);
                    }
                    
                    //__________________________________________________________
                    //
                    //
                    // ...processed
                    //
                    //__________________________________________________________
                }
                else
                {
                    //__________________________________________________________
                    //
                    //
                    // regular node, leave untouched
                    //
                    //__________________________________________________________
                    target << node.yield();

                }
            }
            target.swap_with(source);
        }

    }
}
