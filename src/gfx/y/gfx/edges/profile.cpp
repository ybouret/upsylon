
#include "y/gfx/edges/profile.hpp"
#include "y/momentary/value.hpp"
#include "y/momentary/link.hpp"

namespace upsylon
{
    namespace graphic
    {
        
        namespace edges
        {
            
            profile::  profile() throw() : Blobs(0), Edges(0), Masks(0) {}
            profile:: ~profile() throw() {}

            size_t profile:: tighten(pixmap<uint8_t> &edges,
                                     broker          &apply,
                                     const uint8_t    feeble_limit,
                                     const uint8_t    strong_limit)
            {
                apply.caches.make<size_t>();
                
                
                struct ops
                {
                    pixmap<uint8_t> &edges;
                    const uint8_t    feeble;
                    const uint8_t    strong;
                    
                    static inline void run(const tile &t, void *args, lockable &)
                    {
                        ops             &self   = *static_cast<ops *>(args);
                        pixmap<uint8_t> &E      = self.edges;
                        const unsigned   feeble = self.feeble;
                        const unsigned   strong = self.strong;
                        size_t           active = 0;
                        
                        for(size_t j=t.size();j>0;--j)
                        {
                            const segment  &s      = t[j];
                            const unit_t    y      = s.y;
                            const unit_t    xmin   = s.xmin;
                            pixrow<uint8_t> &E_y   = E(y);
                            
                            for(unit_t x=s.xmax;x>=xmin;--x)
                            {
                                uint8_t &B0ref = E_y(x);
                                const unsigned B0 = B0ref;
                                if(B0>=strong)
                                {
                                    B0ref = Y_GFX_EDGE_STRONG;
                                    ++active;
                                }
                                else
                                {
                                    if(B0>=feeble)
                                    {
                                        B0ref = Y_GFX_EDGE_FEEBLE;
                                        ++active;
                                    }
                                    else
                                    {
                                        B0ref = 0;
                                    }
                                }
                            }
                        }
                        t.cache->as<size_t>() = active;
                    }
                    
                };
                
                ops todo = { edges, feeble_limit, strong_limit };
                apply(ops::run, &todo);
                
                return apply.caches.sum<size_t>();
            }
            
            bool profile:: call_accept(const blob &b, void *args) throw()
            {
                assert(args);
                return static_cast<profile *>(args)->accept(b);
            }

            size_t profile:: track(blobs           &userBlobs,
                                   pixmap<uint8_t> &userEdges,
                                   pixmap<size_t>  &userMasks,
                                   shared_knots    &knotCache)
            {
                momentary_link<blobs>            linkBlobs(userBlobs,&Blobs);
                momentary_link<pixmap<uint8_t> > linkEdges(userEdges,&Edges);
                momentary_link<pixmap<size_t>  > linkMasks(userMasks,&Masks);
                
                userBlobs.build(userMasks,userEdges,knotCache,8,call_accept,this);
                return userBlobs.size;
            }
            
            
            bool profile:: accept(const blob &b) throw()
            {
                pixmap<uint8_t> &E = *Edges;
                for(const knot  *node=b.head;node;node=node->next)
                {
                    if( E(**node) >= Y_GFX_EDGE_STRONG ) goto IS_STRONG;
                }
                
                b.dispatch(0,E);
                return false; // and will remove blob from blobs
                
            IS_STRONG:
                b.dispatch(Y_GFX_EDGE_STRONG,E);
                return true;
            }
        }
        
    }
    
}

