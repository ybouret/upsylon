
#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"
#include <cstdarg>

namespace upsylon
{

    namespace Lang
    {


        DynamoGenerator:: Exception:: ~Exception() throw()
        {
        }

        DynamoGenerator:: Exception:: Exception( const Exception &other ) throw() :
        exception(other),
        _what()
        {
            memcpy(_what,other._what,sizeof(_what));
        }

        const char * DynamoGenerator:: Exception:: what() const throw()
        {
            return _what;
        }

        DynamoGenerator:: Exception:: Exception( const char *fn, const char *fmt, ... ) throw() :
        exception(),
        _what()
        {
            static const size_t wmx   = sizeof(_what)-1;

            static const char   pfx[] = "Dynamo.";
            static const size_t len   = sizeof(pfx)-1;

            memset(_what,0,sizeof(_what));
            
            size_t i=0;
            while(i<=wmx&&i<len)
            {
                _what[i] = pfx[i];
                ++i;
            }

            {
                const size_t len = length_of(fn);
                for(size_t j=0;j<len;++j)
                {
                    if(i>wmx) break;
                    _what[i++] = fn[j];
                }
            }


            va_list ap;
            va_start(ap,fmt);
            format(fmt,&ap);
        }



#if 0

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
            "OPT",
            "ID",

            "RX",
            "RS",
            "^"
        };
#endif

        const char * DynamoGenerator:: ktop[] =
        {
            "RULE",
            "ALIAS",
            "LXR",
            "PLUGIN",
            "dynamo"
        };


        
        DynamoGenerator:: DynamoGenerator():
        parser(0),
        verbose(false),
        level(0),
        htop( YOCTO_MPERF_FOR(ktop) )
        {

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


        Syntax::Parser * DynamoGenerator:: create( Node &dynamo, const bool verbose_flag)
        {
            static const char fn[] = "create";
            //__________________________________________________________________
            //
            // initialize internal data
            //__________________________________________________________________
            parser  = 0;
            verbose = verbose_flag;
            level   = 0;
            modules.free();

            if(verbose)
            {
                indent() << "Dynamo." << fn << std::endl;
            }

            //__________________________________________________________________
            //
            // create an empty parser
            //__________________________________________________________________
            const string parserName = getModuleName(dynamo);
            if(verbose)
            {
                indent() << "creating parser {" << parserName << "}" << std::endl;
            }
            parser = new Syntax::Parser( parserName );
            topLevel(dynamo);

            return parser.yield();
        }

        string DynamoGenerator:: getModuleName(const Node &dynamo) const
        {
            static const char fn[] = "getModuleName";
            Y_DYNAMO_CHECK(dynamo.internal);
            Y_DYNAMO_CHECK(dynamo.children.size>1);
            Y_DYNAMO_CHECK(dynamo.children.head);
            Y_DYNAMO_CHECK(dynamo.children.head->terminal);
            Y_DYNAMO_CHECK(dynamo.children.head->rule.name=="Module");
            return dynamo.children.head->lexeme.to_string(1,0);
        }

#if 0
        void DynamoGenerator:: enterModule( const Node &dynamo )
        {
            static const char fn[] = "enterModule";
            Y_DYNAMO_CHECK(dynamo.internal);
            Y_DYNAMO_CHECK(dynamo.children.size>1);
            Y_DYNAMO_CHECK(dynamo.children.head);
            Y_DYNAMO_CHECK(dynamo.children.head->terminal);
            Y_DYNAMO_CHECK(dynamo.children.head->rule.name=="Module");
            const string id = dynamo.children.head->lexeme.to_string(1,0);
            ++level;
            if(verbose)
            {
                indent() << "entering [" << id << "]" << std::endl;
            }
            const Origin org = new string(id);
            modules.push(org);

        }

        void DynamoGenerator:: leaveModule() throw()
        {
            assert(modules.size()>0);
            if(verbose)
            {
                indent() << "leaving [" << modules.peek() << "]" << std::endl;
            }
            --level;
            modules.pop();
        }
#endif

#if 0
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
            assert(0==level);
            if(verbose)
            {
                indent() << std::endl;
                indent() << "Setup " << parserName << std::endl;
            }
            walkDown(&dynamo);

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

#endif
        
    }

}
