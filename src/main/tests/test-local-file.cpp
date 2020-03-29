#include "y/ios/orstream.hpp"
#include "y/ios/irstream.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(local_file)
{
    {
        ios::offset_t header1 = 0;
        ios::offset_t header2 = 0;

        std::cerr << "Write Level-1" << std::endl;
        {
            ios::orstream fp("ors.dat");
            fp << "Hello";
            header1 = fp.tell();
            fp.flush();
        }

        std::cerr << "Write Level-2" << std::endl;
        {
            ios::orstream fp("ors.dat",true);
            fp << ",World!";
            header2 = fp.tell();
            const string phrase = "password";
            phrase.serialize(fp);
            fp.flush();
        }

        std::cerr << "Reading Level-1/5 bytes max" << std::endl;
        {
            ios::irstream fp("ors.dat",0,5);
            char   C = 0;
            while( fp.query(C) )
            {
                if(C>=' '&&C<126)
                {
                    std::cerr << C;
                }
                else
                {
                    std::cerr << ".";
                }
            }
            std::cerr << std::endl;
        }

        std::cerr << "Reading Level-2" << std::endl;
        {
            ios::irstream fp("ors.dat",0);
            char   C = 0;
            string s1;
            while( s1.size() < size_t(header1) )
            {
                if(!fp.query(C)) throw exception("Missing data level-1");
                s1 << C;
            }
            std::cerr << "s1=" << s1 << std::endl;
            string s2;
            while( s2.size() < size_t(header2-header1) )
            {
                if(!fp.query(C)) throw exception("Missing data level-1");
                s2 << C;
            }
            std::cerr << "s2=" << s2 << std::endl;
            size_t length = 0;
            string phrase = string::read(fp,length,"phrase");
            std::cerr << "phrase=" << phrase << std::endl;
            Y_CHECK(phrase.size()==length);
        }
    }

    if( argc>1 && strcmp("run",argv[1]) == 0 )
    {
        ios::irstream fp( ios::cstdin );
        string        line;
        while( (std::cerr << "> ").flush(), fp.gets(line) )
        {
            std::cerr << "<" << line << ">" << std::endl;
        }
    }
}
Y_UTEST_DONE()

