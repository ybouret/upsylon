
#include "y/lang/dynamo/generator.hpp"
#include "y/lang/lexical/plugin/strings.hpp"
#include "y/lang/lexical/plugin/comment.hpp"

namespace upsylon
{

    namespace Lang
    {
        void DynamoGenerator:: onLxr( const Node &lxr )
        {
            static const char fn[] = "LXR";
            Y_DYNAMO_CHECK(parser.is_valid());
            Y_DYNAMO_CHECK(lxr.rule.name=="LXR");
            Y_DYNAMO_CHECK(lxr.internal);
            Y_DYNAMO_CHECK(lxr.children.size>0);
            Y_DYNAMO_CHECK(lxr.children.head);
            Y_DYNAMO_CHECK(lxr.children.head->terminal);
            Y_DYNAMO_CHECK(lxr.children.head->rule.name == "L_ID");
            const Node  *content    = lxr.children.head;
            const string identifier = content->lexeme.to_string(1,0);
            if(verbose) { indent() << "Lexical<" << identifier << ">" << std::endl; }

            switch( hlxr(identifier) )
            {
                case 0: assert("drop"==identifier); {

                } break;

                case 1: assert("endl"==identifier); {
                } break;

                case 2: assert("comment"==identifier); {

                } break;

                default:
                    throw Exception(fn,"{%s} unknown lexical identifier <%s>",**(parser->name),*identifier);
            }

        }

    }

}

