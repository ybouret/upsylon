
//! \file

#ifndef Y_GFX_ASYNC_GRADIENT_INCLUDED
#define Y_GFX_ASYNC_GRADIENT_INCLUDED 1

#include "y/gfx/async/broker.hpp"
#include "y/mkl/types.hpp"

namespace upsylon
{
    namespace GFX
    {
        template <typename T> class Pixmap;
        
        namespace Async
        {
         
            template <typename T>
            class Gradient : public Pixmap<T>
            {
            public:
                typedef point2d<T>     Vertex;
                typedef Pixmap<Vertex> Vertices;
                
                explicit Gradient(const unit_t W, const unit_t H) :
                Pixmap<T>(W,H),
                grad(W,H)
                {
                }
                
                virtual ~Gradient() throw()
                {
                }
                
                Vertices grad;

                template <typename U>
                void compute(const Pixmap<U> &source, Broker &broker)
                {
                    Compute<U> op = { *this, source };
                    Team     &team = *broker;
                    team.make<T>(2);
                    broker(op.run,&op);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Gradient);
                
                template <typename U>
                struct Compute
                {
                    Gradient        &gradient;
                    const Pixmap<U> &source;

                    static inline void run(Worker   &w,
                                           lockable &sync,
                                           void     *args )
                    {
                        assert(args);
                        if(false)
                        {
                            Y_LOCK(sync);
                            std::cerr << "gradient@" << w.label << "/#" << w.tile.items() << std::endl;
                        }
                        Compute         &self   = *static_cast<Compute *>(args);
                        Pixmap<T>       &norm   = self.gradient;
                        Pixmap<Vertex>  &grad   = self.gradient.grad;
                        const Tile      &tile   = w.tile;
                        const Pixmap<U> &source = self.source;
                        T                vmin=0;
                        T                vmax=0;

                        {
                            const Point  p = w.begin();
                            const unit_t y = p.y, x=p.x;
                            const typename Pixmap<U>::Row &src_y = source(y);
                            const T dx = T(src_y(x+1))     - T(src_y(x-1));
                            const T dy = T(source(y+1)(x)) - T(source(y-1)(x));
                            vmin = vmax = mkl::sqrt_of(dx*dx+dy*dy);
                        }

                        for(size_t t=tile.size();t>0;--t)
                        {
                            const HScan                   &s     = tile[t];
                            const unit_t                   y     = s.begin.y;
                            const unit_t                   ym    = y-1;
                            const unit_t                   yp    = y+1;
                            unit_t                         x     = s.begin.x;
                            unit_t                         xm    = x-1;
                            unit_t                         xp    = x+1;
                            typename Pixmap<T>::Row       &n     = norm[y];
                            typename Pixmap<Vertex>::Row  &g     = grad[y];
                            const typename Pixmap<U>::Row &src_y = source(y);
                            for(unit_t i=s.width;i>0;--i)
                            {
                                const T dx   = T(src_y(xp))     - T(src_y(xm));
                                const T dy   = T(source(yp)(x)) - T(source(ym)(x));
                                g[x]         = Vertex(dx,dy);
                                const T temp = n[x] = mkl::sqrt_of(dx*dx+dy*dy);
                                if(temp<vmin)      vmin = temp;
                                else if(temp>vmax) vmax = temp;
                                xm = x;
                                x  = xp;
                                ++xp;
                            }
                        }
                    }


                };

            };
            
            
            
        }
        
    }
    
}

#endif
