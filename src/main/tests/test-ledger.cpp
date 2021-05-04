
#include "y/associative/ledger.hpp"
#include "y/type/rtti.hpp"
#include "y/sequence/list.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;

namespace {
 
    template <typename T>
    static inline int compare_stub( const BEaddress &lhs, const BEaddress &rhs)
    {
        return comparison::increasing(*lhs.as<T>(), *rhs.as<T>() );
    }
    
    template <typename T> static inline
    void doTest(const char *name)
    {
        const string &tid = rtti::name_of<T>();
        std::cerr << "ledger_of<" << tid << ">" << std::endl;
        
        std::cerr << "\tcreating..." << std::endl;
        list<T> source;
        for(size_t i=2+alea.leq(20);i>0;--i)
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
        
        const string nn = name;
        const string fn = "ledger_" + nn + ".dot";
        std::cerr << "\t"; L.get_root().graphViz(fn);
        
        std::cerr << "\tsearching..." << std::endl;
        for(size_t i=source.size();i>0;--i)
        {
            Y_ASSERT( L.search( source[i]) );
        }
        
        L.release();
        
        std::cerr << "\tsecuring..." << std::endl;
        for(size_t i=source.size();i>0;--i)
        {
            L.secure( source[i] );
            Y_ASSERT( L.search( source[i]) );
        }
        
        std::cerr << "\tsorting" << std::endl;
        std::cerr << "\t-- unsorted:" << std::endl;
        for( typename ledger_of<T>::iterator it=L.begin();it!=L.end();++it)
        {
            const BEaddress &addr = *it;
            std::cerr << "\t\t" << *(addr.as<T>()) << std::endl;
        }
        L.sort_with( compare_stub<T> );
        std::cerr << "\t-- sorted  :" << std::endl;
        for( typename ledger_of<T>::iterator it=L.begin();it!=L.end();++it)
        {
            const BEaddress &addr = *it;
            std::cerr << "\t\t" << *(addr.as<T>()) << std::endl;
        }
        
        std::cerr << "\tremoving..." << std::endl;
        for(size_t i=source.size();i>0;--i)
        {
            L.remove(source[i]);
            Y_ASSERT( !L.search( source[i]) );
        }
        std::cerr << "\tdone" << std::endl;
    }
    
    
}

Y_UTEST(ledger)
{
    doTest<int>("int");
    doTest<string>("string");
    doTest<float>("float");


    
}
Y_UTEST_DONE()

