
#include "y/associative/ledger.hpp"
#include "y/type/spec.hpp"
#include "y/sequence/list.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/os/xbe-address.hpp"

using namespace upsylon;

namespace {
 
    template <typename T>
    static inline int compare_stub( const BEaddress &lhs, const BEaddress &rhs)
    {
        return comparison::increasing(*lhs.as<T>(), *rhs.as<T>() );
    }
    
    template <typename T> static inline
    void doTest()
    {
        const string &tid = type_name_of<T>();
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
        
        const string fn = "ledger_" + tid + ".dot";
        std::cerr << "\t"; L.get_root().graphViz(fn);
        
        std::cerr << "\tsearching..." << std::endl;
        for(size_t i=source.size();i>0;--i)
        {
            Y_ASSERT( L.search( source[i]) );
        }
        
        L.release_all();
        
        std::cerr << "\tsecuring..." << std::endl;
        for(size_t i=source.size();i>0;--i)
        {
            L.secure( source[i] );
            Y_ASSERT( L.search( source[i]) );
        }
        
        std::cerr << "\tsorting" << std::endl;
        L.sort_with( compare_stub<T> );
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
    }
    
    
}

Y_UTEST(ledger)
{
    doTest<int>();
    doTest<string>();
    doTest<float>();
    
    const string hello = "hello";
    XBEaddress<int> xbe_i(hello,1);
    std::cerr << * xbe_i.as<string>() << "/" << xbe_i.data.attr << std::endl;
    
}
Y_UTEST_DONE()

