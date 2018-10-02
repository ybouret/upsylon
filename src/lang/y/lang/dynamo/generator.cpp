
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
            "OS"
        };

#define Y_DYNAMO_SYN(LABEL) assert( hsyn(#LABEL)==IS_##LABEL )
        DynamoGenerator:: DynamoGenerator():
        parser(0),
        verbose(false),
        level(0),
        htop( YOCTO_MPERF_FOR(ktop) ),
        hstr( YOCTO_MPERF_FOR(kstr) ),
        hlxr( YOCTO_MPERF_FOR(klxr) ),
        hsyn( YOCTO_MPERF_FOR(ksyn) )
        {
            Y_DYNAMO_SYN(AGG);
            Y_DYNAMO_SYN(ALT);
            Y_DYNAMO_SYN(OOM);
            Y_DYNAMO_SYN(OPT);
            Y_DYNAMO_SYN(ZOM);
            Y_DYNAMO_SYN(ID);
            Y_DYNAMO_SYN(RS);
            Y_DYNAMO_SYN(OS);
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

            if(verbose) { indent() << "terminals=" << terminals << std::endl; }
            if(verbose) { indent() << "internals=" << internals << std::endl; }

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
        
        
    }

}
