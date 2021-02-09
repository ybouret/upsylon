#include "y/gfx/area/tiles.hpp"
#include "y/concurrent/scheme/nexus.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;
using namespace GFX;

namespace
{

    class Worker
    {
    public:
        const size_t rank;
        const Tile  &tile;

        Worker(const size_t r, const Tile &t) :
        rank(r),
        tile(t)
        {
        }

        ~Worker() throw()
        {
        }

        void work(parallel &, lockable &sync)
        {
            {
                Y_LOCK(sync);
                std::cerr << "working@rank=" << rank << std::endl;
            }

        }

        Worker(const Worker &w) : rank(w.rank), tile(w.tile) {}


    private:
        Y_DISABLE_ASSIGN(Worker);
    };
    

}

Y_UTEST(tiles)
{

    unit_t w = 10;
    unit_t h = 20;

    if(argc>1)
    {
        const Point p = Parse::WxH(argv[1]);
        w=p.x;
        h=p.y;
    }

    Area              area(0,0,w,h);
    Topology::Pointer topo = new Topology(area);
    concurrent::nexus engine;

    for(size_t cpus=1;cpus<=8;++cpus)
    {
        std::cerr << "<cpus=" << cpus << ">" << std::endl;
        Tiles tiles(cpus,topo);
        list<Worker>                 workers;
        vector<concurrent::job_type> jobs;
        for(size_t i=0;i<tiles.size;++i)
        {
            {
                const Worker tmp(i,tiles[i]);
                workers.push_back(tmp);
            }
            const concurrent::job_type J( &workers.back(), & Worker::work );
            jobs.push_back(J);
        }

        vector<concurrent::job_uuid> jids(jobs.size(),0);

        engine.process(jids,jobs);
        engine.flush();







        std::cerr << "+outer: " << tiles.topology->outer << std::endl;
    }



}
Y_UTEST_DONE()


