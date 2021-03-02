#include "y/concurrent/task/pipeline.hpp"
#include "y/concurrent/task/serial.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/utest/timings.hpp"
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
    
    size_t Engine:: Shift = 28;
    
    
    
}

#include "y/string/convert.hpp"

Y_UTEST(thr_pipeline)
{
    
    size_t N = 8;
    if(argc>1)
    {
        N = string_convert::to<size_t>(argv[1],"N");
    }
    
    if(argc>2)
    {
        Engine::Shift = string_convert::to<size_t>(argv[2],"Shift");
    }
    
    
    
    std::cerr << "<Empty  Pipeline>" << std::endl;
    {
        volatile concurrent::pipeline Q;
    }
    std::cerr << "<Empty  Pipeline/>" << std::endl;
    
    std::cerr << "<User's Pipeline>" << std::endl;
    {
        concurrent::pipeline          Q;
        vector<Engine>                engines(N,as_capacity);
        vector<concurrent::job::uuid> jids(N,0);
        vector<concurrent::job::type> jobs(N,as_capacity);
        
        for(size_t i=0;i<N;++i)
        {
            {
                const Engine tmp(N,i);
                engines << tmp;
            }
            std::cerr << engines.back() << std::endl;
        }
        
        for(size_t i=1;i<=N;++i)
        {
            const concurrent::job::type J( &engines[i], & Engine::compute );
            jobs.push_back(J);
        }
        
        for(size_t i=N;i>0;--i)
        {
            Q(engines[i], & Engine::compute );
        }
        
        Q.flush();
        std::cerr << "yield res: " << Engine::Sum(engines) << std::endl;
        
        Q.batch(jids,jobs);
        Q.flush();
        std::cerr << "batch res: " << Engine::Sum(engines) << std::endl;
        
        std::cerr << "\t<Perf>" << std::endl;
        
        Q.verbose = false;
        
        
        double ser_speed = 0;
        {
            concurrent::serial S;
            Y_TIMINGS(ser_speed, 1,
                      S.batch(jids,jobs);
                      S.flush());
        }
        std::cerr << "\tser_speed  = " << ser_speed << std::endl;
        
        double par_speed = 0;
        Y_TIMINGS(par_speed, 1,
                  Q.batch(jids,jobs);
                  Q.flush());
        std::cerr << "\tpar_speed  = " << par_speed << std::endl;
        std::cerr << "\tefficiency = " << Q.efficiency(par_speed/ser_speed) << std::endl;
        std::cerr << "\t<Perf/>" << std::endl;
        
        
    }
    std::cerr << "<User's Pipeline/>" << std::endl;
    
    
    
    
}
Y_UTEST_DONE()

