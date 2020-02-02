#include "y/lang/pattern/compiler.hpp"
#include "y/lang/pattern/dictionary.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/imstream.hpp"

#include "y/codec/delta.hpp"
#include "y/codec/mtf.hpp"

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
        if(p->weak())
        {
            std::cerr << "\t|_WEAK" << std::endl;
        }
        else
        {
            std::cerr << "\t|_FULL" << std::endl;
        }
        if(p->univocal())
        {
            std::cerr << "\t|_UNIVOCAL" << std::endl;
        }
        else
        {
            std::cerr << "\t|_AMBIGUOUS" << std::endl;
        }

        {
            const string bin = p->to_binary();
            Y_CHECK( bin.size() == p->serialize_length() );
            {
                ios::imstream fp(bin);
                auto_ptr<Pattern> q = Pattern::Load(fp);
                const string loaded = q->to_binary();
                Y_CHECK(loaded==bin);
            }



            {
                delta_encoding enc;
                string         binEnc = bin;
                enc.encode_block( *binEnc, *bin, bin.size() );
                binEnc.save_to("bin_del.bin");
            }

            {
                move_to_front  enc;
                string         binEnc = bin;
                enc.encode_block( *binEnc, *bin, bin.size() );
                binEnc.save_to("bin_mtf.bin");
            }

        }

        p->graphViz("regex.dot");

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

