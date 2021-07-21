
#include "y/gfx/color/convert.hpp"
#include "y/utest/run.hpp"
#include "y/type/rtti.hpp"
#include "y/ios/align.hpp"
#include "y/ordered/sorted-vector.hpp"
#include "y/type/utils.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{
    template <typename T, typename U>
    static inline void  Check(const T &value)
    {
        std::cerr << "convert<" << rtti::name_of<T>() << "," << rtti::name_of<U>() << ">::from" << std::endl;
        std::cerr << "\t" << value << " -> " << convert<U,T>::from(value) << std::endl;
    }

    template <typename T>
    void CheckAll(const T &value)
    {
        Check<T,uint8_t>(value);
        Check<T,float>(value);
        Check<T,rgb>(value);
        Check<T,rgba>(value);
        Check<T,YUV>(value);
        std::cerr << std::endl;
    }

    
}

Y_UTEST(gfx_colors)
{

    {
        const uint8_t     u = 'A';
        CheckAll<uint8_t>(u);
    }

    {
        const float     f = 0.22f;
        CheckAll<float>(f);
    }

    {
        const rgb     c(50,100,150);
        CheckAll<rgb>(c);
    }

    {
        const rgba     c(50,100,150);
        CheckAll<rgba>(c);
    }

    {
        const YUV   yuv(0.5f,0.1f,-0.1f);
        CheckAll<YUV>(yuv);
    }


    std::cerr << "Testing saturated colors" << std::endl;
    {
        for(size_t i=0;i<8;++i)
        {
            const rgb C( alea.full<uint8_t>(), alea.full<uint8_t>(), alea.full<uint8_t>() );
            C.saturated();
        }
        
        typedef sorted_vector<rgb>         sorted_rgb;
        typedef ordered_single<sorted_rgb> single_rgb;
        
        single_rgb S1(256*256*256,as_capacity);
        
        size_t count1 = 0;
        size_t count2 = 0;
        size_t count3 = 0;
        for(int i=255;i>=0;--i)
        {
            count3 += ((i+1)*(i+2))/2;
            for(int j=i;j>=0;--j)
            {
                count2 += j+1;
                
                for(int k=j;k>=0;--k)
                {
                    ++count1;
                    const rgb c = rgb(uint8_t(i),
                                      uint8_t(j),
                                      uint8_t(k) );
                    const rgb s = c.saturated();
                    S1.insert(s);
                    Y_ASSERT( (convert<float,rgb>::from(s) >= convert<float,rgb>::from(c) ) );
                }
            }
        }
        std::cerr << "count1=" << count1 << " / " << 256*256*256 << std::endl;
        std::cerr << "count2=" << count2 << std::endl;
        std::cerr << "count3=" << count3 << std::endl;
        Y_CHECK(count1==count2);
        Y_CHECK(count1==count3);
        size_t count4 = 0;
        for(size_t i=1;i<=256;++i)
        {
            count4 += (i*(i+1));
        }
        count4 >>= 1;
        std::cerr << "count4=" << count4 << std::endl;
        Y_CHECK(count1==count4);
        const size_t count5 = ( 256*257*258 )/6;
        std::cerr << "count5=" << count5 << std::endl;
        Y_CHECK(count1==count5);

        std::cerr << "#sat1=" << S1.size() << std::endl;
#if 0
        {
            for(size_t i=1;i<=S1.size();++i)
            {
                std::cerr << " " << S1[i];
                if(0==(i%8)) std::cerr << std::endl;
            }
            std::cerr << std::endl;
        }
        for(size_t i=S1.size();i>0;--i)
        {
            const rgb &lhs = S1[i];
            for(size_t j=i-1;j>0;--j)
            {
                const rgb &rhs = S1[j];
                Y_ASSERT(rhs!=lhs);
            }
        }
#endif

        size_t ns1 = 1;
        size_t ns2 = 1;
        for(int j=255;j>=0;--j)
        {
            for(int k=j;k>=0;--k)
            {
                ++ns1;
            }
            ns2 += (j+1);
        }
        std::cerr << "ns1=" << ns1 << std::endl;
        std::cerr << "ns2=" << ns2 << std::endl;

    }


}
Y_UTEST_DONE()

