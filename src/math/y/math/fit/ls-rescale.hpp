
//! \file
#ifndef Y_MATH_FIT_LS_RESCALE_INCLUDED
#define Y_MATH_FIT_LS_RESCALE_INCLUDED 1

#include "y/math/fit/ls.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            class RescalingInfo : public counted_object
            {
            public:
                static const size_t NVAR=3;
                const Variables     vars;
                explicit RescalingInfo();
                virtual ~RescalingInfo() throw();

                static const char _coeff[];
                static const char _scale[];
                static const char _shift[];

                bool & use_coeff() throw() { return vars(used,_coeff); }
                bool & use_scale() throw() { return vars(used,_scale); }
                bool & use_shift() throw() { return vars(used,_shift); }

            protected:
                vector<bool> used;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(RescalingInfo);
            };

            template <typename T>
            class Rescaling : public RescalingInfo
            {
            public:
                typedef typename Type<T>::Function Function; //!< alias

                explicit Rescaling() : RescalingInfo(),
                sorg(NVAR,0),
                serr(NVAR,0)
                {
                }

                virtual ~Rescaling() throw()
                {
                }

                array<T> &values() throw() { return sorg; }
                array<T> &errors() throw() { return serr; }

                template <typename FUNC>
                class Wrapper
                {
                public:
                    FUNC &F;

                    inline Wrapper( FUNC &f ) throw() : F(f) {}
                    inline ~Wrapper() throw() {}

                    inline T Compute( T x, const array<T> &A, const Variables &V )
                    {
                        const T coeff = V(A,_coeff);
                        const T scale = V(A,_scale);
                        const T shift = V(A,_shift);
                        return coeff * F( scale*(x-shift) );
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Wrapper);
                };


                template <typename FUNC>
                inline
                bool update(LeastSquares<T> &ls,
                            Sample<T>       &input,
                            FUNC            &f,
                            bool             init=false)
                {
                    // prepare sample
                    Sample<T> sample(input.X,input.Y,input.Yf);
                    sample.variables = this->vars;

                    // prepare wrapper
                    Wrapper<FUNC> w (f);
                    Function      F( &w, & Wrapper<FUNC>::Compute );

                    this->vars(sorg,_coeff) = 1;
                    this->vars(sorg,_scale) = 1;
                    this->vars(sorg,_shift) = 0;

                    if( init )
                    {
                        
                    }
                    std::cerr << "sorg=" << sorg << std::endl;
                    std::cerr << "used=" << used << std::endl;
                    
                    return ls.fit(sample, F, sorg, serr, used);

                }


            protected:
                vector<T> sorg;
                vector<T> serr;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rescaling);
            };




        }

    }

}

#endif

