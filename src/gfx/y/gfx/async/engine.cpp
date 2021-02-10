
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

            Engine:: ~Engine() throw()
            {
                releaseAll();
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
            }

            Engine:: Engine(const Tiling &tiling) :
            Tiling(tiling),
            impl(0),
            worker(0),
            wBuilt(0),
            wCount(0),
            wBytes(0)
            {
                static memory::allocator &mgr = memory::dyadic::instance();
                const Tiles              &self = **this;
                const size_t              size = self.size;

                // building workers
                wCount = size;
                worker = mgr.acquire_as<Worker>(wCount,wBytes);
                try
                {
                    while(wBuilt<size)
                    {
                        new (worker+wBuilt) Worker(self[wBuilt]);
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
                    auto_ptr<Batch> batch     = new Batch(size);
                    for(size_t i=0;i<size;++i)
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
