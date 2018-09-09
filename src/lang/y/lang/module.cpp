#include "y/lang/module.hpp"
#include "y/ios/icstream.hpp"

namespace upsylon
{
    namespace Lang
    {
        Module:: ~Module() throw() {}


        Module:: Module( const Origin &org, const Input &inp ) :
        CharInfo(org,1,1),
        input(inp),
        chMgr( Manager::instance() )
        {
        }


        void Module:: newLine() throw()
        {
            ++((int &)line);
            (int&)column = 1;
        }

        Char * Module:: get()
        {
            char C = 0;
            if(input->query(C))
            {
                ++( (int&)column );
                return chMgr.make(C,*this);
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

    }
}
