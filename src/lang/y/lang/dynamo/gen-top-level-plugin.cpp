
#include "y/lang/dynamo/generator.hpp"
#include "y/lang/lexical/plugin/strings.hpp"

namespace upsylon
{

    namespace Lang
    {

        void DynamoGenerator:: onPlugin( const Node &plg )
        {
            static const char fn[] = "Plugin";

            //------------------------------------------------------------------
            // sanity check
            //------------------------------------------------------------------
            Y_DYNAMO_CHECK(plg.internal);
            Y_DYNAMO_CHECK(plg.children.size==2);

            //------------------------------------------------------------------
            // Get the plugin term node
            //------------------------------------------------------------------
            const Node *pluginTermNode = plg.children.head;
            Y_DYNAMO_CHECK(pluginTermNode!=NULL);
            Y_DYNAMO_CHECK(pluginTermNode->terminal);
            Y_DYNAMO_CHECK(pluginTermNode->rule.name=="L_ID");
            const string pluginTerm = pluginTermNode->lexeme.to_string(1,0);

            //------------------------------------------------------------------
            // Get the plugin name node
            //------------------------------------------------------------------
            const Node *pluginNameNode = plg.children.tail;
            Y_DYNAMO_CHECK(pluginNameNode!=NULL);
            Y_DYNAMO_CHECK(pluginNameNode!=pluginTermNode);
            Y_DYNAMO_CHECK(pluginNameNode->rule.name=="ID");
            const string pluginName = pluginNameNode->lexeme.to_string(0,0);


            if(verbose) { indent() << "--> Plugin <" << pluginName << "> --> <" << pluginTerm << ">" << std::endl; }

            const Syntax::Rule *rule = 0;
            if(pluginName=="jstring")
            {
                rule = & parser->plug<Lexical::jString>(pluginTerm);
                goto FINALIZE;
            }

            if(pluginName=="rstring")
            {
                rule = & parser->plug<Lexical::rString>(pluginTerm);
                goto FINALIZE;
            }

            throw Exception(fn,"{%s} unhandled plugin<%s>", **(parser->name), *pluginName);

        FINALIZE:
            Y_DYNAMO_CHECK(rule!=NULL);
            Y_DYNAMO_CHECK(modules.size()>0);
            const string   data = '$' + pluginName;
            const Terminal s    = new _Terminal(data,*rule,modules.peek());
            if(!terminals.insert(s))
            {
                throw Exception(fn,"{%s} unexpected failure to register plugin<%s>", **(parser->name), *pluginName);
            }

        }


    }

}


