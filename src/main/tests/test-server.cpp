#include "y/concurrent/scheme/server.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;


namespace {

    class dummy
    {
    public:
        int data;

        explicit dummy(const int d) : data(d)
        {

        }

        dummy( const dummy &other) : data(other.data)
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
            Y_LOCK(access);
            ++data;
            std::cerr << context.indx << ", data=" << data <<  std::endl;
        }

    private:
        Y_DISABLE_ASSIGN(dummy);
    };



    class dummyServer : public concurrent::server
    {
    public:
        concurrent::fake_lock access;
        parallel              context;
        
        dummyServer() : concurrent::server()
        {
        }

        virtual ~dummyServer() throw()
        {
        }


        void demo( dummy &global )
        {
            concurrent::job_uuid localID = 0;
            reserve(1);

            {
                dummy J(7);
                localID = enqueue(J);
                J.data = 13;

            }

            concurrent::job_uuid globalID = 0;
            {
                globalID = enqueue( &global, &dummy::work );
                global.data = 2;
            }

            std::cerr << "localID =" << localID << std::endl;
            std::cerr << "globalID=" << globalID << std::endl;

            for(jnode *j=jobs.head;j;j=j->next)
            {
                j->work(context,access);
            }
            Y_CHECK(global.data==3);

            remove_pending_jobs();

            reserve(1000);
            
            {
                dummy J(7);
                localID = enqueue(J);
                J.data = 13;
            }

            {
                globalID    = enqueue( &global, &dummy::work );
                global.data = 4;
            }

            std::cerr << "localID =" << localID << std::endl;
            std::cerr << "globalID=" << globalID << std::endl;

            for(jnode *j=jobs.head;j;j=j->next)
            {
                j->work(context,access);
            }
            Y_CHECK(global.data==5);

        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dummyServer);
    };

}

Y_UTEST(server)
{
    std::cerr << "sizeof(job_type)    =" << sizeof(concurrent::job_type) << std::endl;
    std::cerr << "sizeof(server.jnode)=" << sizeof(concurrent::server::jnode) << std::endl;

    dummyServer srv;
    dummy       global(-3);

    srv.demo(global);



}
Y_UTEST_DONE()

