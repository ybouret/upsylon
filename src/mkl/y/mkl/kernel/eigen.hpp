//! \file
#ifndef Y_MKL_EIGEN_INCLUDED
#define Y_MKL_EIGEN_INCLUDED 1

#include "y/container/matrix.hpp"
#include "y/mkl/types.hpp"

namespace upsylon {
    
    namespace mkl {

        //! post diagonalisation sorting kind
        enum sort_eigv_by
        {
            sort_eigv_none,       //!< no sorting
            sort_eigv_by_value,   //!< sort by decreasing value
            sort_eigv_by_module   //!< sort by decreasing module
        };

        //! diagonalisation of symmetric matrices
        struct eigen
        {


            static const size_t min_iter = 4;  //!< for tolerance scaling
            static const size_t max_iter = 64; //!< algorithm should converge before
            //! Jacobi rotation
#define _Y_JACOBI(a,i,j,k,l) g=a[i][j]; h=a[k][l]; a[i][j]=g-s*(h+g*tau); a[k][l]=h+s*(g-h*tau)

            //! Jacobi reduction
            /**
             \param a    is a symetric matrix, REGENERATED at the end...
             \param d    are the eigenvalues
             \param v    columns are the eigenvectors
             \param kind how to sort vectors/values afterwards
             \return a =  v'* d * v
             */
            template <typename T> static inline
            bool build( matrix<T> &a, addressable<T> &d, matrix<T> &v, sort_eigv_by kind = sort_eigv_none )
            {
                assert(a.rows>0);
                assert(a.is_square);
                assert(a.same_size_than(v));
                assert(d.size() == a.rows);
                
                bool           success = true;
                const size_t   n       = a.rows;
                array<T>      &b       = a.r_aux1;
                array<T>      &z       = a.r_aux2;

                //==============================================================
                // initialize eigenvectors and workspace
                //==============================================================
                for(size_t ip=1;ip<=n;++ip)
                {
                    {
                        addressable<T> &v_ip = v[ip];
                        for(size_t iq=1;iq<=n;++iq) v_ip[iq]=T(0);
                        v_ip[ip]=T(1);
                    }
                    addressable<T> &a_ip = a[ip];
                    b[ip]=d[ip]=a_ip[ip];
                    z[ip]=T(0);
                }
                
                //==============================================================
                // looping over sweeps
                //==============================================================
                for(size_t iter=1;iter<=max_iter;++iter)
                {
                    T sm = 0;
                    for(size_t ip=1;ip<n;++ip) {
                        for(size_t iq=ip+1;iq<=n; ++iq)
                        sm += fabs_of(a[ip][iq]);
                    }
                    if (sm <= numeric<T>::minimum )
                    {
                        assert(true==success);
                        goto DONE; // OK
                    }
                    
                    const T tresh = (iter<min_iter) ? T(0.2)*sm/(n*n) : T(0);
                    
                    
                    for(size_t ip=1;ip<n;++ip)
                    {
                        for(size_t iq=ip+1;iq<=n;++iq)
                        {
                            T g=T(100) * fabs_of(a[ip][iq]);
                            if ( (iter>min_iter) && almost_equal( fabs_of(d[ip])+g, fabs_of(d[ip]))
                                && almost_equal( fabs_of(d[iq])+g, fabs_of(d[iq])) )
                            {
                                a[ip][iq]=T(0.0);
                            }
                            else
                                if (fabs_of(a[ip][iq]) > tresh)
                                {
                                    T h = d[iq]-d[ip];
                                    T t = 0;
                                    if ( almost_equal(fabs_of(h)+g,fabs_of(h)) )
                                    {
                                        t=(a[ip][iq])/h;
                                    }
                                    else
                                    {
                                        const T theta=T(0.5)*h/(a[ip][iq]);
                                        t=T(1.0)/(fabs_of(theta)+sqrt_of(T(1.0)+theta*theta));
                                        if (theta < T(0.0))
                                            t = -t;
                                    }
                                    const T c   = T(1.0)/sqrt_of(T(1.0)+t*t);
                                    const T s   = t*c;
                                    const T tau = s/(T(1.0)+c);
                                    h=t*a[ip][iq];
                                    z[ip] -= h;
                                    z[iq] += h;
                                    d[ip] -= h;
                                    d[iq] += h;
                                    a[ip][iq]=T(0.0);
                                    for(size_t j=1;j<ip;++j) {
                                        _Y_JACOBI(a,j,ip,j,iq);
                                    }
                                    for(size_t j=ip+1;j<iq;++j) {
                                        _Y_JACOBI(a,ip,j,j,iq);
                                    }
                                    for(size_t j=iq+1;j<=n;++j) {
                                        _Y_JACOBI(a,ip,j,iq,j);
                                    }
                                    for(size_t j=1;j<=n;++j) {
                                        _Y_JACOBI(v,j,ip,j,iq);
                                    }
                                }
                        }
                    }
                    
                    for(size_t ip=1;ip<=n;++ip)
                    {
                        b[ip] += z[ip];
                        d[ip] = b[ip];
                        z[ip] = T(0.0);
                    }
                }
                success = false;
                
            DONE:
                //__________________________________________________________________
                //
                // matrix regeneration
                //__________________________________________________________________
                for(size_t i=1,ip=2;i<n;++i,++ip)
                {
                    for(size_t j=ip;j<=n;++j)
                    {
                        a[i][j] = a[j][i];
                    }
                }
                if(success)
                {
                    switch(kind)
                    {
                        case sort_eigv_none:                    break;
                        case sort_eigv_by_value:  eigsrt(d,v);  break;
                        case sort_eigv_by_module: eigsrtA(d,v); break;
                    }
                    return true;
                }
                else
                {
                    return false;
                }
                
                
            }
            
            //! sort eigenvalues/vectors by DECREASING value
            template <typename T> static inline
            void  eigsrt( addressable<T> &d, matrix<T> &v) throw()
            {
                const size_t n = v.rows;
                assert( d.size() >= v.rows );
                for (size_t i=1,ip=2;i<n;++i,++ip)
                {
                    size_t k = i;
                    T      p = d[k];
                    for (size_t j=ip;j<=n;j++)
                    {
                        if (d[j] >= p)
                        {
                            p=d[k=j];
                        }
                    }
                    if (k != i)
                    {
                        d[k]=d[i];
                        d[i]=p;
                        v.swap_cols(i,k);
                    }
                }
            }
            
            //! sort by DECREASING ABSOLUTE value
            template <typename T> static inline
            void eigsrtA( addressable<T> &d, matrix<T> &v) throw()
            {
                const size_t n = v.rows;
                assert( d.size() >= v.rows );
                for (size_t i=1;i<n;i++)
                {
                    size_t k = i;
                    T      p = d[k];
                    for (size_t j=i+1;j<=n;j++)
                    {
                        if( fabs_of(d[j]) >= fabs_of(p) )
                        {
                            p=(d[k=j]);
                        }
                    }
                    if (k != i)
                    {
                        d[k]=d[i];
                        d[i]=p;
                        v.swap_cols(i,k);
                    }
                }
            }
            
        };
        
    }
    
}

#endif

