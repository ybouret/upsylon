#include "y/lang/dynamo/loader.hpp"
#include "y/exception.hpp"
#include "y/fs/local/fs.hpp"

namespace upsylon
{
    namespace Lang
    {

        DynamoLoader:: ~DynamoLoader() throw() {}

        DynamoLoader:: DynamoLoader() : DynamoCoreParser(),
        isRS("rs"),
        isRX("rx"),
        isSTR("rx|rs")
        {

        }

        XNode * DynamoLoader:: load( Module *m )
        {
            Source          source(m);
            auto_ptr<XNode> g = run(source);

            checkIncludes(*g,**source);

            return g.yield();
        }


        XNode  * DynamoLoader:: getCmdArgs( Syntax::Node &cmd, string &cmdName ) const
        {
            assert( "cmd" == cmd.rule.name );
            if(!cmd.internal)        throw exception("{%s} invalid <cmd> node", **name);
            const Syntax::Node *sub = cmd.children().head;
            if(!sub)                  throw exception("{%s} empty <cmd> node!!",**name);
            if(!sub->terminal)        throw exception("{%s} invalid <cmd>[1]!!",**name);
            if("cid"!=sub->rule.name) throw exception("{%s} invalid <cmd>[1].name='%s'!='cid'!!",**name,*(sub->rule.name));
            const Lexeme &lx = sub->lexeme();
            if(lx.size<=1||'%'!=lx.head->code) throw exception("{%s} invalid command identifier",**name);
            const string s = sub->lexeme().toString(1,0);
            cmdName = s;
            return sub->next;
        }


        string      DynamoLoader:: getString(const XNode &node,
                                             const char  *description,
                                             Matching    &matching) const
        {
            assert(description);
            const RULE   &theRule = node.rule;
            const string &theName = theRule.name;
            if(!node.terminal)             throw exception("{%s} getString<%s>(invalid  node <%s>)", **name, description, *theName );
            if(!matching.exactly(theName)) throw exception("{%s} getString<%s>(mismatch node <%s>)", **name, description, *theName );
            return node.lexeme().toString(1,1);
        }
        
        string DynamoLoader:: getContent(const XNode &node,
                                         const char  *description,
                                         const char  *id) const
        {
            assert(description);
            assert(id);
            const RULE   &theRule = node.rule;
            const string &theName = theRule.name;
            if(!node.terminal) throw exception("{%s} getLabel<%s>(invalid  node <%s>)", **name, description, *theName );
            if(theName!=id)    throw exception("{%s} getLabel<%s>(invalid  node <%s!=%s>)", **name, description, *theName,id );
            return node.lexeme().toString();
        }
        
        string DynamoLoader:: getRS( const XNode &node ) const
        {
            return getString(node,"raw string",isRS);
        }

#if 1
        string DynamoLoader:: getRX( const Syntax::Node &node ) const
        {
            return getString(node,"regular expression",isRX);

        }

        string DynamoLoader:: getSTR( const XNode &node ) const
        {
            return getString(node,"any string",isSTR);
        }

        string DynamoLoader:: getRID( const XNode &node ) const
        {
            return getContent(node, "rule identifier", "rid");
        }
#endif
        

        void DynamoLoader:: checkIncludes(Syntax::Node &node, const Module &currentModule)
        {
            Y_LANG_SYNTAX_VERBOSE(std::cerr << "{" << name << "} checkInclude" << std::endl);
            if(node.internal)
            {
                //--------------------------------------------------------------
                //
                //
                // scan all sub nodes
                //
                //
                //--------------------------------------------------------------
                Syntax::Node::List &self = node.children();
                {
                    Syntax::Node::List temp;
                    //----------------------------------------------------------
                    //
                    //
                    // ENTER: internal loop
                    //
                    //
                    //----------------------------------------------------------
                    while( self.size > 0 )
                    {
                        auto_ptr<Syntax::Node> cmd( self.pop_front() );

                        if( "cmd" == cmd->rule.name )
                        {
                            //--------------------------------------------------
                            //
                            // this is a command : get its name
                            //
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
                                    const string inc = cwd + getRS(*args);
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
                            //
                            // this is a NOT command
                            //
                            //--------------------------------------------------
                            temp.push_back( cmd.yield() );
                        }
                    }

                    //----------------------------------------------------------
                    //
                    //
                    // LEAVE: internal loop
                    //
                    //
                    //----------------------------------------------------------
                    temp.swap_with(self);
                }
            }
        }

    }

}

