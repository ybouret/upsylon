#include "y/lang/pattern/compiler.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(regex)
{
    Dictionary dict;
    dict("INT","[:digit:]+");
    
    if(argc>1)
    {
        const string rx = argv[1];
        std::cerr << "Compiling '" << rx << "'" << std::endl;
        auto_ptr<Pattern> p = RegExp(rx,&dict);
        p->GraphViz("regex.dot");
        if(p->weak())
        {
            std::cerr << "this is a weak regular expression" << std::endl;
        }
        else
        {
            std::cerr << "this is a full regular expression" << std::endl;
        }
        if(argc>2&&0==strcmp(argv[2],"scan"))
        {

            if(p->weak())
            {
                throw exception("I do not accept weak patterns for this test!");
            }
            
            string        line;
            ios::icstream fp( ios::cstdin );
            while( std::cerr << "> ", fp.gets(line) )
            {
                Source src( Module::OpenData("line",line) );
                while( src.active() )
                {
                    Token tkn;
                    if(p->match(tkn,src))
                    {
                        std::cerr << "'" << tkn << "'";

                    }
                    else
                    {
                        std::cerr << char(src.peek()->code);
                        src.skip();
                    }
                }
                std::cerr << std::endl;
            }

        }
    }

    std::cerr << "sizeof(Dictionary)=" << sizeof(Dictionary) << std::endl;

}
Y_UTEST_DONE()

