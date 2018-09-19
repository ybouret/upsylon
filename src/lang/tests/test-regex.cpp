#include "y/lang/pattern/compiler.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(regex)
{
    Dictionary dict;
    dict("INT",Compile::RegExp("[:digit:]+"));
    
    if(argc>1)
    {
        const string rx = argv[1];
        std::cerr << "Compiling '" << rx << "'" << std::endl;
        auto_ptr<Pattern> p = Compile::RegExp(rx,&dict);
        p->GraphViz("regex.dot");
        if(argc>2&&0==strcmp(argv[2],"scan"))
        {

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

