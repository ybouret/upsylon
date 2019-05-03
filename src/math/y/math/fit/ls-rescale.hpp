
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
            
            //! base class to fit rescaling parameters
            class Rescale : public counted_object
            {
            public:
                //! way to initialize parameters
                enum Setup
                {
                    StartWithIdentity, //!< coeff=1,scale=1,shift=0
                    MinimizeAmplitude, //!< coeff=min,scale=1,shift=0
                    WouldTakeNextStep  //!< do nothing
                };
                
                static const size_t NVAR=3; //!< three parameters
                
                explicit Rescale();         //!< setup
                virtual ~Rescale() throw(); //!< cleanup
                
                static const char _coeff[]; //!< "coeff"
                static const char _scale[]; //!< "scale"
                static const char _shift[]; //!< "shift"
                
                bool & use_coeff() throw(); //!< used["coeff"]
                bool & use_scale() throw(); //!< used["scale"]
                bool & use_shift() throw(); //!< used["shift"]
                
                const Variables     vars; //!< initialized once and for all
                
            protected:
                vector<bool> used; //!< used parameters for fit
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rescale);
            };
            
            //! Rescaling sessions
            template <typename T>
            class Rescaling : public Rescale
            {
            public:
                typedef typename Type<T>::Function Function; //!< alias
                
                //! initialize
                explicit Rescaling() : Rescale(), sorg(NVAR,0), serr(NVAR,0) { }
                virtual ~Rescaling() throw() {} //!< cleanup
                
                array<T> &values() throw() { return sorg; } //!< internal values
                array<T> &errors() throw() { return serr; } //!< internal errors
                
                //! update parameters  from an input sample and a 1d function
                template <typename FUNC>
                inline bool update(LeastSquares<T> &ls,
                                   Sample<T>       &input,
                                   FUNC            &f,
                                   const Setup      setup )
                {
                    //__________________________________________________________
                    //
                    // prepare sample
                    //__________________________________________________________
                    Sample<T> sample(input.X,input.Y,input.Yf);
                    sample.variables = vars;
                    
                    //__________________________________________________________
                    //
                    // prepare wrapper to be called by LeastSquares
                    //__________________________________________________________
                    Wrapper<FUNC> w (f);
                    Function      F( &w, & Wrapper<FUNC>::Compute );
                    
                    //__________________________________________________________
                    //
                    // initialize variables
                    //__________________________________________________________
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
                    
                    return ls.fit(sample, F, sorg, serr, used);
                }
                
                //! helper to evaluate functions
                template <typename FUNC>
                inline T operator()( FUNC &f, const T x) const
                {
                    Wrapper<FUNC> w (f);
                    Function      F( &w, & Wrapper<FUNC>::Compute );
                    return        F(x,sorg,vars);
                }
                
                
            protected:
                vector<T> sorg; //!< scaling values
                vector<T> serr; //!< scaling errors
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rescaling);
                template <typename FUNC> class Wrapper
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
            };
            
            
            
            
        }
        
    }
    
}

#endif

