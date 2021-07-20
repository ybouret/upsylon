
#include "y/gfx/color/convert.hpp"
#include "y/utest/run.hpp"
#include "y/type/rtti.hpp"
#include "y/ios/align.hpp"
#include "y/sequence/vector.hpp"
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
    
#if 0
    static inline
    void add_to( vector<rgb> &v, const rgb &c)
    {
        for(size_t i=v.size();i>0;--i)
        {
            if(c==v[i]) return;
        }
        v.push_back(c);
    }
#endif
    
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
                    const rgb s = rgb(uint8_t(i),
                                      uint8_t(j),
                                      uint8_t(k) ).saturated();
                    S1.insert(s);
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
        std::cerr << "count4=" << count3 << std::endl;
        Y_CHECK(count1==count4);
        std::cerr << "#sat1=" << S1.size() << std::endl;
        
        
        
    }


}
Y_UTEST_DONE()

