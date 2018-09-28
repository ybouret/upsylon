
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

        static const char *syn_kw[] =
        {
            "AGG",
            "ALT",
            "ZOM",
            "OOM",
            "OPT"
        };

#define Y_DYN_CHECK(RULE) assert( hsyn(#RULE) == is##RULE )
        DynamoGenerator:: DynamoGenerator():
        parser(0),
        verbose(false),
        //top(8,as_capacity),
        level(0),
        htop( YOCTO_MPERF_FOR(top_kw) ),
        hsyn( YOCTO_MPERF_FOR(syn_kw) ),
        hstr( YOCTO_MPERF_FOR(str_kw) ),
        hlxr( YOCTO_MPERF_FOR(lxr_kw) ),
        icom(0),
        modules(4,as_capacity)
        {
            Y_DYN_CHECK(AGG);
            Y_DYN_CHECK(ALT);
            Y_DYN_CHECK(ZOM);
            Y_DYN_CHECK(OOM);
            Y_DYN_CHECK(OPT);
        }

        std::ostream & DynamoGenerator:: indent() const
        {
            std::cerr << "| ";
            for(int i=0;i<level;++i) std::cerr << "  ";
            return std::cerr;
        }


        DynamoGenerator:: ~DynamoGenerator() throw()
        {

        }

        Syntax::Parser * DynamoGenerator:: create( Node &dynamo, const bool flag )
        {
            //top.free();
            parser  = 0;
            level   = 0;
            verbose = flag;
            icom    = 0;
            modules.free();

            const string parserName = getModuleName(dynamo);
            if(verbose)
            {
                indent() << "_______________" << std::endl;
                indent() << std::endl;
                indent() << "creating parser {" << parserName << "}" << std::endl;
            }
            parser = new Syntax::Parser(parserName);
            topLevel( &dynamo );
            modules.free();
            
            if(verbose)
            {
                indent() << std::endl;
                indent() << "internals : " << internals << std::endl;
                indent() << "terminals : " << terminals << std::endl;
                indent() << "_______________" << std::endl << std::endl;
            }

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

        int   DynamoGenerator:: getRuleKind( const Node &node ) const
        {
            // a little sanity check
            assert(parser.is_valid());
            assert(node.internal);
            assert(node.children.size==2);
            assert(node.children.tail);
            const string &kind_ID = node.children.tail->rule.name;
            if(verbose) { indent() << "\\_[" << kind_ID << "]" << std::endl; }
            const int h = hsyn(kind_ID);
            return h;
        }

        void  DynamoGenerator:: newSymbol( Symbols &target, const string &name)
        {
            assert(parser.is_valid());
            if(modules.size()<=0)
            {
                throw exception("{%s} no parsed dynamo module before '%s'", **(parser->name), *name);
            }
            const Symbol::Pointer p = new Symbol(name,modules.peek());
            if(!target.insert(p))
            {
                throw exception("{%s} multiple symbol '%s'",**(parser->name), *name);
            }
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
            newTerminal(label);
        }


        string DynamoGenerator:: getCommentLabel()
        {
            assert(parser.is_valid());
            ++icom;
            return *(parser->name) + vformat(".comment#%d",icom);
        }


    }

}
