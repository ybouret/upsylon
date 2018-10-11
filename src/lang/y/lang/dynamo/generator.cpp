
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



        const char * DynamoGenerator:: ktop[] =
        {
            "RULE",
            "ALIAS",
            "LXR",
            "PLUGIN",
            "dynamo"
        };

        const char * DynamoGenerator:: kstr[] =
        {
            "RX",
            "RS",
            "OS"
        };

        const char * DynamoGenerator:: klxr[] =
        {
            "drop",
            "endl",
            "comment"
        };

        const char * DynamoGenerator:: ksyn[] =
        {
            "AGG",
            "ALT",
            "OOM",
            "OPT",
            "ZOM",
            "ID",
            "RS",
            "OS",
            "RX"
        };

#define Y_DYNAMO_SYN(LABEL) assert( hsyn(#LABEL)==IS_##LABEL )
        DynamoGenerator:: DynamoGenerator():
        parser(0),
        verbose(false),
        level(0),
        modules(),
        htop( YOCTO_MPERF_FOR(ktop) ),
        hstr( YOCTO_MPERF_FOR(kstr) ),
        hlxr( YOCTO_MPERF_FOR(klxr) ),
        hsyn( YOCTO_MPERF_FOR(ksyn) ),
        terminals(),
        internals()
        {
            Y_DYNAMO_SYN(AGG);
            Y_DYNAMO_SYN(ALT);
            Y_DYNAMO_SYN(OOM);
            Y_DYNAMO_SYN(OPT);
            Y_DYNAMO_SYN(ZOM);
            Y_DYNAMO_SYN(ID);
            Y_DYNAMO_SYN(RS);
            Y_DYNAMO_SYN(OS);
            Y_DYNAMO_SYN(RX);
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
            internals.free();
            terminals.free();
            
            if(verbose) { indent() << "Dynamo." << fn << std::endl; }

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
            parser->root.verbose = verbose;
            
            //__________________________________________________________________
            //
            // create top level structure
            //__________________________________________________________________
            topLevel(dynamo);
            dynamo.GraphViz("dynout.dot");

            //__________________________________________________________________
            //
            // linking 
            //__________________________________________________________________
            walkDown(dynamo);
            
            if(verbose) {
                indent() << "____________________________________" << std::endl;
                indent() << std::endl;
                indent() << " (*) visible terminals=" << std::endl;

                for( Terminals::iterator i=terminals.begin();i!=terminals.end();++i)
                {
                    const _Terminal &t = **i;
                    if(!t.visible) continue;
                    indent() << "                      |_<" << t << ">" << std::endl;
                }
                indent() << " (*) hidden  terminals=" << std::endl;
                for( Terminals::iterator i=terminals.begin();i!=terminals.end();++i)
                {
                    const _Terminal &t = **i;
                    if(t.visible) continue;
                    indent() << "                      |_<" << t << ">" << std::endl;
                }
                indent() << " (*) parsed  internals=" << std::endl;
                for( Internals::iterator i=internals.begin();i!=internals.end();++i)
                {
                    const _Internal &c = **i;
                    indent() << "                      |_<" << c << ">" << std::endl;
                }
                indent() << "____________________________________" << std::endl;
            }

            //__________________________________________________________________
            //
            // checking if normal compound are in fact acting/design
            //__________________________________________________________________
            finalize();

         

            //__________________________________________________________________
            //
            // checking validity
            //__________________________________________________________________
            if(verbose) { indent() << "checking validity of {" << parserName << "}" << std::endl; }
            parser->checkValidity();

            return parser.yield();
        }

        string DynamoGenerator:: getModuleName(const Node &dynamo) const
        {
            static const char fn[] = "getModuleName";
            if(verbose) { indent() << "Dynamo." << fn << std::endl; }
            Y_DYNAMO_CHECK(dynamo.internal);
            Y_DYNAMO_CHECK(dynamo.children.size>1);
            Y_DYNAMO_CHECK(dynamo.children.head);
            Y_DYNAMO_CHECK(dynamo.children.head->terminal);
            Y_DYNAMO_CHECK(dynamo.children.head->rule.name=="Module");
            return dynamo.children.head->lexeme.to_string(1,0);
        }
        
        
    }

}
