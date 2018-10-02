
#include "y/lang/dynamo/generator.hpp"
//#include "y/lang/lexical/plugin/strings.hpp"
#include "y/lang/lexical/plugin/comment.hpp"

namespace upsylon
{

    namespace Lang
    {
        string DynamoGenerator:: LxrToRX(const Node   *sub,
                                         const string &lxr,
                                         string       &id) const
        {
            static const char fn[] = "LxrToRX";
            Y_DYNAMO_CHECK(sub->terminal);
            const string &name = sub->rule.name;
            switch(hstr(name))
            {
                case 0: assert("RX"==name); {
                    const string rx = sub->lexeme.to_string(1,1);
                    id = rx;
                    if(verbose) { indent() << "\\__RX=<" << rx << ">" << std::endl; }
                    return rx;
                }

                case 1: assert("RS"==name); {
                    const string rs = sub->lexeme.to_string(1,1);
                    const string rx = StringToRegExp(rs);
                    id = rs;
                    if(verbose) { indent() << "\\__RS=<" << rs << "> => <" << rx << ">" << std::endl; }
                    return rx;
                }

                default:
                    break;
            }
            throw Exception(fn,"{%s} unexpected '%s' @%s", **(parser->name), *name, *lxr);

        }

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
                    for( const Node *sub = content->next;sub;sub=sub->next)
                    {
                        string       id;
                        const string rx = LxrToRX(sub,identifier,id);
                        parser->root.drop(id,rx);
                    }
                } break;

                case 1: assert("endl"==identifier); {
                    for( const Node *sub = content->next;sub;sub=sub->next)
                    {
                        string       id;
                        const string rx = LxrToRX(sub,identifier,id);
                        parser->root.endl(id,rx);
                    }
                } break;

                case 2: assert("comment"==identifier); {

                } break;

                default:
                    throw Exception(fn,"{%s} unknown lexical identifier <%s>",**(parser->name),*identifier);
            }

        }

    }

}

