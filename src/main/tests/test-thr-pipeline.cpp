#include "y/concurrent/task/pipeline.hpp"
#include "y/concurrent/task/serial.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/os/real-time-clock.hpp"
#include "y/type/utils.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace {
    
    class Engine : public concurrent::context
    {
    public:
        static size_t Shift;
        
        double       sum;
        const size_t length;
        const size_t offset;
        
        Engine(const size_t size, const size_t rank) throw() :
        concurrent::context(size,rank),
        sum(0),
        length( size_t(1) << Shift ),
        offset( 1 )
        {
            split(aliasing::_(length),aliasing::_(offset));
        }
        
        virtual ~Engine() throw()
        {
        }
        
        Engine(const Engine &other) throw() :
        concurrent::context(other),
        sum( other.sum ),
        length( other.length ),
        offset( other.offset )
        {
        }
        
        inline friend std::ostream & operator<<(std::ostream &os, const Engine &eng)
        {
            os << "engine@" << eng.label << ": " << eng.offset << "+" << eng.length;
            return os;
        }
        
        void compute(lockable &) throw()
        {
            double localSum = 0;
            size_t i=offset;
            for(size_t j=length;j>0;--j)
            {
                localSum += 1.0 / square_of( double(i++));
            }
            sum = localSum;
        }
        
        static inline
        double Sum( const accessible<Engine> &engines ) throw()
        {
            const size_t n  = engines.size();
            double       s  = 0;
            for(size_t i=n;i>0;--i) s += engines[i].sum;
            return sqrt(6.0*s);
        }
        
    private:
        Y_DISABLE_ASSIGN(Engine);
    };
    
    size_t Engine:: Shift = 24;
    
    
    
}

#include "y/string/convert.hpp"

Y_UTEST(thr_pipeline)
{
    
    size_t N = 1;
    if(argc>1)
    {
        N = string_convert::to<size_t>(argv[1],"N");
    }
    std::cerr << "<Empty  Pipeline>" << std::endl;
    {
        volatile concurrent::pipeline Q;
    }
    std::cerr << "<Empty  Pipeline/>" << std::endl;

    std::cerr << "<User's Pipeline>" << std::endl;
    {
        concurrent::pipeline Q;
        vector<Engine>       engines(N,as_capacity);
        
        for(size_t i=0;i<N;++i)
        {
            {
                const Engine tmp(N,i);
                engines << tmp;
            }
            std::cerr << engines.back() << std::endl;
        }
        
        for(size_t i=N;i>0;--i)
        {
            Q(engines[i], & Engine::compute );
        }
        
        Q.flush();
        std::cerr << "res: " << Engine::Sum(engines) << std::endl;
    }
    std::cerr << "<User's Pipeline/>" << std::endl;
    
    
}
Y_UTEST_DONE()

