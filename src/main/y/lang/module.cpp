#include "y/lang/module.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/imstream.hpp"

namespace upsylon
{
    namespace Lang
    {
        Module:: ~Module() throw() {}


        Module:: Module( const Tag &org, const Input &inp, const Type t ) :
        CharInfo(org,1,1),
        input(inp),
        type(t)
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
                Char *ch = new Char(C,*this);
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
            const Tag    org = new string("STDIN");
            const Input  inp = new ios::icstream( ios::cstdin );
            return new Module(org,inp,FromSTDIN);
        }

        Module * Module:: OpenFile( const string &filename )
        {
            const Tag    org = new string(filename);
            const Input  inp = new ios::icstream(filename);
            return new Module(org,inp,FromFile);
        }

        Module * Module:: OpenFile(const char   *filename) { const string _(filename); return OpenFile(_); }

        Module * Module:: OpenData( const string &name, const void *data, const size_t size)
        {
            const Tag    org = new string(name);
            const Input  inp = new ios::imstream( data, size );
            return new Module(org,inp,FromData);
        }

        Module * Module:: OpenData( const char *name, const void *data, const size_t size)
        {
            const string _(name); return OpenData(_,data,size);
        }

        Module * Module:: OpenData(const string &name, const memory::ro_buffer &buff)
        {
            return OpenData(name, (char *)buff.ro(), buff.length() );
        }

        Module * Module:: OpenData(const char *name, const memory::ro_buffer &buff)
        {
            return OpenData(name, (char *)buff.ro(), buff.length() );
        }
        
        Module * Module:: OpenResource(const ios::rc::loader &resources,
                                       const string          &identifier)
        {
            const Tag   org = new string(identifier);
            const Input inp = resources.load_stream(identifier);
            return new Module(org,inp,FromData);
        }
        
        Module * Module:: OpenResource(const ios::rc::loader &resources,
                                       const char            *identifier)
        {
            const string _(identifier);
            return OpenResource(resources,_);
        }
    }
}
