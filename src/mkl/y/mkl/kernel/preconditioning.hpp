//! \file

#ifndef Y_ML_PRECOND_INCLUDED
#define Y_ML_PRECOND_INCLUDED 1


#include "y/container/matrix.hpp"
#include "y/sequence/arrays.hpp"
#include "y/sort/heap.hpp"
#include "y/momentary/link.hpp"
#include "y/momentary/value.hpp"
#include "y/momentary/acquire.hpp"
#include "y/type/aliasing.hpp"
#include "y/mkl/triplet.hpp"
#include "y/mkl/types.hpp"

namespace upsylon
{
    namespace mkl
    {

        //______________________________________________________________________
        //
        //
        //! preconditioning curvature matrices
        //
        //______________________________________________________________________
        template <typename T>
        class preconditioning
        {
        public:
            typedef lightweight_array<T>            array_type; //!< alias
            typedef typename numeric<T>::function   function1d; //!< alias

            //! setup
            inline explicit preconditioning() throw():
            dmin(0),
            dmax(0),
            drho(0),
            diag_(0),
            wksp_(0),
            cut( numeric<T>::tiny ),
            one(1),
            zero(0),
            ctol(numeric<T>::ftol),
            xtol(numeric<T>::sqrt_ftol),
            half( T(0.5) )
            {
            }

            //! cleanup
            inline virtual ~preconditioning() throw() {}


