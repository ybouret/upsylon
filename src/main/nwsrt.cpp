#include "y/program.hpp"
#include "y/ios/icstream.hpp"
#include "y/ptr/arc.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

class swaps : public counted_object
{
public:
    const string title;
    const string data;

    inline swaps(const string &id, const string &dd ) :
    title(id), data(dd)
    {
    }

    virtual ~swaps() throw()
    {
    }

private:
    Y_DISABLE_COPY_AND_ASSIGN(swaps);
};

typedef arc_ptr<swaps> swaps_ptr;

Y_PROGRAM_START()
{
    if(argc>1)
    {
        ios::icstream fp(argv[1]);
        vector<swaps_ptr> Swaps(128,as_capacity);
        string line;
        while( fp.gets(line) )
        {
            string title = line;
            if(!fp.gets(line))
            {
                throw exception("Missing Data for '%s'", *title);
            }
            string data = line;
            if(!fp.gets(line))
            {
                throw exception("Cannot skip line after '%s'",*title);
            }
            std::cerr << title << std::endl;
            const swaps_ptr p = new swaps(title,data);
            Swaps.push_back(p);
        }

    }
}
Y_PROGRAM_END()

