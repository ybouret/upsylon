
#include "y/container/query.hpp"

#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/associative/map.hpp"

#include "y/utest/run.hpp"
#include "y/string.hpp"

#include "y/ios/icstream.hpp"

using namespace upsylon;

#define NN (32*1024)

namespace {

    static inline bool query4( const string &s )
    {
        return s.size() == 4;
    }

}

Y_UTEST(query)
{
    vector<string>     vs(NN,as_capacity);
    list<string>       ls(NN,as_capacity);
    map<string,string> ms(NN,as_capacity);

    if(argc>1)
    {
        std::cerr << "Loading " << argv[1] << "..." << std::endl;
        ios::icstream fp( argv[1] );
        string line;
        while( fp.gets(line) )
        {
            vs << line;
            ls << line;
            (void) ms.insert(line,line);
        }

        {
            string *pS = container_query::first(vs,query4);
            if(pS)
            {
                std::cerr << "vs.first.query4 =  " << *pS << std::endl;
            }
            pS = container_query::first_reverse(vs,query4);
            if(pS)
            {
                std::cerr << "vs.rfirst.query4 = " << *pS << std::endl;
            }
        }

        {
            const vector<string> &cvs = vs;
            const string *pS = container_query::first(cvs,query4);
            if(pS)
            {
                std::cerr << "cvs.first.query4 =  " << *pS << std::endl;
            }
            pS = container_query::first_reverse(cvs,query4);
            if(pS)
            {
                std::cerr << "cvs.rfirst.query4 = " << *pS << std::endl;
            }
        }



        {
            string *pS = container_query::first(ls,query4);
            if(pS)
            {
                std::cerr << "vs.first.query4 =  " << *pS << std::endl;
            }
            pS = container_query::first_reverse(ls,query4);
            if(pS)
            {
                std::cerr << "vs.rfirst.query4 = " << *pS << std::endl;
            }
        }

        {
            string *pS = container_query::first(ms,query4);
            if(pS)
            {
                std::cerr << "ms.first.query4 =  " << *pS << std::endl;
            }
        }


        std::cerr << "done..." << std::endl;
    }

}
Y_UTEST_DONE()
