
//! \file

#ifndef Y_GFX_ASYNC_GRADIENT_INCLUDED
#define Y_GFX_ASYNC_GRADIENT_INCLUDED 1

#include "y/gfx/async/broker.hpp"


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
                void build(const Pixmap<U> & ,
                           Broker          & )
                {
                    
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Gradient);
                
                template <typename U>
                struct Builder
                {
                    
                };
              
            };
            
            
            
        }
        
    }
    
}

#endif
