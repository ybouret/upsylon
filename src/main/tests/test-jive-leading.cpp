#include "y/jive/pattern/leading.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_leading)
{
    Y_UTEST_SIZEOF(Interval);
    
    Leading L;
    std::cerr << "L=" << L << std::endl;
    L.insert('c');
    std::cerr << "L=" << L << std::endl;
    L.insert('b');
    std::cerr << "L=" << L << std::endl;
    L.insert('a');
    std::cerr << "L=" << L << std::endl;
    
    L.insert('z');
    std::cerr << "L=" << L << std::endl;

    L.insert('0');
    std::cerr << "L=" << L << std::endl;

    L.insert('k');
    std::cerr << "L=" << L << std::endl;

    L.insert('m');
    std::cerr << "L=" << L << std::endl;
    
    L.insert('l');
    std::cerr << "L=" << L << std::endl;
    
    for(int i='0';i<='z';++i)
    {
        L.insert(uint8_t(i));
    }
    std::cerr << "L=" << L << std::endl;
    
    L.release();
    std::cerr << "L=" << L << std::endl;
    L.complete();
    std::cerr << "L=" << L << std::endl;
   
    for(size_t iter=0;iter<8;++iter)
    {
        L.release();
        uint8_t ch[256];
        for(int i=0;i<256;++i)
        {
            ch[i] = uint8_t(i);
        }
        alea.shuffle(ch,256);
        for(int i=0;i<256;++i)
        {
            Y_ASSERT(L.insert(ch[i]));
        }
        std::cerr << "L=" << L << std::endl;
        alea.shuffle(ch,256);
        for(int i=0;i<256;++i)
        {
            Y_ASSERT(L.remove(ch[i]));
        }
    }
    
    
}
Y_UTEST_DONE()

