
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

            
            
            class Rescale : public counted_object
            {
            public:
                enum Setup
                {
                    StartWithIdentity,
                    MinimizeAmplitude,
                    WouldTakeNextStep
                };
                
                static const size_t NVAR=3;
                
                const Variables     vars;
                
                explicit Rescale();
                virtual ~Rescale() throw();

                static const char _coeff[];
                static const char _scale[];
                static const char _shift[];

                bool & use_coeff() throw() { return vars(used,_coeff); }
                bool & use_scale() throw() { return vars(used,_scale); }
                bool & use_shift() throw() { return vars(used,_shift); }

            protected:
                vector<bool> used;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rescale);
            };

            template <typename T>
            class Rescaling : public Rescale
            {
            public:
                typedef typename Type<T>::Function Function; //!< alias

                explicit Rescaling() : Rescale(),
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
                            const Setup      setup )
                {
                    // prepare sample
                    Sample<T> sample(input.X,input.Y,input.Yf);
                    sample.variables = vars;

                    // prepare wrapper
                    Wrapper<FUNC> w (f);
                    Function      F( &w, & Wrapper<FUNC>::Compute );

                   

                    switch( setup )
                    {
                        case StartWithIdentity:
                            vars(sorg,_coeff) = 1;
                            vars(sorg,_scale) = 1;
                            vars(sorg,_shift) = 0;
                            break;
                            
                        case WouldTakeNextStep:
                            break;
                            
                        case MinimizeAmplitude: {
                            vars(sorg,_scale) = 1;
                            vars(sorg,_shift) = 0;
                            T num = 0;
                            T den = 0;
                            for(size_t i=sample.count();i>0;--i)
                            {
                                const T yi = sample.Y[i];
                                const T fi = f(sample.X[i]);
                                num += yi*fi;
                                den += fi*fi;
                            }
                            if(den<=0)
                            {
                                throw exception("Fit::Rescale.MimimizeAmplitude Failure");
                            }
                            vars(sorg,_coeff) = num/den;
                        } break;
                            
                    }
                    
                    std::cerr << "sorg=" << sorg << std::endl;
                    std::cerr << "used=" << used << std::endl;
                    
                    return ls.fit(sample, F, sorg, serr, used);
                }
                
                template <typename FUNC>
                inline T eval( FUNC &f, const T x) const
                {
                    Wrapper<FUNC> w (f);
                    Function      F( &w, & Wrapper<FUNC>::Compute );
                    return        F(x,sorg,vars);
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

