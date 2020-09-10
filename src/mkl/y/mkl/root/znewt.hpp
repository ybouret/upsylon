
//! \file

#ifndef Y_MATH_FCN_ZNEWT_INCLUDED
#define Y_MATH_FCN_ZNEWT_INCLUDED 1

#include "y/mkl/fcn/jacobian.hpp"
#include "y/sequence/arrays.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/core/temporary-link.hpp"
#include "y/core/temporary-value.hpp"
#include "y/comparison.hpp"
#include "y/ios/ocstream.hpp"

namespace upsylon
{
    namespace mkl
    {

        namespace kernel
        {

            class znewt
            {
            public:
                virtual ~znewt() throw();
                explicit znewt() throw();

                bool verbose;
                static const char CLID[]; //!< "znewt"
            private:
                Y_DISABLE_COPY_AND_ASSIGN(znewt);
            };
        };

#define Y_MKL_ZNEWT_PRINTLN(MSG) do { if(this->verbose) { std::cerr << '[' << CLID << ']' << ' ' << MSG << std::endl; } } while(false)

        //______________________________________________________________________
        //
        //
        //! a Newton's method
        //
        //______________________________________________________________________
        template <typename T>
        class znewt : public kernel::znewt
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef typename numeric<T>::vector_field field_type; //!< alias
            typedef typename numeric<T>::function     function1d; //!< alias
            typedef lightweight_array<T>              array_type; //!< alias
            inline explicit znewt() :
            J(),
            A(5),
            nvar(0),
            step( A.next() ),
            Xtry( A.next() ),
            Ftry( A.next() ),
            Fsqr( A.next() ),
            _f(0),
            _X(0),
            g(this, & znewt::g_ )
            {
            }

            inline virtual ~znewt() throw()
            {
            }

            //! start from f(F,X)
            inline bool cycle(addressable<T> &F,
                              addressable<T> &X,
                              field_type     &f,
                              jacobian<T>    &fjac)
            {
                static const T ftol       = numeric<T>::ftol;
                static const T one        = 1;
                static const T alpha      = 0.8;
                static const T lambda_min = 0.1;

                assert(F.size()==X.size());

                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                core::temporary_value<size_t>          nlink(nvar,X.size());
                core::temporary_link< addressable<T> > xlink(X,&_X);
                core::temporary_link< field_type     > flink(f,&_f);
                assert(nvar==X.size());
                J.make(nvar,nvar);
                A.acquire(nvar);

                //--------------------------------------------------------------
                // compute Newton's step
                //--------------------------------------------------------------
                fjac(J,X);
                Y_MKL_ZNEWT_PRINTLN("X="<<X);
                Y_MKL_ZNEWT_PRINTLN("F="<<F);
                Y_MKL_ZNEWT_PRINTLN("J="<<J);

                if( !LU::build(J) ) return false;
                quark::neg(step,F);
                LU::solve(J,step);
                Y_MKL_ZNEWT_PRINTLN("step="<<step);

                //--------------------------------------------------------------
                // compute global decreasing setup
                //--------------------------------------------------------------
                const T g0    = g(0);
                Y_MKL_ZNEWT_PRINTLN("g0="<<g0);
                if(g0<=0)
                {
                    Y_MKL_ZNEWT_PRINTLN("spurious success");
                    return true;
                }
                const T slope = g0+g0; // - g'(0)
                const T sigma = alpha*slope;
                Y_MKL_ZNEWT_PRINTLN("slope="<<slope);
                //--------------------------------------------------------------
                // ensure g(lamba) <= g0 - sigma*lambda, sigma=alpha*slope
                //--------------------------------------------------------------


                T lambda = 1;
                while(true)
                {
                    T gtry   = g(lambda);
                    if(gtry<=g0-sigma*lambda)
                    {
                        break;
                    }
                    else
                    {
                        const T beta = (gtry - g0*(one-(lambda+lambda) ))/(lambda*lambda);
                        Y_MKL_ZNEWT_PRINTLN("backtracking: beta   = "<<beta);
                        if(beta<=0)
                        {
                            Y_MKL_ZNEWT_PRINTLN("spurious failure");
                            return false;
                        }
                        lambda = (slope*(one-alpha))/beta;
                        Y_MKL_ZNEWT_PRINTLN("backtracking: lambda = "<<lambda);
                        if(lambda<=lambda_min)
                        {
                            lambda = lambda_min;
                            (void) g(lambda);
                            break;
                        }
                        if(true)
                        {

                            ios::ocstream fp("znewt.dat");
                            for(int i=0;i<=100;++i)
                            {
                                const T x = T(i)/100;
                                fp("%.15e %.15e\n", x,g(x));
                            }
                            exit(1);
                        }
                    }
                }
                Y_MKL_ZNEWT_PRINTLN("lambda="<<lambda);



                //--------------------------------------------------------------
                // take full step whilst checking convergence
                //--------------------------------------------------------------
                bool converged = true;
                for(size_t i=nvar;i>0;--i)
                {
                    const T x_old = X[i];
                    const T x_new = (X[i]  += step[i]);
                    const T dx    = fabs_of(x_new-x_old);
                    if( dx > ftol * fabs_of(x_new) ) converged=false;
                }

                f(F,X);
                return converged;
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(znewt);
            matrix<T>       J;    //!< jacobian matrix
            arrays<T>       A;    //!< localarrays
            size_t          nvar; //!< temporary nvar
            array_type     &step; //!< full Newton's step
            array_type     &Xtry; //!< trial position
            array_type     &Ftry; //!< trial f
            array_type     &Fsqr; //!< temporary square
            field_type     *_f;   //!< temporaty current field
            addressable<T> *_X;   //!< temporary current position
            function1d       g;

            inline T __g(const array_type &F) const throw()
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

            inline T g_(T lam)
            {
                assert(_X);
                quark::muladd(Xtry,*_X,lam,step);
                (*_f)(Ftry,Xtry);
                return __g(Ftry);
            }

        };


    }

}

#endif
