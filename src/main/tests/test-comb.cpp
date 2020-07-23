#include "y/counting/comb.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;


Y_UTEST(comb)
{

    for(int iarg=1;iarg<argc;++iarg)
    {
        const size_t n = string_convert::to<size_t>(argv[1],"n");

        for(size_t k=1;k<=n;++k)
        {
            combination comb(n,k);
            std::cerr << "comb(" << n << "," << k << ")=" << comb.count << std::endl;

            vector<combination> Comb(comb.count,as_capacity);

            for( comb.boot(); comb.good(); comb.next() )
            {
                std::cerr << "\t" << (counting &)comb;
                std::cerr << " (";
                for(size_t i=0;i<k;++i)
                {
                    std::cerr << ' ' << comb(i);
                }
                std::cerr << " )";
                std::cerr << std::endl;
                Comb.push_back_(comb);
            }
            std::cerr << "comb(" << n << "," << k << ")=" << comb.count << std::endl;
            std::cerr << "checking..." << std::endl;
            for(size_t i=1;i<=Comb.size();++i)
            {
                for(size_t j=i;j<=Comb.size();++j)
                {
                    combination::memchk(Comb[i],Comb[j]);
                }
            }
        }
    }
}
Y_UTEST_DONE()


#include "y/associative/suffix/tree.hpp"
#include "y/sort/groups.hpp"
Y_UTEST(combinations)
{

    suffix_tree<bool> db;

    if(argc>1)
    {
        string       source = argv[1];
        const size_t n      = source.size();


        vector<size_t> groups;
        {
            lightweight_array<char> mock(*source,n);
            hsort(mock,comparison::increasing<char>);
            find_groups::guess(groups,*source,*source+n);
        }
        std::cerr << "<" << source << "> groups: " << groups << std::endl;



        for(size_t k=1;k<=n;++k)
        {
            std::cerr << "(" << n << "," << k << ")" << std::endl;
            combination C(n,k);
            db.free_all();
            size_t standard = 0;
            size_t univocal = 0;
            for(C.boot();C.good();C.next())
            {
                string choice(k,as_capacity,true);
                for(size_t i=0;i<k;++i)
                {
                    choice[i] = source[ C(i) ];
                }
                std::cerr << "\t" << choice << std::endl;
                ++standard;
                if( db.insert_by(choice,true) ) ++univocal;
                
            }
            std::cerr << "\t#standard=" << standard << std::endl;
            std::cerr << "\t#univocal=" << univocal << std::endl;
        }

    }
}
Y_UTEST_DONE()

