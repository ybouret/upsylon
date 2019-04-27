#include "y/ink/ops/edges.hpp"
#include "y/ink/ops/histogram.hpp"

namespace upsylon
{
    namespace Ink
    {
        Edges:: ~Edges() throw()
        {
        }

        Edges:: Edges( const size_t W, const size_t H ):
        Pixmap1(W,H),
        pixels(W,H),
        grad_x(W,H),
        grad_y(W,H),
        grad(W,H),
        gmax(0),
        angle(W,H),
        blobs(W,H),
        particles(),
        compute_metrics_func( this , & Edges::compute_metrics_call),
        analyze_borders_func( this , & Edges::analyze_borders_call)
        {
        }


        namespace
        {
            struct __discriminate
            {
                Pixmap1 *_edges;
                size_t   strongLevel;
                size_t   weakLevel;

                inline void operator()( const Area &area, lockable & )
                {
                    assert(_edges);

                    Pixmap1 &edges = * _edges;
                    Y_INK_AREA_LIMITS(area);

                    for(unit_t y=ymax;y>=ymin;--y)
                    {
                        Pixmap1::Row &B = edges[y];
                        for(unit_t x=xmax;x>=xmin;--x)
                        {
                            const size_t level = B[x];
                            if(level>=strongLevel)
                            {
                                B[x] = Edges::Strong;
                            }
                            else if(level>weakLevel)
                            {
                                B[x] = Edges::Weak;
                            }
                            else
                            {
                                B[x] = 0;
                            }
                        }
                    }
                }
            };
        }

        void Edges:: compute_with(Engine &E)
        {
            // initialize
            particles.release();
            ld(0);
            E.acquire_all(Histogram::BYTES);
            gmax=0;

            // start to run
            E.run(compute_metrics_func);
            gmax = E.get_max<float>();
            if(gmax>0)
            {
                // ok, some borders are detected: find significant levels
                EdgesType &self = *this;
                Histogram H;
                E.run(analyze_borders_func);
                H.append_from(E);
                const size_t thr = H.threshold();
                const size_t sub   = thr>>1;
                std::cerr << "Levels: " << thr << "," << sub << std::endl;
                __discriminate proxy = { this, thr, sub };
                E.run(proxy);
                
                // find borders blobs !
                blobs.build(particles,self,Connect8);

                // analyze
                Blob::List strongParticles;
                while(particles.size)
                {
                    Blob *particle = particles.pop_back();
                    bool  isStrong = false;
                    for(const Vertex *vtx=particle->head;vtx;vtx=vtx->next)
                    {
                        if( Strong == self[vtx->position] )
                        {
                            isStrong = true;
                            break;
                        }
                    }
                    if(isStrong)
                    {
                        strongParticles.push_front(particle);
                        for(const Vertex *vtx=particle->head;vtx;vtx=vtx->next)
                        {
                            self[vtx->position] = Strong;
                        }
                    }
                    else
                    {
                        for(const Vertex *vtx=particle->head;vtx;vtx=vtx->next)
                        {
                            self[vtx->position] = 0;
                        }
                        delete particle;
                    }
                }
                particles.swap_with(strongParticles);
                blobs.rewrite(particles,true);
                std::cerr << "#Edges=" << particles.size << std::endl;
                for(const Blob *particle = particles.head;particle;particle=particle->next)
                {
                    std::cerr << "\tedge#" << particle->tag << " : " << particle->size << std::endl;
                }

            }
            
        }

        void Edges:: compute_metrics_call(const Tile &tile, lockable &)
        {
            Y_INK_AREA_LIMITS(tile);
            float local_gmax = 0;
            for(unit_t y=ymax;y>=ymin;--y)
            {
                const PixmapF::Row &Gx = grad_x[y];
                const PixmapF::Row &Gy = grad_y[y];
                PixmapF::Row       &G  = grad[y];
                PixmapF::Row       &A  = angle[y];
                for(unit_t x=xmax;x>=xmin;--x)
                {
                    const float gx = Gx[x];
                    const float gy = Gy[x];
                    const float g  = sqrt(gx*gx+gy*gy);
                    const float a  = math::__atan2(gy,gx);

                    G[x] = g;
                    A[x] = a;
                    if(g>local_gmax) local_gmax=g;
                }
            }
            tile.cache.get<float>() = local_gmax;
        }

        void Edges:: analyze_borders_call(const Tile &area, lockable &)
        {
            Y_INK_AREA_LIMITS(area);
            LocalMemory        &cache = area.cache; assert(cache.size>=Histogram::BYTES);
            Histogram::count_t *bins  = & cache.get<Histogram::count_t>();
            for(size_t i=0;i<Histogram::BINS;++i) bins[i] = 0;
            for(unit_t y=ymax;y>=ymin;--y)
            {
                const PixmapF::Row &G  = grad[y];
                const PixmapF::Row &A  = angle[y];
                Pixmap1::Row       &B  = (*this)[y];
                for(unit_t x=xmax;x>=xmin;--x)
                {
                    const float g0 = G[x];
                    float       gp = g0;
                    float       gn = g0;
                    {
                        const float  a  = A[x];
                        const unit_t dx = unit_t( floorf( cosf(a) + 0.5f ) );
                        const unit_t dy = unit_t( floorf( sinf(a) + 0.5f ) );
                        {
                            const coord prev(x-dx,y-dy);
                            if(grad.has(prev)) gp = grad[prev];
                        }

                        {
                            const coord next(x+dx,y+dy);
                            if(grad.has(next)) gn = grad[next];
                        }
                    }
                    if( (g0>=gp) && (g0>=gn) )
                    {
                        const uint8_t b    = Y_INK_F2B(g0/gmax);
                        ++bins[ (B[x] = b) ];
                    }
                    else
                    {
                        B[x] = 0;
                    }
                }

            }
        }

    }

}

