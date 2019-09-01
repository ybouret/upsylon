
#include "y/memory/static-slots.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;

template <typename SLOTS>
static inline void display_slots( SLOTS &s )
{
    const SLOTS &cs = s;
    for(size_t i=0;i<s.size;++i)
    {
        const size_t j=SLOTS::offset+i;
        std::cerr << "slot[" << j << "]=" << s[j] << "|" << cs[j] << std::endl;
    }
}

Y_UTEST(static_slots)
{
    memory::static_slots<double,10> sl;
    
    while(sl.size<sl.capacity)
    {
        sl.push_back( alea.to<double>() );
    }
    display_slots(sl);
    
    memory::static_slots<string,16,1> ss;
    while(ss.size<ss.capacity)
    {
        ss.push_back( support::get<string>() );
    }
    display_slots(ss);
    
}
Y_UTEST_DONE()

