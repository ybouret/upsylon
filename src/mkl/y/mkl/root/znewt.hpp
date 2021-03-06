
//! \file

#ifndef Y_MATH_FCN_ZNEWT_INCLUDED
#define Y_MATH_FCN_ZNEWT_INCLUDED 1

#include "y/sequence/arrays.hpp"
#include "y/mkl/utils.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/tao.hpp"
#include "y/momentary/link.hpp"
#include "y/momentary/value.hpp"
#include "y/comparison.hpp"

namespace upsylon
{
    namespace mkl
    {

        namespace kernel
        {

            //! base class for znewt
            class znewt
            {
            public:
                virtual ~znewt() throw(); //!< cleanup
                explicit znewt() throw(); //!< setup

                bool verbose;             //!< verbosity
                static const char CLID[]; //!< "znewt"
                static const char BACK[]; //!< "backtracking: "

            private:
                Y_DISABLE_COPY_AND_ASSIGN(znewt);
            };
        };

        //! inline verbose
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
            typedef typename numeric<T>::vector_field ftype;      //!< alias
            typedef typename numeric<T>::function     function1d; //!< alias
            typedef lightweight_array<T>              array_type; //!< alias
            typedef typename numeric<T>::jacobian     jtype;      //!< alias
            inline explicit znewt() :
            J(),
            A(5),
            nvar(0),
            step( A.next() ),
            Xtry( A.next() ),
            Ftry( A.next() ),
            Fsqr( A.next() ),
            f_(0),
            X_(0),
            g(this, & znewt::g_ )
            {
            }

            inline virtual ~znewt() throw()
            {
            }

            //! start from f(F,X)
            inline bool cycle(addressable<T> &F,
                              addressable<T> &X,
                              ftype          &f,
                              jtype         &fjac)
            {
                static const T one        = 1;
                static const T alpha      = 1e-4;
                static const T lambda_min = 0.1;

                assert(F.size()==X.size());

                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                momentary_value<size_t>          nlink(nvar,X.size());
                momentary_link< addressable<T> > xlink(X,&X_);
                momentary_link<ftype>            flink(f,&f_);
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
                tao::neg(step,F);
                LU::solve(J,step);
                Y_MKL_ZNEWT_PRINTLN("step="<<step);

                //--------------------------------------------------------------
                // compute global decreasing setup
                //--------------------------------------------------------------
                const T g0    = __g(F);
                Y_MKL_ZNEWT_PRINTLN("g0="<<g0);
                if(g0<=0)
                {
                    Y_MKL_ZNEWT_PRINTLN("|F|^2<=0");
                    return true;
                }
                const T slope = g0+g0;        // - g'(0)
                const T sigma = alpha*slope;  // constraint
                Y_MKL_ZNEWT_PRINTLN("slope="<<slope);

                //--------------------------------------------------------------
                // ensure g(lamba) <= g0 - sigma*lambda, sigma=alpha*slope
                // starting at lambda=1
                //--------------------------------------------------------------

                

                T lambda = 1;
                while(true)
                {
                    T gtry   = g(lambda);
                    if(gtry<=g0-sigma*lambda)
                    {
                        Y_MKL_ZNEWT_PRINTLN("accept lambda=" << lambda);
                        break;
                    }
                    else
                    {
                        const T beta = (gtry - g0*(one-(lambda+lambda) ))/(lambda*lambda);
                        if(beta<=0)
                        {
                            Y_MKL_ZNEWT_PRINTLN("invalid local curvature");
                            return false;
                        }
                        const T lambda_try = (slope*(one-alpha))/beta;
                        Y_MKL_ZNEWT_PRINTLN( BACK << "curvature  = "<<beta);
                        Y_MKL_ZNEWT_PRINTLN( BACK << "lambda_try = "<<lambda_try);
                        if(lambda_try>=lambda)
                        {
                            Y_MKL_ZNEWT_PRINTLN( BACK << "stalled!");
                            return false;
                        }
                        lambda = lambda_try;
                        if(lambda<=lambda_min)
                        {
                            Y_MKL_ZNEWT_PRINTLN( BACK << "reached minimum");
                            lambda = lambda_min;
                            (void) g(lambda);
                            break;
                        }

                    }
                }
                Y_MKL_ZNEWT_PRINTLN("lambda="<<lambda);
                // check convergence
                const bool xcvg = __find<T>::convergence(X,Xtry);
                const bool fcvg = __find<T>::convergence(F,Ftry);
                return xcvg||fcvg;
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
            ftype          *f_;   //!< temporaty current field
            addressable<T> *X_;   //!< temporary current position
            function1d      g;

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

            inline T g_(T lam)
            {
                assert(X_);
                tao::muladd(Xtry,*X_,lam,step);
                (*f_)(Ftry,Xtry);
                return __g(Ftry);
            }

        };


    }

}

#endif
