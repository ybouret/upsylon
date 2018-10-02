#include "y/lang/dynamo/generator.hpp"
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
                        onLxr(*sub);
                        break;

                    case 3: assert("PLUGIN"==rid);
                        onPlugin(*sub);
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


        
    }

}


