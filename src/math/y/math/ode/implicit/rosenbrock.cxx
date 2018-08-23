#include "y/math/ztype.hpp"
#include "y/math/ode/implicit/rosenbrock.hpp"

namespace upsylon
{
    namespace math
    {
        
        namespace ODE
        {

            template <>
            rosenbrock<real_t>:: ~rosenbrock() throw() {}
            
            template <>
            rosenbrock<real_t>:: rosenbrock() : 
            implicit_step<real_t>( 9 ),
            g1(    next() ),
            g2(    next() ),
            g3(    next() ),
            g4(    next() ),
            dfdx(  next() ),
            err(   next() ),
            ysav(  next() ),
            dysav( next() ),
            dfdy(),
            a()
            {
            }
            
            template <>
            void rosenbrock<real_t>:: prepare( size_t nv )
            {
                assert(nv>0);
                acquire(nv);
                dfdy.make(nv,nv);
                a.make(nv,nv);
            }
            
        }


    }

}
