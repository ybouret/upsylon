
#include "y/string/defrag.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "support.hpp"
#include "y/memory/pooled.hpp"

using namespace upsylon;

namespace
{
    template <typename SEQUENCE>
    static inline
    void do_defrag( string_defrag &SD, SEQUENCE &strings )
    {
        std::cerr << "defrag sequence of " << strings.size() << std::endl;
        const size_t nd = SD.cycle_on(strings);
        std::cerr << "could do " << nd << std::endl;
    }
}

Y_UTEST(string_defrag)
{
    const size_t nv = 20;
    const size_t nl = 30;
    vector<string>                vs(nv,as_capacity);
    vector<string,memory::pooled> ps(nv,as_capacity);

    list<string>   ls(nl,as_capacity);

    {
        const size_t   ns = 1000;
        vector<string> tmp(ns,as_capacity);
        for(size_t i=0;i<ns;++i)
        {
            const string s( alea.leq(100), as_capacity );
            tmp.push_back(s);
        }

        for(size_t i=0;i<nl;++i)
        {
            const string tmp = support::get<string>();
            ls.push_back(tmp);
        }

        for(size_t i=0;i<nv;++i)
        {
            const string tmp = support::get<string>();
            vs.push_back(tmp);
        }

        for(size_t i=0;i<nv;++i)
        {
            const string tmp = support::get<string>();
            ps.push_back(tmp);
        }

    }

    string_defrag SD;

    do_defrag(SD,vs);
    vs.release();

    do_defrag(SD,ls);
    ls.release();

    do_defrag(SD,ps);
    ps.release();


}
Y_UTEST_DONE()

