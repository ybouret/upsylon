#include "y/sequence/maintain.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"

using namespace upsylon;

static size_t sz = 0;

bool keep_short_string( const string &s ) throw()
{
    return s.size() <= sz;
}

template <typename SEQUENCE>
static inline
void do_maintain( SEQUENCE &strings )
{
    strings.free();
    {
        const size_t n = 1 + alea.leq(100);
        std::cerr << "Generating " << n << " strings" << std::endl;
        sz=0;
        for(size_t i=n;i>0;--i)
        {
            const string s = support::get<string>();
            strings.push_back(s);
            sz += strings.back().size();
        }
        sz/=n;
        std::cerr << "strings  =" << strings << std::endl;

        list<size_t> indices;
        maintain::build_indices(indices, strings.begin(), strings.size(), keep_short_string, false);
        std::cerr << "indices  =" << indices << std::endl;
        maintain::strip(strings,indices);
        std::cerr << "stripped =" << strings << std::endl;
    }

    {
        const size_t n = 1 + alea.leq(100);
        std::cerr << "Generating " << n << " strings" << std::endl;
        sz=0;
        for(size_t i=n;i>0;--i)
        {
            const string s = support::get<string>();
            strings.push_back(s);
            sz += strings.back().size();
        }
        sz/=n;
        std::cerr << "strings  =" << strings << std::endl;

        vector<size_t> indices;
        maintain::build_indices(indices, strings.begin(), strings.size(), keep_short_string, true);
        std::cerr << "indices  =" << indices << std::endl;
        maintain::strip(strings,indices);
        std::cerr << "stripped =" << strings << std::endl;
    }

}

Y_UTEST(maintain)
{
    for(size_t iter=256;iter>0;--iter)
    {
        {
            vector<string,memory::global> strings;
            do_maintain(strings);
        }

        {
            vector<string,memory::pooled> strings;
            do_maintain(strings);
        }

        {
            list<string> strings;
            do_maintain(strings);
        }
    }
}
Y_UTEST_DONE()

