
#include "y/gfx/edges/profile.hpp"

namespace upsylon
{
    namespace graphic
    {
        
        namespace edges
        {
            
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
                                    B0ref = 255;
                                    ++active;
                                }
                                else
                                {
                                    if(B0>=feeble)
                                    {
                                        B0ref = 127;
                                        ++active;
                                    }
                                    else
                                    {
                                        B0ref = 0;
                                    }
                                }
                            }
                        }
                        (*t.cache) = active;
                    }
                    
                };
                
                ops todo = { edges, feeble_limit, strong_limit };
                apply(ops::run, &todo);
                
                return apply.caches.sum<size_t>();
            }
        }
        
    }
    
}

