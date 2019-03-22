#include "y/concurrent/scheme/dispatcher.hpp"
#include "y/utest/run.hpp"
#include "y/os/wtime.hpp"

using namespace upsylon;


namespace {

    class dummy
    {
    public:
        int       data;
        rt_clock &rtc;

        explicit dummy(const int d, rt_clock &c) : data(d), rtc(c)
        {

        }

        dummy( const dummy &other) : data(other.data), rtc(other.rtc)
        {
        }

        ~dummy() throw()
        {
        }

        inline void operator()( parallel &context, lockable &access )
        {
            work(context,access);
        }

        inline void work(parallel &context, lockable &access )
        {
            double dt = 0.05;
            {
                Y_LOCK(access);
                ++data;
                std::cerr << "@thread#" << context.rank << ", data=" << data <<  std::endl;
                std::cerr << "waiting..." << std::endl;
                dt += alea.to<double>()*0.1;
            }
            rtc.sleep(dt);
        }

    private:
        Y_DISABLE_ASSIGN(dummy);
    };

    
}



Y_UTEST(server)
{
    std::cerr << "sizeof(job_type)           =" << sizeof(concurrent::job_type) << std::endl;
    std::cerr << "sizeof(dispatcher::jnode)  =" << sizeof(concurrent::dispatcher::jnode) << std::endl;

    concurrent::dispatcher srv(true);

    wtime chrono;
    dummy d(7,chrono);


    srv.enroll( &d, &dummy::work );
    srv.enroll(d);

    for(size_t i=0;i<32;++i)
    {
        srv.enroll( &d, &dummy::work );
    }

    srv.flush();

}
Y_UTEST_DONE()

#include "y/sequence/vector.hpp"
#include "y/sort/sorted-sum.hpp"
namespace
{
    class ComputeEngine
    {
    public:
        static const size_t Count = 1024*1024*4;

        const size_t rank;
        const size_t size;
        double      &target;

        inline ComputeEngine(const size_t r,
                             const size_t s,
                             double      &tgt) throw() :
        rank(r), size(s), target(tgt)
        {
            assert(size>0);
            assert(rank<size);
        }

        inline ~ComputeEngine() throw()
        {
        }

        inline ComputeEngine( const ComputeEngine &other ) :
        rank(other.rank), size(other.size), target(other.target)
        {
        }

        inline void operator()( parallel &, lockable &access )
        {
            const size_t nmin = 1+rank * Count;
            const size_t nmax = nmin + Count;
            if(false){
                Y_LOCK(access);
                std::cerr << "...compute " << nmin << ".." << nmax-1 << std::endl;
            }
            double sum = 0;
            for(size_t i=nmin;i<nmax;++i)
            {
                const double d = i;
                sum += 1.0 / (d*d);
            }
            target = sum;
        }

    private:
        Y_DISABLE_ASSIGN(ComputeEngine);
    };

    static inline double data2pi( array<double> &target)
    {
        return sqrt( 6.0 * sorted_sum(target) );
    }

}

Y_UTEST(servpi)
{
    size_t n=100;
    vector<double>               target(n);
    vector<concurrent::job_uuid> uuids(n,0);
    vector<concurrent::job_type> batch(n,as_capacity);
    for(size_t i=1;i<=n;++i)
    {
        const ComputeEngine        tmp(i-1,n,target[i]);
        const concurrent::job_type job(tmp);
        batch.push_back(job);
    }

    alea.shuffle(*batch,*uuids,batch.size());

    wtime chrono;
    double seq_ell = 0;
    target.ld(0);
    std::cerr << "<SEQ>" << std::endl;
    {
        concurrent::sequential_server srv;
        chrono.start();
        srv.process(uuids,batch);
        srv.flush();
        seq_ell = chrono.query();
    }
    const double seq_pi = data2pi(target);
    std::cerr << "seq_pi=" << seq_pi << " in " << seq_ell << " seconds" << std::endl;

    double par_ell = 0;
    target.ld(0);
    std::cerr << "<PAR>" << std::endl;
    {
        concurrent::dispatcher srv(false);
        chrono.start();
        srv.process(uuids,batch);
        srv.flush();
        par_ell = chrono.query();

        const double par_pi = data2pi(target);
        std::cerr << "par_pi=" << par_pi << " in " << par_ell << " seconds" << std::endl;
        std::cerr << "Efficiency: " << srv.engine()[0].efficiency(seq_ell/par_ell) << "%" << std::endl;
    }
    
}
Y_UTEST_DONE()

