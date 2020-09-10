
//! \file

#ifndef Y_MATH_FCN_ZIRCON_INCLUDED
#define Y_MATH_FCN_ZIRCON_INCLUDED 1

#include "y/mkl/kernel/svd.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/utils.hpp"
#include "y/sequence/arrays.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/core/temporary-link.hpp"
#include "y/core/temporary-value.hpp"
#include "y/comparison.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace kernel
        {
            class zircon
            {
            public:
                static const char CLID[];
                bool              verbose;

                virtual ~zircon() throw();

            protected:
                explicit zircon() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(zircon);
            };
        }

#define Y_ZIRCON_PRINTLN(MSG) \
do { if(this->verbose) { std::cerr << '[' << CLID << ']' << ' ' << MSG << std::endl; } } while(false)

        template <typename T>
        class zircon : public kernel::zircon
        {

        public:
            typedef typename numeric<T>::vector_field ftype; //!< alias
            typedef typename numeric<T>::jacobian     jtype; //!, alias
            typedef typename numeric<T>::function     function1d; //!< alias
            typedef lightweight_array<T>              array_type; //!< alias


            inline explicit zircon() :
            nvar(0),
            A(5),
            J(),
            u(),
            v(),
            w(    A.next() ),
            step( A.next() ),
            Ftry( A.next() ),
            Xtry( A.next() ),
            Fsqr( A.next() ),
            f_(0),
            F_(0),
            X_(0),
            g(this, & zircon::_g)
            {
            }

            inline virtual ~zircon() throw()
            {
            }

            //! starting with f(F,X) precomputed
            bool cycle( addressable<T> &F, addressable<T> &X, ftype &f, jtype &fjac )
            {
                assert( F.size() == X.size() );
                //--------------------------------------------------------------
                //
                // prepare topology
                //
                //--------------------------------------------------------------
                core::temporary_value<size_t>         nlink(nvar,X.size());
                core::temporary_link<ftype>           flink(f,&f_);
                core::temporary_link<addressable<T> > Xlink(X,&X_);
                core::temporary_link<addressable<T> > Flink(F,&F_);



                A.acquire(nvar);
                J.make(nvar,nvar);
                u.make(nvar,nvar);
                v.make(nvar,nvar);

                fjac(J,X);
                Y_ZIRCON_PRINTLN("<cycle>");
                Y_ZIRCON_PRINTLN("X="<<X);
                Y_ZIRCON_PRINTLN("F="<<F);
                Y_ZIRCON_PRINTLN("J="<<J);
                u.assign(J);

                //--------------------------------------------------------------
                //
                // use SVD
                //
                //--------------------------------------------------------------
                if(!svd::build(u,w,v))
                {
                    Y_ZIRCON_PRINTLN("svd failure");
                    return false;
                }

                //--------------------------------------------------------------
                //
                // try to find a step
                //
                //--------------------------------------------------------------
                const size_t ker = __find<T>::truncate(*w,nvar);
                Y_ZIRCON_PRINTLN("u="<<u);
                Y_ZIRCON_PRINTLN("w="<<w);
                Y_ZIRCON_PRINTLN("v="<<v);
                Y_ZIRCON_PRINTLN("ker="<<ker);
                if(ker>0)
                {
                    const size_t img = nvar-ker;
                    if(img<=0)
                    {
                        Y_ZIRCON_PRINTLN("singular matrix");
                        return false;
                    }

                    if(!reducedSpanStep(img)) return false;
                    //if(!inNullSpaceStep(ker)) return false;

                }
                else
                {
                    quasiNewtonStep();
                }

                const T g0 = __g(F);  Y_ZIRCON_PRINTLN("g0="<<g0);
                if(g0<=0)
                {
                    Y_ZIRCON_PRINTLN("null RMS");
                    return true;
                }
                const T g1 = g(1);

                Y_ZIRCON_PRINTLN("g1="<<g1);
                {
                    const string fn = vformat("zircon%u.dat",unsigned(ker));
                    ios::ocstream fp(fn);
                    for(int i=-1000;i<=1000;++i)
                    {
                        const T x = T(i)/1000;
                        fp("%g %g\n",x,g(x));
                    }
                }


                return false;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(zircon);
            size_t           nvar;
            arrays<T>        A;
            matrix<T>        J;
            matrix<T>        u;
            matrix<T>        v;
            array_type      &w;
            array_type      &step;
            array_type      &Ftry;
            array_type      &Xtry;
            array_type      &Fsqr;
            ftype           *f_;
            addressable<T>  *F_;
            addressable<T>  *X_;
            function1d       g;

            inline T __g(const accessible<T> &F) const throw()
            {
                assert(F.size()==nvar);
                for(size_t i=nvar;i>0;--i)
                {
                    Fsqr[i] = square_of(F[i]);
                }
                hsort(Fsqr,comparison::decreasing<T>);
                T sum = 0;
                for(size_t i=nvar;i>0;--i)
                {
                    sum += Fsqr[i];
                }
                return T(0.5) * sum;
            }

            inline T _g(const T lambda)
            {
                quark::muladd(Xtry,*X_,lambda,step);
                (*f_)(Ftry,Xtry);
                return __g(Ftry);
            }

            inline void quasiNewtonStep()
            {
                svd::solve(u,w,v,*F_,step);
                quark::neg(step);
                Y_ZIRCON_PRINTLN("step="<<step);
            }

            inline bool inNullSpaceStep(const size_t ker)
            {
                matrix<T> S(nvar,ker);
                for(size_t i=1,s=1;i<=nvar;++i)
                {
                    if(fabs_of(w[i])<=0)
                    {
                        for(size_t k=nvar;k>0;--k)
                        {
                            S[k][s] = v[k][i];
                        }
                        ++s;
                    }
                }
                __find<T>::truncate(S);
                Y_ZIRCON_PRINTLN("S="<<S);
                matrix<T> tSJS(ker,ker);
                matrix<T> tS(S,matrix_transpose);
                matrix<T> JS(nvar,ker);
                quark::mmul(JS,J,S);
                quark::mmul(tSJS,tS,JS);
                Y_ZIRCON_PRINTLN("tSJS=" << tSJS);
                if(!LU::build(tSJS))
                {
                    Y_ZIRCON_PRINTLN("invalid reduced problem");
                    return false;
                }
                vector<double> mu(ker,0);
                quark::mulneg(mu,tS,*F_);
                LU::solve(tSJS,mu);
                Y_ZIRCON_PRINTLN("mu=" <<mu);
                quark::mul(step,S,mu);
                Y_ZIRCON_PRINTLN("step=" << step);
                return true;
            }


            inline bool reducedSpanStep(const size_t img)
            {
                matrix<T> S(nvar,img);
                for(size_t i=1,s=1;i<=nvar;++i)
                {
                    if(fabs_of(w[i])>0)
                    {
                        for(size_t k=nvar;k>0;--k)
                        {
                            S[k][s] = v[k][i];
                        }
                        ++s;
                    }
                }
                __find<T>::truncate(S);
                Y_ZIRCON_PRINTLN("S="<<S);
                matrix<T> tSJS(img,img);
                matrix<T> tS(S,matrix_transpose);
                matrix<T> JS(nvar,img);
                quark::mmul(JS,J,S);
                quark::mmul(tSJS,tS,JS);
                Y_ZIRCON_PRINTLN("tSJS=" << tSJS);
                if(!LU::build(tSJS))
                {
                    Y_ZIRCON_PRINTLN("invalid reduced problem");
                    return false;
                }
                vector<double> mu(img,0);
                quark::mulneg(mu,tS,*F_);
                LU::solve(tSJS,mu);
                Y_ZIRCON_PRINTLN("mu=" <<mu);
                quark::mul(step,S,mu);
                Y_ZIRCON_PRINTLN("step=" << step);
                return true;
            }

        };

    }

}

#endif
