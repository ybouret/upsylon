
#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"
#include "y/lang/lexical/plugin/strings.hpp"
#include "y/lang/lexical/plugin/comment.hpp"

namespace upsylon
{

    namespace Lang
    {

#if 0
        //----------------------------------------------------------------------
        //
        // recursive top level rules build up
        //
        //----------------------------------------------------------------------
        void DynamoGenerator:: topLevel(Node *node)
        {
            assert(node);
            if(verbose) { indent() << "<" << node->rule.name << ">" << std::endl; }
            bool enterModule = false;
            if(node->rule.name == "dynamo" )
            {
                enterModule = true;
                const string moduleName = getModuleName(*node);
                const Origin moduleOrgn = new string(moduleName);
                modules.push(moduleOrgn);
                indent() << " (+) stack: " << modules << std::endl;
            }
            if(node->internal)
            {
                Node::List    &source = node->children;
                Node::SaveList target;
                while(source.size)
                {
                    auto_ptr<Node> sub = source.pop_front();
                    const string   rid = sub->rule.name;
                    switch( htop(rid) )
                    {
                        case 0: assert("RULE"==rid);
                            onRule(*sub);
                            target << sub.yield();
                            break;

                        case 1: ;assert("ALIAS"==rid);
                            onAlias(*sub);
                            break;

                        case 2: assert("LXR"==rid);
                            onLxr(*sub);
                            break;

                        case 3: assert("PLUGIN"==rid);
                            onPlugin(*sub);
                            break;

                        default:
                            ++level;
                            topLevel( & *sub );
                            --level;
                            target << sub.yield();
                    }
                }
                source.swap_with(target);
            }
            if(enterModule)
            {
                assert(modules.size()>0);
                modules.pop();
                indent() << " (-) stack => " << modules << std::endl;
            }
        }


        //----------------------------------------------------------------------
        //
        // create a new top-level rule
        //
        //----------------------------------------------------------------------
        void DynamoGenerator:: onRule( const Node &node )
        {
            const string name = getNodeName(node,"ID",0);
            if(verbose) { indent() << "+RULE    <" << name << ">" << std::endl; }

            switch( getRuleKind(node) )
            {
                case isAGG:
                    parser->agg(name);
                    newInternal(name);
                    break;

                case isALT:
                    parser->alt(name);
                    break;

                case isOOM:
                case isZOM:
                case isOPT:
                case isID:
                    parser->acting(name);
                    break;

                case CARET:
                    throw exception("{%s} misplaced '^' operator sign: use alias or grouping", **(parser->name) );
                    
                default:
                    throw exception("{%s} unexpected top-level rule '%s'", **(parser->name), *name );
            }
            
        }

        //----------------------------------------------------------------------
        //
        // create a new alias
        //
        //----------------------------------------------------------------------
        void DynamoGenerator:: onAlias( const Node &node )
        {
            const string label = getNodeName(node,"ID",0); // the alias ;bel
            if(verbose) { indent() << "+ALIAS   <" << label << ">" << std::endl; }
            assert(node.children.size==2);
            const Node    *content = node.children.tail; assert(content);
            createSpecificTerminal(label,*content);
        }

        //----------------------------------------------------------------------
        //
        // create a new lexical only rule
        //
        //----------------------------------------------------------------------
        void DynamoGenerator:: onLxr( const Node &node )
        {
            const string label = getNodeName(node,"L_ID",1);
            if(verbose) { indent() << "+LXR     <" << label << ">" << std::endl; }
            const int   kind = hlxr(label);
            if(kind<0) throw exception("{%s} invalid lexical rule type '%s'", **(parser->name), *label);
            const Node *sub  = node.children.head;

            switch(kind)
            {
                case 0: assert("drop"==label);
                    for(sub=sub->next;sub;sub=sub->next)
                    {
                        const string rx = getNodeRegExp(*sub);
                        parser->root.drop(rx,rx);
                    }
                    break;

                case 1: assert("endl"==label);
                    for(sub=sub->next;sub;sub=sub->next)
                    {
                        const string rx = getNodeRegExp(*sub);
                        parser->root.endl(rx,rx);
                    }
                    break;

                case 2: assert("comment"==label);
                    switch(node.children.size) {
                        case 2: // 1+One Expression => EndOfLine
                        {
                            const string comName = getCommentLabel();
                            const string trigger = getNodeRegExp(*(sub->next));
                            if(verbose) { indent() << "End Of Line Comment '" << comName << "' with trigger='" << trigger << "'" << std::endl;}
                            parser->hook<Lexical::EndOfLineComment>(comName,*trigger);
                        } break;

                        case 3: //1+Two Expressions => MultiLines
                        {
                            const string comName = getCommentLabel();
                            const string rx_init = getNodeRegExp(*(sub->next));
                            const string rx_quit = getNodeRegExp(*(sub->next->next));
                            if(verbose) { indent() << "Multiple Line Comment '" << comName << "' with init='" << rx_init << "' and quit='" << rx_quit << "'" << std::endl; }
                            parser->hook<Lexical::MultiLinesComment>(comName,*rx_init,*rx_quit);
                        } break;

                        default:
                            throw exception("{%s} <%s> must have 1 or 2 arguments, found %u", **(parser->name), *label, unsigned(node.children.size-1) );
                    }
                    break;
            }
        }


        //----------------------------------------------------------------------
        //
        // create a new plugin
        //
        //----------------------------------------------------------------------
        void DynamoGenerator:: onPlugin(const Node &node)
        {
            const string label = getNodeName(node,"L_ID",1);
            if(verbose) { indent()  << "+PLUGIN  <" << label << ">" << std::endl; }
            const Node *sub = node.children.tail;
            assert(sub);
            assert(sub->terminal);
            assert(sub->rule.name=="ID");
            const string pluginClass = sub->lexeme.to_string();
            if(verbose) { indent() << "  \\_[" << pluginClass << "]" << std::endl; }

            newTerminal(label);

            // TODO: plugin factory, maybe...
            if( "jstring" == pluginClass )
            {
                // create the plugin
                parser->hook<Lexical::jString>(label);
                // associate the terminal
                parser->term(label);
                return;
            }

            if( "rstring" == pluginClass )
            {
                // create the plugin
                parser->hook<Lexical::rString>(label);
                // associate the terminal
                parser->term(label);
                return;
            }

            throw exception("{%s} unknown plugin 'class %s'", **(parser->name), *pluginClass);


        }
#endif
        
    }

}