            //! find diagonal weights
            /**
             for algorithm where curvature[i][i] -> curvature[i][i] * (1+weight[i]*lambda),
             then weights are computed to improve the quality of the quasi-inverse
             */
            bool operator()(addressable<T>         &weight,
                            const matrix<T>        &curvature,
                            const accessible<bool> *used,
                            T                      &quality)
            {
                assert(!used||(used->size()==weight.size()));
                assert(weight.size()==curvature.rows);
                assert(weight.size()==curvature.cols);

                //--------------------------------------------------------------
                //
                // initialize, using auxilairy arrays in matrix
                //
                //--------------------------------------------------------------
                array_type  &cdiag = aliasing::_(curvature.r_aux1);
                array_type  &fwksp = aliasing::_(curvature.r_aux2);
                const size_t nvar  = weight.size();

                //--------------------------------------------------------------
                //
                // capture active diagonal components in nrun
                //
                //--------------------------------------------------------------
                const  bool full = (NULL==used);
                size_t      nrun = 0;
                for(size_t i=1;i<=nvar;++i)
                {
                    weight[i] = zero;
                    if( full || (*used)[i] )
                    {
                        ++nrun;
                        cdiag[nrun] = curvature[i][i];
                    }
                }

                switch(nrun)
                {

                    case  1: set_active(weight,used); /* FALLTHRU */
                    case  0: quality=0;  return true; // early return
                    default: break;
                }

                //--------------------------------------------------------------
                //
                // find dmin and dmax using adapted diag[1..nrun]
                //
                //--------------------------------------------------------------
                array_type              diag(*cdiag,nrun); sort(diag);
                const momentary_value<T> kdmin(dmin,diag[1]);
                const momentary_value<T> kdmax(dmax,diag[nrun]);

                //--------------------------------------------------------------
                //
                // check status
                //
                //--------------------------------------------------------------
                quality = dmin / dmax;
                if(dmin<=0)
                {
                    return false; // singular curvature
                }



                //--------------------------------------------------------------
                //
                // look up
                //
                //--------------------------------------------------------------
                const core::temporary_link<array_type>  dlink(diag,&diag_);
                const momentary_value<T>                kdrho(drho,_drho());


                //--------------------------------------------------------------
                //
                // prepare a workspace to compute new values
                //
                //--------------------------------------------------------------
                array_type                              wksp(*fwksp,nrun);
                const core::temporary_link<array_type>  wlink(wksp,&wksp_);



                //--------------------------------------------------------------
                //
                // built-in bracket/minimize
                //
                //--------------------------------------------------------------
                triplet<T> x     = { dmax,       -1, dmax+dmax  };
                triplet<T> f     = { delta(x.a), -1, delta(x.c) };
                T          width = fabs_of(x.c-x.a);
                while(true)
                {
                    static const unsigned B_GT_A  = 0x01;
                    static const unsigned B_GT_C  = 0x02;
                    assert(x.a<=x.c);
                    assert(width>=0);
                    f.b = ( delta( x.b = half*(x.a+x.c) ) );
                    if( f.b <= ctol )
                    {
                        break; // numerical convergence
                    }

                    unsigned    flag  = 0x00;
                    if(f.b>f.a) flag |= B_GT_A;
                    if(f.b>f.c) flag |= B_GT_C;
                    switch(flag)
                    {
                        case B_GT_A:
                            // f.b>f.a, f.b<=f.c
                            // move c to b
                            x.c = x.b;
                            f.c = f.b;
                            break;

                        case B_GT_C:
                            // f.c > f.c, f.b<=f.c
                            // move a to b
                            x.a = x.b;
                            f.a = f.b;
                            break;

                        case B_GT_A|B_GT_C:
                            // got to make a choice..
                            if(f.a<=f.c)
                            {
                                x.c = x.b;
                                f.c = f.b;
                            }
                            else
                            {
                                x.a=x.b;
                                f.a=f.b;
                            }
                            break;

                        default:
                            // bracketed
                            break;
                    }
                    const T new_width = fabs_of(x.c-x.a);
                    const T max_width = fabs_of(xtol * x.b);
                    if(new_width>=width || width <= max_width )
                    {
                        break;
                    }
                    width = new_width;
                }

                //--------------------------------------------------------------
                //
                // compute weights an optimial quality
                //
                //--------------------------------------------------------------
                const T    p    = x.b;
                const T    beta = cut+ max_of(p-drho,zero);


                for(size_t i=nvar;i>0;--i)
                {
                    if(full || (*used)[i])
                    {
                        weight[i] = max_of(p-curvature[i][i],zero)/beta;
                    }
                }

                (void) delta(p);
                quality = wksp[1]/wksp[nrun];
                return true;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(preconditioning);
            T               dmin;
            T               dmax;
            T               drho;
            array_type     *diag_;
            array_type     *wksp_;
            const T         cut,one,zero,ctol,xtol,half;

            inline T _drho() throw()
            {
                assert(diag_);
                const array_type &diag = *diag_;
                const size_t      nrun = diag.size();
                T sum=0, sum2=0;
                for(size_t i=1;i<=nrun;++i)
                {
                    const T d = diag[i];
                    sum  += d;
                    sum2 += square_of(d);
                }
                return min_of(sum2/sum,dmax);
            }



            inline T delta(const T p) throw()
            {

                assert(diag_);
                assert(wksp_);
                assert(diag_->size()==wksp_->size());


                const T           beta = cut+max_of(p-drho,zero);
                array_type       &wksp = *wksp_;
                const array_type &diag = *diag_;
                const size_t      nrun = wksp.size();

                for(size_t j=nrun;j>0;--j)
                {
                    const T dj = diag[j];
                    const T wj = max_of(p-dj,zero)/beta;
                    wksp[j]    = dj * wj;
                }
                sort(wksp);
                return fabs_of(one-wksp[1]/wksp[nrun]);
            }

            inline void set_active(addressable<T>         &w,
                                   const accessible<bool> *used ) throw()
            {
                if(used)
                {
                    const accessible<bool> &u = *used;
                    assert(u.size()==w.size());
                    for(size_t i=w.size();i>0;--i)
                    {
                        if( u[i] )
                        {
                            w[i] = one;
                        }
                    }
                }
                else
                {
                    for(size_t i=w.size();i>0;--i)
                    {
                        w[i] = one;
                    }
                }
            }

            static inline void sort(addressable<T> &arr)
            {
                hsort(arr,comparison::increasing<T>);
            }

        };
    }
}

#endif
