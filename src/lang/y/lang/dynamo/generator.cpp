
#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {

        static const char *top_kw[] =
        {
            "RULE",
            "ALIAS",
            "LXR",
            "PLUGIN"
        };

        static const char *str_kw[] =
        {
            "RX",
            "RS",
            "^"
        };

        static const char *lxr_kw[] =
        {
            "drop",
            "endl",
            "comment"
        };

        DynamoGenerator:: DynamoGenerator():
        parser(0),
        verbose(false),
        top(8,as_capacity),
        level(0),
        htop( YOCTO_MPERF_FOR(top_kw) ),
        hstr( YOCTO_MPERF_FOR(str_kw) ),
        hlxr( YOCTO_MPERF_FOR(lxr_kw) )
        {
        }

        std::ostream & DynamoGenerator:: indent() const
        {
            std::cerr << "|_";
            for(int i=0;i<level;++i) std::cerr << "__";
            return std::cerr;
        }


        DynamoGenerator:: ~DynamoGenerator() throw()
        {

        }

        Syntax::Parser * DynamoGenerator:: create( Node &dynamo, const bool flag )
        {
            //modules.clear();
            top.free();
            parser  = 0;
            level   = 0;
            verbose = flag;

            const string parserName = getModuleName(dynamo);
            if(verbose) { indent() << "creating parser {" << parserName << "}" << std::endl; }
            parser = new Syntax::Parser(parserName);

            collectTopLevel( &dynamo );

            parser->GraphViz("parser.dot");
            return parser.yield();
        }

        string DynamoGenerator:: getModuleName( const Node &dynamo ) const
        {
            assert(dynamo.internal);
            assert(dynamo.rule.name=="dynamo");
            assert(dynamo.children.size>0);
            const Node *node = dynamo.children.head;
            assert(node);
            assert(node->terminal);
            assert(node->rule.name=="Module");
            return node->lexeme.to_string(1,0);
        }

#if 0
        void DynamoGenerator:: collectSubModules(Node *node) throw()
        {
            assert(node);
            if(node->terminal)
            {
                return;
            }
            else
            {
                Node::List &source = node->children;
                Node::List  target;
                while(source.size)
                {
                    Node *sub = source.pop_front();
                    if(sub->rule.name=="dynamo")
                    {
                        modules << sub;
                    }
                    else
                    {
                        target << sub;
                    }
                }
                target.swap_with(source);
            }
        }
#endif


        string DynamoGenerator:: getNodeName( const Node &node, const char *label, const size_t nskip ) const
        {
            // a little sanity check
            assert(parser.is_valid());
            assert(node.internal);
            assert(node.children.size>0);
            assert(node.children.head);
            assert(node.children.head->terminal);


            const Node *head = node.children.head;
            if(label != *(head->lexeme.label) )
            {
                throw exception("{%s} unexpected label '%s' for '%s'", **(parser->name), **(head->lexeme.label), label );
            }
            return head->lexeme.to_string(nskip,0);
        }






        string DynamoGenerator:: nodeToRegExp(const Node &node, int &h) const
        {
            const string &name = node.rule.name;
            switch( (h=hstr(name)) )
            {
                    //----------------------------------------------------------
                    // extract a regular expression
                    //----------------------------------------------------------
                case 0: assert("RX"==name);
                    if(verbose) { indent() << "@regular expression from STD <" << node.lexeme << ">" << std::endl; }
                    return node.lexeme.to_string(1,1);

                    //----------------------------------------------------------
                    // extract a raw string and make it to a regular expression
                    //----------------------------------------------------------
                case 1: assert("RS"==name);
                {
                    if(verbose) { indent() << "@regular expression from RAW <" << node.lexeme << ">" << std::endl; }
                    const string rs = node.lexeme.to_string(1,1);
                    return StringToRegExp(rs);

                }

                    //----------------------------------------------------------
                    // extract a raw string and make it to a regular expression
                    // will be made an operator
                    //----------------------------------------------------------
                case 2: assert("^" ==name); {
                    assert(node.children.size==1);
                    const Node *sub = node.children.head;
                    assert(sub);
                    assert(sub->terminal);
                    assert(sub->rule.name=="RS");
                    if(verbose) { indent() << "@regular expression from OP  <" << sub->lexeme << ">" << std::endl; }
                    const string rs = sub->lexeme.to_string(1,1);
                    return StringToRegExp(rs); }

                    //----------------------------------------------------------
                    // unexpected
                    //----------------------------------------------------------
                default:
                    break;
            }
            throw exception("{%s} invalid string class '%s'", **(parser->name), *name);

        }

        void DynamoGenerator:: createSpecificTerminal(const string &label, const Node &node)
        {
            const string &name = node.rule.name;
            int           h    = -1;
            const string  rx   = nodeToRegExp(node,h);

            switch(h)
            {
                case 0:
                    if(verbose) { indent() << "@--> compile <" << rx << ">/term" << std::endl; }
                    (void)parser->term(label,rx);
                    break;

                case 1:
                    if(verbose) { indent() << "@--> compile <" << rx << ">/sole" << std::endl; }
                    (void) parser->sole(label,rx);
                    break;

                case 2:
                    if(verbose) { indent() << "@--> compile <" << rx << ">/op" << std::endl; }
                    (void) parser->op(label,rx);
                    break;

                default:
                    throw exception("{%s} invalid alias kind '%s'", **(parser->name), *name);
            }
        }


       

    }

}
