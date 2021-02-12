
//! \file

#ifndef Y_GFX_ASYNC_GRADIENT_INCLUDED
#define Y_GFX_ASYNC_GRADIENT_INCLUDED 1

#include "y/gfx/async/ops/execute.hpp"
#include "y/mkl/types.hpp"

namespace upsylon
{
    namespace GFX
    {
        template <typename T> class Pixmap;
        
        namespace Async
        {

            //__________________________________________________________________
            //
            //
            //! numerical gradient computation
            //
            //__________________________________________________________________
            template <typename T>
            class Gradient : public Pixmap<T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef point2d<T>     Vertex;    //!< alias for coordinate
                typedef Pixmap<Vertex> Vertices;  //!< alias for vertices

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! setup
                explicit Gradient(const unit_t W, const unit_t H) :
                Pixmap<T>(W,H),
                grad(W,H)
                {
                }

                //! cleanup
                virtual ~Gradient() throw()
                {
                }
                

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                //! minimum is always zero on a corber
                template <typename U>
                T compute(const Pixmap<U> &source,
                          Broker          &broker,
                          bool            normalize=false)
                {
                    // prepare aync context
                    Compute<U> op   = { *this, source };
                    Team      &team = *broker;
                    team.make<T>();

                    // run
                    broker(op.run,&op);

                    // collect vmax
                    T vmax = team[1]._<T>();
                    for(size_t i=team.size();i>0;--i)
                    {
                        const T temp = team[i]._<T>();
                        if(temp>vmax) vmax=temp;
                    }

                    // optional vmax
                    if(normalize)
                    {
                        if(vmax>0)
                        {
                            Rescale rescale = { vmax };
                            Execute::Function(*this,*this,rescale,broker);
                        }
                    }
                    return vmax;
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                Vertices grad; //!< gradient field

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Gradient);
                
                template <typename U>
                struct Compute
                {
                    Gradient        &gradient;
                    const Pixmap<U> &source;

                    static inline void run(Worker   &w,
                                           lockable &,
                                           void     *args )
                    {
                        assert(args);

                        Compute         &self   = *static_cast<Compute *>(args);
                        Pixmap<T>       &norm   = self.gradient;
                        Pixmap<Vertex>  &grad   = self.gradient.grad;
                        const Tile      &tile   = w.tile;
                        const Pixmap<U> &source = self.source;
                        T                vmax=0;

                        {
                            const Point  p = w.begin();
                            const unit_t y = p.y, x=p.x;
                            const typename Pixmap<U>::Row &src_y = source(y);
                            const T dx = T(src_y(x+1))     - T(src_y(x-1));
                            const T dy = T(source(y+1)(x)) - T(source(y-1)(x));
                            vmax = mkl::sqrt_of(dx*dx+dy*dy);
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
                                if(temp>vmax) vmax = temp;
                                xm = x;
                                x  = xp;
                                ++xp;
                            }
                        }
                        w._<T>()=vmax;
                    }

                };

                struct Rescale
                {
                    T vmax;
                    inline T operator()(const T v)
                    {
                        if(v<=0)
                        {
                            return 0;
                        }
                        else
                        {
                            if(v>=vmax)
                            {
                                return 1;
                            }
                            else
                            {
                                return v/vmax;
                            }
                        }

                    }
                };

            };
            
            
            
        }
        
    }
    
}

#endif
