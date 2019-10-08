//! \file
#ifndef Y_EUCLIDEAN_BASIS_INCLUDED
#define Y_EUCLIDEAN_BASIS_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/type/args.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //! prototype for Frenet basis
            template <typename T,size_t> class __Basis;
            

            //! 2D Frenet: tangent,normal
            template <typename T>
            class __Basis<T,2>
            {
            public:
                Y_DECL_ARGS(T,type);                  //!< alias
                typedef point2d<mutable_type> vertex; //!< alias
                typedef const vertex          const_vertex;

                const_vertex t; //!< unit tangent
                const_vertex n; //!< unit normal

                inline      __Basis() throw() : t(), n() {}              //!< setup
                inline     ~__Basis() throw() { zero(); }                //!< cleanup
                inline void  zero()    throw() { bzset_(t); bzset_(n); } //!< clear

            
            private:
                Y_DISABLE_COPY_AND_ASSIGN(__Basis);
            };

            //! 3D Frenet: tangent, normal, binormal
            template <typename T>
            class __Basis<T,3>
            {
            public:
                Y_DECL_ARGS(T,type);                         //!< alias
                typedef point3d<mutable_type> vertex;        //!< alias
                typedef const vertex          const_vertex;

                const_vertex t; //!< unit tangent
                const_vertex n; //!< unit normal
                const_vertex b; //!< unit binormal t^n

                inline      __Basis() throw() : t(), n(), b() {}                    //!< setup
                inline     ~__Basis() throw() { zero(); }                           //!< cleanup
                inline void zero()    throw() { bzset_(t); bzset_(n); bzset_(b); }  //!< clear

                //! assuming tangent is set
                inline void finalize( const_vertex &N ) throw()
                {
                    // assign normal
                    vertex &_n = aliasing::_(n);
                    _n = N;


                    // remove component on t
                    _n -= (_n*t)*t;

                    // update
                    const_type n2 = _n.norm2();
                    if(n2>0)
                    {
                        _n /= sqrt_of(n2);
                        aliasing::_(b) = t ^ n;
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

