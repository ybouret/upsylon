#include "y/lang/dynamo/generator.hpp"
#include "y/string/convert.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {


        void DynamoGenerator:: storeDecl( Syntax::Terminal &t )
        {
            assert( parser.is_valid() );
            assert( modules.size()>0);

            const DynamoTerm symb(modules.back(),t);
            if( !symbols.insert(symb) || !terminals.insert(symb))
            {
                throw exception("{%s} multiple terminal <%s>",**(parser->name), *(t.name));
            }
        }


        void DynamoGenerator:: declModule(DynamoNode &dynamo)
        {
            static const char fn[] = "DynamoGenerator.declModule";

            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            if(dynamo.name!="dynamo")       throw exception("%s(expecting <dynamo> and not <%s>)",fn,*(dynamo.name));
            if(dynamo.type!=DynamoInternal) throw exception("%s(<dynamo> is not internal)",fn);

            DynamoList   &self = dynamo.children();
            if(self.size<=0)              throw exception("%s(Empty <dynamo> node)",fn);
            if(self.head->name!="module") throw exception("%s(Invalid Module Name='%s')",fn,*(self.head->name));

            //__________________________________________________________________
            //
            // build a tag
            //__________________________________________________________________
            const Tag moduleID = new string( self.head->content() );
            modules.push_back(moduleID);
            delete self.pop_front();

            //__________________________________________________________________
            //
            // and declare the new module
            //__________________________________________________________________
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[DECL '" << moduleID << "']" << std::endl);

            if(1==modules.size())
            {
                //______________________________________________________________
                //
                // would create parser
                //______________________________________________________________
                assert( parser.is_empty() );
                parser = new Syntax::Parser( *moduleID );
            }

            //__________________________________________________________________
            //
            // decl for sub items
            //__________________________________________________________________
            ++level;
            DynamoList temp;
            while(self.size)
            {
                auto_ptr<DynamoNode> sub  = self.pop_front();
                const string         id   = sub->name;
                bool                 keep = true;
                switch( declH(id) )
                {
                    case 0: assert("dynamo"==id); declModule(*sub); break;
                    case 1: assert("aka"==id);    declAlias(*sub);  keep=false; break;
                    case 2: assert("plg"==id);    declPlugin(*sub); keep=false; break;
                    default:
                        break;
                }

                if(keep) temp.push_back( sub.yield() );
            }
            --level;
            temp.swap_with(self);
            modules.pop_back();
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[DONE '" << moduleID << "']" << std::endl);
        }





        void DynamoGenerator:: declAlias( const DynamoNode &alias )
        {
            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            assert( "aka" == alias.name );
            assert( parser.is_valid()   );

            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[ALIAS]" << std::endl);
            if( alias.type != DynamoInternal ) throw exception("{%s} unexpected terminal alias", **(parser->name));

            //__________________________________________________________________
            //
            // get name
            //__________________________________________________________________
            const DynamoNode *node = alias.children().head;
            const string aliasName = getRID(node,"alias name");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_name='" << aliasName << "'" << std::endl );

            //__________________________________________________________________
            //
            // get string
            //__________________________________________________________________
            const string aliasExpr = getSTR(node=node->next,"alias string");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_expr='" << aliasExpr << "'" << std::endl );

            if((node=node->next))
            {
                // check modifier is '^'
                Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_operator" << std::endl);
                const string aliasMod = getContent(node, "^", "alias modifier");
                if(aliasMod.size()>0)       throw exception("{%s} unexpected alias '%s' modifier content='%s'", **(parser->name), *aliasName, *aliasMod);
                if(NULL!=(node=node->next)) throw exception("{%s} unexpected extraneous child for alias '%s'",  **(parser->name), *aliasName);
            }
            else
            {
                // nothing to do
                Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_standard" << std::endl);
            }

            //__________________________________________________________________
            //
            // declare it
            //__________________________________________________________________
            Syntax::Terminal &t = parser->term(aliasName,aliasExpr);
            if(node) t.op();
            storeDecl(t);

        }

        void DynamoGenerator:: declPlugin( const DynamoNode &plg )
        {
            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            assert( "plg" == plg.name );
            assert( parser.is_valid()   );

            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[PLUGIN]" << std::endl);
            if( plg.type != DynamoInternal ) throw exception("{%s} unexpected terminal plugin", **(parser->name));
            const DynamoNode *node     = plg.children().head;
            const string      plgLabel = getLID(node,"plugin label");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_label='" << plgLabel << "'" << std::endl );

            const string      plgClass = getRID(node=node->next,"plugin class");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_class='" << plgClass << "'" << std::endl );

            if(NULL!=(node=node->next))
            {
                throw exception("{%s} unexpected extraneous args for plugin '%s'", **(parser->name), *plgLabel);
            }

            //__________________________________________________________________
            //
            // find plugin and register corresponding syntax terminal
            //__________________________________________________________________
            DynamoPlugin     &P = findPlugin(plgClass);
            Syntax::Terminal &t = P(plgLabel,*parser);
            storeDecl(t);

        }


    }

}
