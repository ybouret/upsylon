
#include "y/gfx/async/engine.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace GFX
    {

        namespace Async
        {

            size_t Engine:: size() const throw()
            {
                return count;
            }

            Engine:: ~Engine() throw()
            {
                releaseAll();
            }

            parallel & Engine:: operator[](const size_t i) throw()
            {
                assert(i>0);
                assert(i<=count);
                return wShift[i];
            }


            const parallel & Engine:: operator[](const size_t i) const throw()
            {
                assert(i>0);
                assert(i<=count);
                return wShift[i];
            }

            class Batch : public object
            {
            public:
                vector<concurrent::job_type,memory::dyadic> tasks;
                vector<concurrent::job_uuid,memory::dyadic> uuids;

                inline Batch(const size_t nt) :
                tasks(nt,as_capacity),
                uuids(nt,0)
                {
                }

                inline virtual ~Batch() throw()
                {
                }
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Batch);
            };

            void Engine:: releaseAll() throw()
            {
                static memory::allocator &mgr = memory::dyadic::location();

                if(impl)
                {
                    delete static_cast<Batch *>(impl);
                    impl = 0;
                }

                while(wBuilt>0)
                {
                    self_destruct(worker[--wBuilt]);
                }

                mgr.release_as(worker, wCount, wBytes);
                wShift = 0;
            }

            Engine:: Engine(const Area &area, const size_t maxThreads) :
            Tiles(area,maxThreads),
            impl(0),
            worker(0),
            wShift(0),
            wBuilt(0),
            wCount(0),
            wBytes(0)
            {
                static memory::allocator &mgr = memory::dyadic::instance();
                const Tiles              &self = *this;
                
                // building workers
                wCount = count;
                worker = mgr.acquire_as<Worker>(wCount,wBytes);
                try
                {
                    while(wBuilt<count)
                    {
                        new (worker+wBuilt) Worker(count,self[wBuilt]);
                        ++wBuilt;
                    }
                }
                catch(...)
                {
                    releaseAll();
                    throw;
                }

                // building batch
                try
                {
                    auto_ptr<Batch> batch     = new Batch(count);
                    for(size_t i=0;i<count;++i)
                    {
                        const concurrent::job_type J(worker+i, &Worker::run );
                        batch->tasks.push_back_(J);
                    }
                    assert( batch->tasks.size() == batch->uuids.size() );
                    impl = batch.yield();
                }
                catch(...)
                {
                    releaseAll();
                    throw;
                }

                wShift = worker-1;
                
            }

            void Engine:: cycle(concurrent::server &srv)
            {
                assert(impl);
                Batch &batch = *static_cast<Batch *>(impl);
                srv.process(batch.uuids,batch.tasks);
                srv.flush();
            }



        }

    }
}
