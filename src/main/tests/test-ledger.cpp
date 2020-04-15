
#include "y/associative/ledger.hpp"
#include "y/type/spec.hpp"
#include "y/sequence/list.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;

namespace {
 
    template <typename T> static inline
    void doTest()
    {
        const string &tid = type_name_of<T>();
        std::cerr << "ledger_of<" << tid << ">" << std::endl;
        
        std::cerr << "\tcreating..." << std::endl;
        list<T> source;
        for(size_t i=2+alea.leq(10);i>0;--i)
        {
            const T temp = support::get<T>();
            source.push_back(temp);
        }
        
        std::cerr << "\tinserting..." << std::endl;
        ledger_of<T> L;
        for(size_t i=source.size();i>0;--i)
        {
            Y_ASSERT( L.insert( source[i]) );
        }
        
        const string fn = "ledger_" + tid + ".dot";
        L.get_root().graphViz(fn);
        
        std::cerr << "\tsearching..." << std::endl;
        for(size_t i=source.size();i>0;--i)
        {
            Y_ASSERT( L.search( source[i]) );
        }
    }
    
    
}

Y_UTEST(ledger)
{
    doTest<int>();
    doTest<string>();

}
Y_UTEST_DONE()

