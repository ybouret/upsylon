
#include "y/gfx/async/engine.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace upsylon
{
    namespace GFX
    {

        namespace Async
        {

            Worker:: Worker()
            {
            }

            Worker:: ~Worker() throw()
            {
            }
        }

    }

}

namespace upsylon
{
    namespace GFX
    {

        namespace Async
        {

            InnerWorker:: InnerWorker(const Tile &t) :
            Worker(),
            tile(t)
            {
            }


            InnerWorker:: ~InnerWorker() throw()
            {
            }


            void InnerWorker:: compute(parallel &, lockable &sync)
            {
                Y_LOCK(sync);
                std::cerr << "InnerWorker@rank=" << tile.rank << std::endl;
            }
        }

    }

}

namespace upsylon
{
    namespace GFX
    {

        namespace Async
        {

            OuterWorker:: OuterWorker(const Boundaries &b) :
            Worker(),
            boundaries(b)
            {
            }


            OuterWorker:: ~OuterWorker() throw()
            {
            }


            void OuterWorker:: compute(parallel &, lockable &sync)
            {
                Y_LOCK(sync);
                std::cerr << "OuterWorker" << std::endl;
            }

        }

    }

}

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
                if(impl)
                {
                    delete static_cast<Batch *>(impl);
                    impl = 0;
                }

                static memory::allocator &mgr = memory::dyadic::location();
                while(iwBuilt>0)
                {
                    self_destruct( insideWorkers[--iwBuilt] );
                }
                mgr.release_as(insideWorkers,iwCount,iwBytes);
            }

            Engine:: Engine(const size_t             cpus,
                            const Topology::Pointer &topo) :
            Tiles(cpus,topo),
            impl(0),
            outsideWorker(*topo),
            insideWorkers(NULL),
            iwBuilt(0),
            iwCount(0),
            iwBytes(0)
            {
                static memory::allocator &mgr = memory::dyadic::instance();
                if(topology->inner)
                {
                    iwCount       = size;
                    insideWorkers = mgr.acquire_as<InnerWorker>(iwCount,iwBytes);
                    try
                    {
                        const Tiles &self = *this;
                        while(iwBuilt<size)
                        {
                            new (insideWorkers+iwBuilt) InnerWorker( self[iwBuilt] );
                            ++iwBuilt;
                        }
                    }
                    catch(...)
                    {
                        releaseAll();
                        throw;
                    }

                }
                else
                {
                    // no inner worker!
                }

                try
                {
                    const size_t    num_tasks = iwBuilt+1;
                    auto_ptr<Batch> batch     = new Batch(num_tasks);
                    std::cerr << "#tasks=" << num_tasks << std::endl;
                    {
                        const concurrent::job_type J( &outsideWorker, & OuterWorker::compute );
                        batch->tasks.push_back_(J);
                    }
                    for(size_t i=0;i<iwBuilt;++i)
                    {
                        const concurrent::job_type J( insideWorkers+i, & InnerWorker::compute );
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
