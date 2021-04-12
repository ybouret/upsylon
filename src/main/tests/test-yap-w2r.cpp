#include "y/yap/natural.hpp"
#include "y/utest/run.hpp"
#include "y/os/endian.hpp"
#include "y/code/utils.hpp"

using namespace upsylon;


namespace {

    template <typename WORD>
    static inline void show(const WORD w)
    {
        std::cerr << std::endl;
        const union
        {
            WORD    w;
            uint8_t b[sizeof(WORD)];
        } alias = { w };

        std::cerr << std::hex;
        std::cerr << "w=" << uint64_t(w) << std::endl;
        std::cerr << std::dec;
        for(unsigned i=0;i<sizeof(WORD);++i)
        {
            std::cerr << " | w[" << i << "]=" << hexadecimal::lowercase[alias.b[i]] << " (" << std::setw(3) << unsigned(alias.b[i]) << ")";
        }
        double arr[sizeof(WORD)] = { 0 };
#if Y_BYTE_ORDER == Y_BIG_ENDIAN
        std::cerr << " <== BE";
        for(unsigned i=0;i<sizeof(WORD);++i)
        {
            arr[i] = double(alias.b[i]);
        }
#else
        std::cerr << " <== LE";
        for(unsigned i=0;i<sizeof(WORD);++i)
        {
            arr[i] = double(alias.b[i]);
        }
#endif
        std::cerr << std::endl;
        for(unsigned i=0;i<sizeof(WORD);++i)
        {
            std::cerr << " a[" << i << "]=" << arr[i] << std::endl;
        }
    }
}


Y_UTEST(yap_w2r)
{

    std::cerr << "word_size=" << apn::word_size << std::endl;


    show<uint8_t>( 0x12);
    show<uint16_t>(0x1234);
    show<uint32_t>(0x12345678);
    show<uint64_t>(0x12345678);

    std::cerr << "/**/";
    for(unsigned i=0;i<256;++i)
    {
        std::cerr << std::setw(4) << i;
        if(i<255) std::cerr << ",";
        if(i<255&&((1+i)%16)==0) std::cerr << std::endl << "/**/";
    }
    std::cerr << std::endl;


}
Y_UTEST_DONE()

