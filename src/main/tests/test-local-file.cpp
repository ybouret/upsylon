#include "y/ios/orstream.hpp"
#include "y/ios/irstream.hpp"
#include "y/utest/run.hpp"
#include "y/string/io.hpp"

using namespace upsylon;

Y_UTEST(local_file)
{
    {
        ios::offset_t header1 = 0;
        ios::offset_t header2 = 0;

        {
            ios::orstream fp("ors.dat");
            fp << "Hello";
            header1 = fp.tell();
        }

        {
            ios::orstream fp("ors.dat",true);
            fp << ",World!";
            header2 = fp.tell();
            const string phrase = "password";
            string_io::save_binary(fp,phrase);
        }

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
            string phrase = string_io::load_binary(fp);
            std::cerr << "phrase=" << phrase << std::endl;
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

