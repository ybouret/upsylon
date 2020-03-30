#include "y/ios/upacker.hpp"
#include "y/type/spec.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace ios;


template <typename T>
static inline void doEncode( uint8_t *buffer, T value )
{
    size_t extra = 0;
    *(buffer++) = upacker::encode::init(value,extra);
    Y_ASSERT(extra<=sizeof(T));
    while(extra-- > 0)
    {
        *(buffer++) = upacker::encode::next(value);
    }
    Y_ASSERT(value==0);
}

template <typename T>
static inline T doDecode( const uint8_t *buffer )
{
    T      value = 0;
    size_t shl   = 0;
    size_t extra = upacker::decode::init(value,*(buffer++),shl);

    Y_ASSERT(extra<=sizeof(T));
    while(extra-- > 0)
    {
        upacker::decode::next(value,*(buffer++),shl);
    }
    return value;
    
}


template <typename T>
static inline void doUpack()
{
    std::cerr << "upack<" << type_name_of<T>() << ">" << std::endl;
    static const T direct_value_max = upacker::direct_mask;
    std::cerr << "|_direct_value_max=" << uint64_t(direct_value_max) << std::endl;
    
    uint8_t buffer[1+sizeof(T)];
    
    
    
    std::cerr << "|_testing direct values" << std::endl;
    for(T i=0;i<=direct_value_max;++i)
    {
        doEncode(buffer,i);
        Y_ASSERT(doDecode<T>(buffer)==i);
    }
    
    std::cerr << "|_testing maximum value" << std::endl;
    {
        const T value = limit_of<T>::maximum;
        doEncode(buffer,value);
        Y_CHECK(doDecode<T>(buffer) == limit_of<T>::maximum );
    }
    
    std::cerr << "|_testing random  values" << std::endl;
    {
        for(size_t i=0;i<1024*1024;++i)
        {
            const T value = alea.full<T>();
            doEncode(buffer,value);
            const T temp = doDecode<T>(buffer);
            if(value!=temp)
            {
                std::cerr << "error " << uint64_t(value) << " => " << uint64_t(temp) << std::endl;
                throw exception("decode mismatch for %s", *type_name_of<T>());
            }
        }
    }
    
    
    
    
    
}

Y_UTEST(upacker)
{
    doUpack<uint8_t>();
    doUpack<uint16_t>();
    doUpack<uint32_t>();
    doUpack<uint64_t>();
}
Y_UTEST_DONE()

