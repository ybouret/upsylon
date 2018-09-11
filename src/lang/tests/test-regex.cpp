#include "y/lang/pattern/compiler.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(regex)
{
    Dictionary dict;
    dict("INT",Compile::RegEx("[:digit:]+"));

    if(argc>1)
    {
        const string rx = argv[1];
        std::cerr << "Compiling '" << rx << "'" << std::endl;
        auto_ptr<Pattern> p = Compile::RegEx(rx,&dict);
        p->GraphViz("regex.dot");
        if(argc>2&&0==strcmp(argv[2],"scan"))
        {

            string        line;
            ios::icstream fp( ios::cstdin );
            while( std::cerr << "> ", fp.gets(line) )
            {
                Source src( Module::OpenData("line",line) );
                char C=0;
                while( src.peek(C) )
                {
                    Token tkn;
                    if(p->match(tkn,src))
                    {
                        std::cerr << "'" << tkn << "'";
                    }
                    else
                    {
                        std::cerr << C;
                        src.skip();
                    }
                }
                std::cerr << std::endl;
            }

        }
    }


}
Y_UTEST_DONE()

