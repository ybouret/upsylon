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

        //! preconditioning matrices
        template <typename T>
        class preconditioning
        {
        public:
            typedef lightweight_array<T>            array_type; //!< alias
            typedef const accessible<bool>          flags_type; //!< alias
            typedef typename numeric<T>::function   function1d; //!< alias

            //! setup
            inline explicit preconditioning() :
            nvar(0),
            dmin(0),
            dmax(0),
            used_(0),
            diag_(0),
            wksp_(0),
            delta(this, & preconditioning::_delta)
            {
            }

            //! cleanup
            inline virtual ~preconditioning() throw()
            {
            }

            //! find diagonal weight
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
                const core::temporary_value<T> kdrho(drho,_drho());

                std::cerr << "curv=" << curvature << std::endl;
                std::cerr << "diag=" << diag << std::endl;
                std::cerr << "dmin=" << dmin << std::endl;
                std::cerr << "dmax=" << dmax << std::endl;
                std::cerr << "drho=" << drho << std::endl;


                if(dmin<=0)
                {
                    // singular curvature
                    return false;
                }

                std::cerr << "inv_cond0=" << dmax/dmin << std::endl;
                array_type                              wksp(*fwksp,nrun);
                const core::temporary_link<array_type>  wlink(wksp,&wksp_);

                {
                    ios::ocstream fp("omega.dat");
                    for(T p=1.1*dmax;p<=3*dmax;p+=0.001)
                    {
                        const T d = delta(p);
                        fp("%.20g %.20g\n",p,d);
                    }
                }

                return true;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(preconditioning);
            size_t          nvar;
            T               dmin;
            T               dmax;
            T               drho;
            flags_type     *used_;
            array_type     *diag_;
            array_type     *wksp_;
            function1d      delta;

            inline T _drho() throw()
            {
                assert(diag_);
                const array_type &diag = *diag_;
                const size_t      nrun = diag.size();
                T sum = 0, sum2=0;
                for(size_t i=1;i<=nrun;++i)
                {
                    const T d = diag[i];
                    sum += d;
                    sum2 += square_of(d);
                }
                return sum2/sum;
            }



            inline T _delta(const T p) throw()
            {
                assert(used_); assert(used_->size()==nvar);
                assert(diag_);
                assert(wksp_);
                assert(diag_->size()==wksp_->size());

                const T           beta = p - drho;
                array_type       &wksp = *wksp_;
                const array_type &diag = *diag_;
                const size_t      nrun = wksp.size();

                for(size_t j=nrun;j>0;--j)
                {
                    const T dj = diag[j];
                    const T wj = (p-dj)/beta;
                    wksp[j]    = dj * wj;
                }
                hsort(wksp,comparison::increasing<T>);
                return fabs_of(wksp[nrun]-wksp[1]);
            }


        };
    }
}

#endif
