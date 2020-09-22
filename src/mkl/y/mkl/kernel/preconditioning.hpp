//! \file

#ifndef Y_ML_PRECOND_INCLUDED
#define Y_ML_PRECOND_INCLUDED 1


#include "y/container/matrix.hpp"
#include "y/sequence/arrays.hpp"
#include "y/sort/heap.hpp"
#include "y/core/temporary-link.hpp"
#include "y/core/temporary-value.hpp"
#include "y/core/temporary-acquire.hpp"
#include "y/type/aliasing.hpp"
#include "y/mkl/utils.hpp"
#include "y/ios/ocstream.hpp"
#include "y/mkl/opt/bracket.hpp"
#include "y/mkl/opt/minimize.hpp"

namespace upsylon
{
    namespace mkl
    {

        template <typename T>
        class preconditioning
        {
        public:
            typedef lightweight_array<T>            array_type; //!< alias
            typedef const accessible<bool>          flags_type; //!< alias
            typedef typename numeric<T>::function   function1d; //!< alias

            inline explicit preconditioning() :
            nvar(0),
            dmin(0),
            dmax(0),
            used_(0),
            wksp_(0),
            delta(this, & preconditioning::_delta)
            {
            }

            inline virtual ~preconditioning() throw()
            {
            }

            bool run(addressable<T>         &weight,
                     const matrix<T>        &curvature,
                     flags_type             &used)
            {
                assert(weight.size()==used.size());
                assert(weight.size()==curvature.rows);
                assert(weight.size()==curvature.cols);

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                array_type                             &fdiag = aliasing::_(curvature.r_aux1);
                array_type                             &fwksp = aliasing::_(curvature.r_aux2);
                const core::temporary_value<size_t>     nlink(nvar,weight.size());
                const core::temporary_link<flags_type>  ulink(used,&used_);

                //--------------------------------------------------------------
                //
                // capture active diagonal components
                //
                //--------------------------------------------------------------
                size_t nrun = 0;
                for(size_t i=1;i<=nvar;++i)
                {
                    weight[i] = 0;
                    if(used[i])
                    {
                        ++nrun;
                        fdiag[nrun] = curvature[i][i];
                    }

                }

                if(nrun<=0)
                {
                    return true;
                }

                //--------------------------------------------------------------
                //
                // find dmin and dmax using adapted diag[1..nrun]
                //
                //--------------------------------------------------------------
                array_type                              diag(*fdiag,nrun);
                const core::temporary_link<array_type>  dlink(diag,&diag_);

                hsort(diag,comparison::increasing<T>);
                const core::temporary_value<T> kdmin(dmin,diag[1]);
                const core::temporary_value<T> kdmax(dmax,diag[nrun]);

                std::cerr << "curv=" << curvature << std::endl;
                std::cerr << "diag=" << diag << std::endl;

                if(dmin<=0)
                {
                    // singular curvature
                    return false;
                }

                std::cerr << "inv_cond0=" << dmax/dmin << std::endl;

                //--------------------------------------------------------------
                //
                // prepare workspace
                //
                //--------------------------------------------------------------
                array_type                              wksp(*fwksp,nrun);
                const core::temporary_link<array_type>  wlink(wksp,&wksp_);

                {
                    ios::ocstream fp("precond.dat");
                    for(T s2=0;s2<=1.0;s2+=0.01)
                    {
                        fp("%g %g\n",s2,delta(s2));
                    }
                }

                //--------------------------------------------------------------
                //
                // find optimal distribution
                //
                //--------------------------------------------------------------
                triplet<T> S2 = { 0,           -1, 1           };
                triplet<T> D2 = { delta(S2.a), -1, delta(S2.c) };
                bracket::inside(delta, S2, D2);
                const T    s2 = minimize::run(delta, S2, D2);
                std::cerr << "s2=" << s2 << std::endl;
                const T    fac  = (1+s2);
                const T    beta = 1;

                {
                    //size_t j = 0;
                    for(size_t i=1;i<=nvar;++i)
                    {
                        if(used[i])
                        {
                            //++j;
                            weight[i] = (fac * dmax - curvature[i][i])/beta;
                        }
                    }
                }
                std::cerr << "weight=" << weight << std::endl;

                delta(s2);
                std::cerr << "inv_cond=" << wksp[nrun]/wksp[1] << std::endl;


                return true;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(preconditioning);
            size_t          nvar;
            T               dmin;
            T               dmax;
            flags_type     *used_;
            array_type     *diag_;
            array_type     *wksp_;
            function1d      delta;

            inline T _delta(const T s2) throw()
            {
                assert(used_); assert(used_->size()==nvar);
                assert(diag_);
                assert(wksp_);
                assert(diag_->size()==wksp_->size());

                static const T    one  = T(1);
                const T           fac  = one + s2;
                const flags_type &used = *used_;
                array_type       &wksp = *wksp_;
                const array_type &diag = *diag_;
                size_t            j    = 0;

                for(size_t i=1;i<=nvar;++i)
                {
                    if(used[i])
                    {
                        ++j;
                        const T dj = diag[j];
                        wksp[j]    = dj * ( fac * dmax - dj );
                    }
                }
                assert(j==wksp.size());
                hsort(wksp,comparison::increasing<T>);
                return fabs_of(wksp[1]-wksp[j]);
            }


        };
    }
}

#endif
