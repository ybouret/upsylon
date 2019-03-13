#include "y/lang/dynamo/generator.hpp"

namespace upsylon
{
    namespace Lang
    {
        void DynamoGenerator:: onAlias( const Node &alias )
        {
            static const char fn[] = "Alias";

            //------------------------------------------------------------------
            //
            // local sanity check
            //
            //------------------------------------------------------------------
            Y_DYNAMO_CHECK(parser.is_valid());
            Y_DYNAMO_CHECK(alias.internal);
            Y_DYNAMO_CHECK(alias.children.size==2);
            Y_DYNAMO_CHECK(alias.children.head);
            Y_DYNAMO_CHECK(alias.children.head->terminal);
            Y_DYNAMO_CHECK(modules.size()>0);

            //------------------------------------------------------------------
            //
            // get the terminal name
            //
            //------------------------------------------------------------------
            const string label = alias.children.head->lexeme.to_string();
            if(verbose) { indent() << "--> Alias <" << label << ">" << std::endl; }

            //------------------------------------------------------------------
            //
            // get the content node
            //
            //------------------------------------------------------------------
            const Node  *content      = alias.children.tail;Y_DYNAMO_CHECK(NULL!=content);
            const string &contentName = content->rule.name;

            //------------------------------------------------------------------
            //
            // create a terminal
            //
            //------------------------------------------------------------------
            switch( hstr(contentName) )
            {
                    //----------------------------------------------------------
                    // from a regular expression
                    //----------------------------------------------------------
                case 0: assert("RX"==contentName); {
                    Y_DYNAMO_CHECK(content->terminal);
                    const string   rx = content->lexeme.to_string(1,1);  if(verbose) { indent() << " |_RX=\"" << rx << "\"" << std::endl; }
                    const Rule    &r  = parser->term(label,rx);
                    const Terminal s  = new _Terminal(rx,r,modules.peek());
                    if(!terminals.insert(s))
                    {
                        throw Exception(fn,"{%s} multiple terminals \"%s\" from regular expression",**(parser->name),*rx);
                    }
                } break;

                    //----------------------------------------------------------
                    // from a raw string
                    //----------------------------------------------------------
                case 1: assert("RS"==contentName); {
                    Y_DYNAMO_CHECK(content->terminal);
                    const string   rs = content->lexeme.to_string(1,1);
                    const string   rx = StringToRegExp(rs);               if(verbose) { indent() << " |_RS='" << rs << "' => \"" << rx << "\"" << std::endl; }
                    const Rule    &r  = parser->sole(label,rx);
                    const Terminal s  = new _Terminal(rs,r,modules.peek());
                    if(!terminals.insert(s))
                    {
                        throw Exception(fn,"{%s} multiple terminals \"%s\" from raw string",**(parser->name),*rs);
                    }
                } break;

                    //----------------------------------------------------------
                    // from an operator string
                    //----------------------------------------------------------
                case 2: assert("OS"==contentName); {
                    Y_DYNAMO_CHECK(content->internal);
                    Y_DYNAMO_CHECK(content->children.size==1);
                    Y_DYNAMO_CHECK(content->children.head);
                    Y_DYNAMO_CHECK(content->children.head->terminal);
                    Y_DYNAMO_CHECK(content->children.head->rule.name=="RS");
                    const string   os = content->children.head->lexeme.to_string(1,1);
                    const string   rx = StringToRegExp(os);               if(verbose) { indent() << " |_OS='" << os << "' => \"" << rx << "\"" << std::endl; }
                    const Rule    &r  = parser->op(label,rx);
                    const Terminal s  = new _Terminal(os,r,modules.peek());
                    if(!terminals.insert(s))
                    {
                        throw Exception(fn,"{%s} multiple terminals \"%s\" from operator",**(parser->name),*os);
                    }
                }   break;

                default:
                    throw Exception(fn,"invalide alias content '%s'", *contentName);
            }

        }
    }
}


