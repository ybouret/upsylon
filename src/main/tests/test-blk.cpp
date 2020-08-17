#include "y/core/locate.hpp"
#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"
#include "y/sort/heap.hpp"
#include "y/os/wtime.hpp"
#include "y/type/spec.hpp"
#include "support.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    bool find_std(const T     *arr,
                  const size_t num,
                  const T      tgt )
    {
        for(size_t i=0;i<num;++i)
        {
            if(tgt==arr[i])
            {
                return true;
            }
        }
        return false;
    }
    
    template <typename T>
    bool find_bis(const T     *arr,
                  const size_t num,
                  const T      tgt )
    {
        
        if(tgt<arr[0])
        {
            return false;
        }
        else
        {
            size_t jhi = num;
            if( tgt>arr[--jhi] )
            {
                return false;
            }
            else
            {
                size_t jlo = 0;
                while(jlo<=jhi)
                {
                    //std::cerr << "[" << jlo << ":" << jhi << "]" << std::endl;
                    const size_t mid = (jhi+jlo)>>1;
                    const T      tmp = arr[mid];
                    if( tmp < tgt )
                    {
                        jlo = mid+1;
                    }
                    else
                    {
                        if(tgt<tmp)
                        {
                            jhi = mid-1;
                        }
                        else
                        {
                            return true;
                        }
                    }
                }
                return false;
            }
        }
        
        return false;
    }
    
    template <typename T>
    void check(const size_t num,
               double      &std_speed,
               double      &bis_speed)
    {
        vector<T> arr(num,as_capacity);
        for(size_t i=0;i<num;++i)
        {
            const T tmp = support::get<T>();
            arr.push_back_(tmp);
        }
        Y_ASSERT(arr.size()==num);
        hsort(arr, comparison::increasing<T> );
        
        uint64_t std64 = 0;
        uint64_t bis64 = 0;
        const T *data  = &arr[1];
        const size_t nops = 1024*128;
        for(size_t i=nops;i>0;--i)
        {
            const T    temp = support::get<T>();
            uint64_t   mark = real_time_clock::ticks();
            const bool flag = find_std(data, num, temp);
            std64          += real_time_clock::ticks() - mark;
            mark            = real_time_clock::ticks();
            const bool fbis = find_bis(data,num,temp);
            bis64          += real_time_clock::ticks() - mark;
            Y_ASSERT(fbis==flag);
        }
        
        wtime chrono;
        
        const double std_ell   = chrono(std64);
        const double bis_ell   = chrono(bis64);
        std_speed = nops/std_ell/1e6;
        bis_speed = nops/bis_ell/1e6;
        std::cerr << "speed: std=" << std_speed << " | bis=" << bis_speed << "                \r";
    }
    
    template <typename T>
    void perform()
    {
        const string &tn = type_name_of<T>();
        std::cerr << "checking <" << tn << ">" << std::endl;
        ios::ocstream fp( "blk_" + tn + ".dat" );
        double std_speed=0, bis_speed=0;
        for(unsigned num=1;num<=48;++num)
        {
            check<T>(num,std_speed,bis_speed);
            fp("%u %g %g\n",num,std_speed,bis_speed);
        }
        std::cerr << std::endl;
    }
    
}

Y_UTEST(blk)
{
    
    perform<short>();
    perform<char>();

}
Y_UTEST_DONE()


