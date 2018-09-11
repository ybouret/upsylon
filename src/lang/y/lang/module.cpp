#include "y/lang/module.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/imstream.hpp"

namespace upsylon
{
    namespace Lang
    {
        Module:: ~Module() throw() {}


        Module:: Module( const Origin &org, const Input &inp ) :
        CharInfo(org,1,1),
        input(inp),
        cache( Manager::instance() )
        {
        }


        void Module:: newLine() const throw()
        {
            ++((int &)line);
            (int&)column = 1;
        }

        Char * Module:: get()
        {
            char C = 0;
            if(input->query(C))
            {
                Char *ch = cache.make(C,*this);
                ++( (int&)column );
                return ch;
            }
            else
            {
                return NULL;
            }
        }

        Module * Module:: OpenSTDIN()
        {
            const Origin org = new string("STDIN");
            const Input  inp = new ios::icstream( ios::cstdin );
            return new Module(org,inp);
        }

        Module * Module:: OpenFile( const string &filename )
        {
            const Origin org = new string(filename);
            const Input  inp = new ios::icstream(filename);
            return new Module(org,inp);
        }

        Module * Module:: OpenData( const string &name, const char *data, const size_t size)
        {
            const Origin org = new string(name);
            const Input  inp = new ios::imstream( data, size );
            return new Module(org,inp);
        }

    }
}
