#include "y/program.hpp"
#include "y/ios/icstream.hpp"
#include "y/ptr/arc.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/tokenizer.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

class swap
{
public:
    const unsigned I;
    const unsigned J;
    inline  swap(unsigned i,unsigned j) throw() : I(i), J(j) { assert(I!=J); }
    inline ~swap() throw() {}
    inline  swap(const swap &other) throw() : I(other.I), J(other.J) {}

private:
    Y_DISABLE_ASSIGN(swap);
};


class swaps : public counted_object
{
public:
    unsigned     count;
    vector<swap> tests;
    template <char CH>
    static inline bool is_char(const int C) { return CH == C; }

    inline swaps(const string &id, const string &dd ) :
    tests(256,as_capacity)
    {
        // get tableau count
        {
            tokenizer<char> tkn(id);
            if( !tkn.next( is_char<'-'> ) )
            {
                throw exception("Missing Count in swaps title");
            }
            const string cnt = tkn.to_string();
            count = unsigned( string_convert::to<size_t>(cnt,"count") );
            std::cerr << "count=" << count << std::endl;
        }

        {
            string       data = dd;
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
            while( tkn.next(is_char<':'>) )
            {
                string swp = tkn.to_string();
                assert(swp.size()>2);
                assert(swp.front()=='[');
                assert(swp.back()==']');
                swp.trim(1).skip(1);
                tokenizer<char> sub(swp);
                if( !sub.next(is_char<','>) )
                {
                    throw exception("missing I");
                }
                const string I = sub.to_string();
                if( !sub.next(is_char<','>) )
                {
                    throw exception("missing J");
                }
                const string J = sub.to_string();
                const swap   s(unsigned(string_convert::to<size_t>(I,"I")),
                               unsigned(string_convert::to<size_t>(J,"J")));

                //std::cerr << s.I << "-" << s.J << "/";
                tests.push_back(s);
            }
            //std::cerr << std::endl;
        }
        std::cerr << "#tests=" << tests.size() << std::endl;
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
            if(Swaps.size()<=0 || p->count>Swaps.back()->count)
            {
                Swaps.push_back(p);
            }
        }
        std::cerr << "Found " << Swaps.size() << " swaps" << std::endl;

    }
}
Y_PROGRAM_END()

