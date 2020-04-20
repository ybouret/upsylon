
#include "y/associative/ledger.hpp"
#include "y/type/spec.hpp"
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
        
        const string nn = name;
        const string fn = "ledger_" + nn + ".dot";
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

#include "y/associative/xbe-key.hpp"
#include "y/code/utils.hpp"


namespace {

    template <typename T>
    static inline void displayXBE( const  xbe_key<T> &key )
    {
        const uint8_t *p = (uint8_t *) (key.ro());
        for(size_t i=0;i<sizeof(void*);++i)
        {
            std::cerr << " 0x" << hexadecimal::uppercase[ p[i] ];
        }
        std::cerr << " |";
        for(size_t i=sizeof(void*);i<key.length();++i)
        {
            std::cerr << " 0x" << hexadecimal::uppercase[ p[i] ];
        }
        //const uint64_t attr = uint64_t(key.xaddr.data.attr);
        //std::cerr << " = @" << key.xaddr.data.addr << " $" << uint64_t(swap_be(key.xaddr.data.attr));
        std::cerr << std::endl;
    }
    
    template <typename T>
    static inline void testXBE()
    {
        std::cerr << "xbe size=" <<  XBEaddress<T>::size << "/" << sizeof(XBEaddress<T>) << std::endl;
        const string  hello = "hello";
        xbe_key<T>    xk1(hello,1);
        const string  world = "world";
        xbe_key<T>    xk2(world,2);

        std::cerr << xk1.template as<string>() << ", " << xk2.template as<string>() << std::endl;
        displayXBE(xk1);
        displayXBE(xk2);

    }
    
}

Y_UTEST(ledger)
{
    doTest<int>("int");
    doTest<string>("string");
    doTest<float>("float");
    
    testXBE<uint8_t>();
    testXBE<uint16_t>();
    testXBE<uint32_t>();
    testXBE<uint64_t>();

    
}
Y_UTEST_DONE()

