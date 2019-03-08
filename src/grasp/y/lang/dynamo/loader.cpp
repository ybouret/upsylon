#include "y/lang/dynamo/loader.hpp"
#include "y/exception.hpp"
#include "y/fs/local.hpp"

namespace upsylon
{
    namespace Lang
    {

        DynamoLoader:: ~DynamoLoader() throw() {}

        DynamoLoader:: DynamoLoader() : DynamoParser()
        {

        }

        Syntax::Node * DynamoLoader:: load( Module *m )
        {
            Source                 source(m);
            auto_ptr<Syntax::Node> g = run(source);

            checkIncludes(*g,**source);

            return g.yield();
        }


        Syntax::Node * DynamoLoader:: getCmdArgs( Syntax::Node &cmd, string &cmdName ) const
        {
            assert( "cmd" == cmd.rule.name );
            if(!cmd.internal)        throw exception("{%s} invalid <cmd> node", **name);
            const Syntax::Node *sub = cmd.children().head;
            if(!sub)                  throw exception("{%s} empty <cmd> node!!",**name);
            if(!sub->terminal)        throw exception("{%s} invalid <cmd>[1]!!",**name);
            if("cid"!=sub->rule.name) throw exception("{%s} invalid <cmd>[1].name='%s'!='cid'!!",**name,*(sub->rule.name));
            const Lexeme &lx = sub->lexeme();
            if(lx.size<=1||'%'!=lx.head->code) throw exception("{%s} invalid command identifier",**name);
            const string s = sub->lexeme().to_string(1,0);
            cmdName = s;
            return sub->next;
        }
        

        void DynamoLoader:: checkIncludes(Syntax::Node &node, const Module &currentModule)
        {
            Y_LANG_SYNTAX_VERBOSE(std::cerr << "{" << name << "} checkInclude" << std::endl);
            if(node.internal)
            {
                //--------------------------------------------------------------
                //
                // scan all sub nodes
                //
                //--------------------------------------------------------------
                Syntax::Node::List &self = node.children();
                {
                    Syntax::Node::List temp;
                    //----------------------------------------------------------
                    //
                    // ENTER: internal loop
                    //
                    //----------------------------------------------------------
                    while( self.size > 0 )
                    {
                        auto_ptr<Syntax::Node> cmd( self.pop_front() );

                        if( "cmd" == cmd->rule.name )
                        {
                            //--------------------------------------------------
                            // this is a command
                            //--------------------------------------------------
                            Y_LANG_SYNTAX_VERBOSE(std::cerr << "{" << name << "} <cmd>" << std::endl);
                            string        cmdName;
                            Syntax::Node *args = getCmdArgs(*cmd,cmdName);
                            Y_LANG_SYNTAX_VERBOSE(std::cerr << "{" << name << "} |_'" << cmdName << "'" << std::endl);
                            if( "include" == cmdName )
                            {
                                //----------------------------------------------
                                // this is an 'include' : get directory
                                //----------------------------------------------
                                if(currentModule.type!=Module::FromFile)
                                {
                                    throw exception("{%s} cannot include from '%s", **name, **(currentModule.origin));
                                }
                                const string cwd = vfs::get_file_dir( *currentModule.origin );
                                Y_LANG_SYNTAX_VERBOSE(std::cerr << "{" << name << "} |_cwd=[" << cwd << "]" << std::endl);

                                //----------------------------------------------
                                // include sub files
                                //----------------------------------------------
                                for(;args;args=args->next)
                                {
                                    if(!args->terminal)         throw exception("{%s} invalid include argument",**name);
                                    if("rs"!=args->rule.name)   throw exception("{%s} invalid include argument name",**name);
                                    const string inc = cwd + args->lexeme().to_string(1,1);
                                    Y_LANG_SYNTAX_VERBOSE(std::cerr << "{" << name << "} |_loading=[" << inc << "]" << std::endl);
                                    temp.push_back( load( Module::OpenFile(inc) ) );
                                }
                                //----------------------------------------------
                                // and cmd content is destroyed by auto_ptr
                                //----------------------------------------------
                            }
                            else
                            {
                                //----------------------------------------------
                                // this is NOT an 'include'
                                //----------------------------------------------
                                temp.push_back( cmd.yield() );
                            }

                        }
                        else
                        {
                            //--------------------------------------------------
                            // this is a NOT command
                            //--------------------------------------------------
                            temp.push_back( cmd.yield() );
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // LEAVE: internal loop
                    //
                    //----------------------------------------------------------
                    temp.swap_with(self);
                }
            }
        }

    }

}

