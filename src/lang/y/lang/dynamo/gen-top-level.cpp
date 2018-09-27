
#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {

        //----------------------------------------------------------------------
        //
        // recursive top level rules build up
        //
        //----------------------------------------------------------------------
        void DynamoGenerator:: collectTopLevel(Node *node)
        {
            assert(node);
            if(verbose) { indent() << "<" << node->rule.name << ">" << std::endl; }
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
                            collectTopLevel( & *sub );
                            --level;
                            target << sub.yield();
                    }
                }
                source.swap_with(target);
            }
        }


        //----------------------------------------------------------------------
        //
        // create a new top-level rule
        //
        //----------------------------------------------------------------------
        void DynamoGenerator:: onRule( const Node &node )
        {
            const string name = getNodeName(node,"ID",0); // the rule name
            Agg         &agg  = parser->agg(name);        // put in parser
            const MetaAgg m(agg);                         // keep it for later
            if( ! top.insert(m) )
            {
                throw exception("{%s} unexpected multiple rule '%s'", **(parser->name), *name );
            }
            if(verbose) { indent() << "..RULE    <" << name << ">" << std::endl; }
        }

        //----------------------------------------------------------------------
        //
        // create a new alias
        //
        //----------------------------------------------------------------------
        void DynamoGenerator:: onAlias( const Node &node )
        {
            const string label = getNodeName(node,"ID",0); // the alias ;bel
            if(verbose) { indent() << "..ALIAS   <" << label << ">" << std::endl; }
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
            if(verbose) { indent() << "..LXR     <" << label << ">" << std::endl; }
            const int   kind = hlxr(label);
            if(kind<0) throw exception("{%s} invalid lexical rule type '%s'", **(parser->name), *label);
            const Node *sub  = node.children.head;
            for(sub=sub->next;sub;sub=sub->next)
            {
                assert(sub->terminal);
                int          h  = -1;
                const string rx = nodeToRegExp(*sub,h);
                switch(kind)
                {
                    case 0: assert("drop"==label);
                        parser->root.drop(rx,rx);
                        break;

                    case 1: assert("endl"==label);
                        parser->root.endl(rx,rx);
                        break;
                }
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
            if(verbose) { indent()  << "..PLUGIN <" << label << ">" << std::endl; }
            const Node *sub = node.children.tail;
            assert(sub);
            assert(sub->terminal);
            assert(sub->rule.name=="ID");
            const string pluginClass = sub->lexeme.to_string();
            if(verbose) { indent() << "  |_[" << pluginClass << "]" << std::endl; }

            // TODO: plugin factory, maybe...



        }

    }

}


