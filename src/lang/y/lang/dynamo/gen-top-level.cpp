
#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"
#include "y/lang/lexical/plugin/strings.hpp"
#include "y/lang/lexical/plugin/comment.hpp"

namespace upsylon
{

    namespace Lang
    {


        void DynamoGenerator:: topLevel( Node &dynamo )
        {
            static const char fn[] = "topLevel";

            const string moduleName = getModuleName(dynamo);
            {
                const Origin org = new string(moduleName);
                modules.push(org);
            }
            if(verbose) { indent() << "ENTER: topLevel <" << moduleName << "> | " << modules << std::endl; }
            ++level;

            Y_DYNAMO_CHECK(dynamo.children.size>0);
            Node::List    &source = dynamo.children; delete source.pop_front();
            Node::SaveList target;
            while(source.size)
            {
                auto_ptr<Node> sub = source.pop_front();
                const string  &rid = sub->rule.name;
                switch( htop(rid) )
                {
                    case 0: assert("RULE"==rid);
                        // process and keep
                        target << sub.yield();
                        break;

                    case 1: assert("ALIAS"==rid);
                        // process and remove
                        onAlias(*sub);
                        break;

                    case 2: assert("LXR"  ==rid);
                        // process and remove
                        break;

                    case 3: assert("PLUGIN"==rid);
                        //process and remove
                        break;

                    case 4: assert("dynamo"==rid);
                        // process and fusion
                        topLevel( *sub );
                        while(sub->children.size)
                        {
                            //target << sub.yield();
                            target << sub->children.pop_front();
                        }
                        break;

                    default:
                        throw Exception(fn,"unexpected top level <%s>", *rid);
                }
            }
            source.swap_with(target);
            --level;
            modules.pop();
            if(verbose) { indent() << "LEAVE: topLevel <" << moduleName << "> | " << modules << std::endl; }

        }

        
        void DynamoGenerator:: onAlias( const Node &alias )
        {
            static const char fn[] = "Alias";

            // get the terminal name
            Y_DYNAMO_CHECK(alias.internal);
            Y_DYNAMO_CHECK(alias.children.size==2);
            Y_DYNAMO_CHECK(alias.children.head);
            Y_DYNAMO_CHECK(alias.children.head->terminal);
            const string label = alias.children.head->lexeme.to_string();
            if(verbose) { indent() << "--> Alias <" << label << ">" << std::endl; }

            // get the content node
            const Node  *content = alias.children.tail;
            Y_DYNAMO_CHECK(NULL!=content);
            const string &contentName = content->rule.name;

            switch( hstr(contentName) )
            {
                case 0: assert("RX"==contentName); {
                    Y_DYNAMO_CHECK(content->terminal);
                    const string rx=content->lexeme.to_string(1,1);
                    if(verbose) { indent() << " |_RX=\"" << rx << "\"" << std::endl; }
                } break;

                case 1: assert("RS"==contentName); {
                    Y_DYNAMO_CHECK(content->terminal);
                    const string rs=content->lexeme.to_string(1,1);
                    if(verbose) { indent() << " |_RS=\"" << rs << "\"" << std::endl; }
                } break;

                case 2: assert("OS"==contentName); {
                    Y_DYNAMO_CHECK(content->internal);
                    Y_DYNAMO_CHECK(content->children.size==1);
                    Y_DYNAMO_CHECK(content->children.head);
                    Y_DYNAMO_CHECK(content->children.head->terminal);
                    Y_DYNAMO_CHECK(content->children.head->rule.name=="RS");
                    const string os=content->children.head->lexeme.to_string(1,1);
                    if(verbose) { indent() << " |_OS=\"" << os << "\"" << std::endl; }
                }   break;

                default:
                    throw Exception(fn,"invalide alias content '%s'", *contentName);
            }

        }
    }

}


