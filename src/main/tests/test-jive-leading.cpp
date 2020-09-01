#include "y/jive/pattern/leading.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_leading)
{

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

    L.release();


    L.insert('a','c');std::cerr << L << std::endl;
    L.remove('a');    std::cerr << L << std::endl;
    L.insert('a','c');std::cerr << L << std::endl;
    L.remove('c');    std::cerr << L << std::endl;
    L.insert('a','c');std::cerr << L << std::endl;
    L.remove('b');    std::cerr << L << std::endl;

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
            Y_ASSERT(L.search(ch[i]));
        }
        std::cerr << "L=" << L << std::endl;
        alea.shuffle(ch,256);
        for(int i=0;i<256;++i)
        {
            Y_ASSERT(L.remove(ch[i]));
            Y_ASSERT(!L.search(ch[i]));
        }
        Y_ASSERT(0==L.size);
    }

    Y_UTEST_SIZEOF(Interval);
    Y_UTEST_SIZEOF(Leading);
    Y_UTEST_SIZEOF(string);
    
}
Y_UTEST_DONE()

