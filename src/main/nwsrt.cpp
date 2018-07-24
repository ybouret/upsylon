#include "y/program.hpp"
#include "y/ios/icstream.hpp"
#include "y/ptr/arc.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/tokenizer.hpp"

using namespace upsylon;

class swaps : public counted_object
{
public:
    const string title;
    string       data;
    static inline bool is_sep(const int C) { return ':' == C; }

    inline swaps(const string &id, const string &dd ) :
    title(id), data(dd)
    {
        data.trim(1).skip(1);
        const string &source = data;
        {
            string        target;
            for(size_t i=0;i<source.size();++i)
            {
                if( ',' == source[i] && ' '==source[i+1] )
                {
                    target<< ':';
                    ++i;
                }
                else
                {
                    target << source[i];
                }
            }
            data.swap_with(target);
        }
        tokenizer<char> tkn(data);
        while( tkn.next(is_sep) )
        {
            const string swp = tkn.to_string();
            std::cerr << swp << "/";
        }
        std::cerr << std::endl;
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

