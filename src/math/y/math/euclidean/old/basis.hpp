//! \file
#ifndef Y_EUCLIDEAN_BASIS_INCLUDED
#define Y_EUCLIDEAN_BASIS_INCLUDED 1

#include "y/math/euclidean/types.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //! prototype for Frenet basis
            template <typename T,template <typename> class POINT,size_t> class __Basis;
            

            //! 2D Frenet: tangent,normal
            template <typename T,template <typename> class POINT>
            class __Basis<T,POINT,2>
            {
            public:
                Y_DECL_ARGS(T,type);                       //!< alias
                typedef POINT<T>                   Vertex; //!< alias
                typedef typename __Core<T,2>::Type VTX;    //!< alias
                
                const Vertex t; //!< unit tangent
                const Vertex n; //!< unit normal

                inline      __Basis() throw() : t(), n() {}             //!< setup
                inline     ~__Basis() throw() { zero(); }               //!< cleanup
                inline void zero()    throw() { bzset_(t); bzset_(n); } //!< clear

#if 0
                //! assuming tangent is set
                inline void finalize( const Vertex N ) throw()
                {
                    // assign normal
                    aliasing::_(n) = N;
                    const VTX & tt = aliasing::cast<VTX,Vertex>(t);
                    VTX       & nn = aliasing::cast_<VTX,Vertex>(n);

                    // remove component on t
                    nn -= (nn*tt)*tt;
                    const_type n2 = nn.norm2();
                    if(n2>0)
                    {
                        nn /= sqrt_of(n2);
                    }
                }
#endif
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(__Basis);
            };

            //! 3D Frenet: tangent, normal, binormal
            template <typename T,template <typename> class POINT>
            class __Basis<T,POINT,3>
            {
            public:
                Y_DECL_ARGS(T,type);                       //!< alias
                typedef POINT<T>                   Vertex; //!< alias
                typedef typename __Core<T,3>::Type VTX;    //!< alias

                const Vertex t; //!< unit tangent
                const Vertex n; //!< unit tangent
                const Vertex b; //!< unit binormal t^n

                inline      __Basis() throw() : t(), n(), b() {}                    //!< setup
                inline     ~__Basis() throw() { zero(); }                           //!< cleanup
                inline void zero()    throw() { bzset_(t); bzset_(n); bzset_(b); }  //!< clear

                //! assuming tangent is set
                inline void finalize( const Vertex &N ) throw()
                {
                    // assign normal
                    aliasing::_(n) = N;
                    const VTX & tt = aliasing::cast<VTX,Vertex>(t);
                    VTX       & nn = aliasing::cast_<VTX,Vertex>(n);

                    // remove component on t
                    nn -= (nn*tt)*tt;

                    // update
                    const_type n2 = nn.norm2();
                    if(n2>0)
                    {
                        nn /= sqrt_of(n2);
                        aliasing::cast_<Vertex,VTX>(b) = tt ^ nn;
                    }
                    else
                    {
                        bzset_(b);
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(__Basis);
            };


        }

    }

}

#endif

