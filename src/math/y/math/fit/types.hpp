//! \file
#ifndef Y_MATH_FIT_TYPES_INCLUDED
#define Y_MATH_FIT_TYPES_INCLUDED 1

#include "y/math/fit/variables.hpp"
#include "y/sequence/vector.hpp"
#include "y/container/matrix.hpp"
#include "y/math/fcn/derivative.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            ////////////////////////////////////////////////////////////////////
            //
            //! Type dependent definitions
            //
            ////////////////////////////////////////////////////////////////////
            template <typename T>
            struct Type
            {
                //______________________________________________________________
                //
                // aliases
                //______________________________________________________________
                typedef array<T>  Array;   //!< array interface
                typedef vector<T> Vector;  //!< sequence
                typedef matrix<T> Matrix;  //!< matrix
                typedef functor<T,TL3(T,const Array&,const Variables&)> Function; //!< fit function  prototype
                typedef T (*CFunction)(T,const Array&,const Variables&);          //!< fit CFunction prototype

                //______________________________________________________________
                //
                // Sequential Call
                //______________________________________________________________

                //! interface for sequent call
                class Sequential
                {
                public:
                    const T current; //!< currently called position
                    inline virtual ~Sequential() throw() { (T&)current=0; } //!< cleanup

                    //! make a first call
                    inline T initialize(const T x, const Array &aorg, const Variables &vars)
                    {
                        const T  ans = on_initialize(x,aorg,vars);
                        (T &)current = x;
                        return ans;
                    }

                    //! update value from previous call
                    inline T compute_to(const T x, const Array &aorg, const Variables &vars)
                    {
                        const T  ans = on_compute_to(x,aorg,vars);
                        (T &)current = x;
                        return ans;
                    }


                protected:
                    //! setup
                    inline explicit Sequential() throw() : current(0)
                    {
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Sequential);
                    virtual T on_initialize(const T, const Array &,const Variables &) = 0;
                    virtual T on_compute_to(const T, const Array &,const Variables &) = 0;
                };

                //! lightweight proxy for regular function
                class SequentialFunction : public Sequential
                {
                public:
                    inline explicit SequentialFunction( Function &F ) throw() : host(F) {} //!< setup
                    inline virtual ~SequentialFunction() throw() {}                        //!< cleanup
                    Function &host; //!< reference to external function

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(SequentialFunction);
                    inline virtual T on_initialize(const T x, const Array &aorg, const Variables &vars) { return host(x,aorg,vars); }
                    inline virtual T on_compute_to(const T x, const Array &aorg, const Variables &vars) { return host(x,aorg,vars); }

                };


                //! compute gradient of fit function
                class Gradient : public derivative<T>
                {
                public:
                    typedef typename Type<T>::Sequential Sequential; //!< alias

                    T scale; //!< parameters scaling, default=1e-4

                    //! initialize
                    inline explicit Gradient() : derivative<T>(), scale(1e-4) {}
                    //! destuctor
                    inline virtual ~Gradient() throw() {}

                    //! compute all active components
                    inline void operator()(Array              &dFda,
                                           Sequential         &F,
                                           const T             x,
                                           const Array        &aorg,
                                           const Variables    &vars,
                                           const array<bool>  &used)
                    {
                        assert(dFda.size()==aorg.size());
                        assert(used.size()==aorg.size());
                        Parameters proxy = { x, &aorg, &vars, &F, 0 };
                        size_t    &i     = proxy.ivar;
                        for(i=aorg.size();i>0;--i)
                        {
                            const size_t ii = i;
                            if(used[ii])
                            {
                                dFda[ii] = this->diff(proxy,aorg[ii],scale);
                            }
                            else
                            {
                                dFda[ii] = 0;
                            }
                        }
                    }

                private:
                    struct Parameters
                    {
                        T                xvalue;
                        const Array     *aorg_p;
                        const Variables *vars_p;
                        Sequential      *func_p;
                        size_t           ivar;
                        inline T operator()( T atry )
                        {
                            assert(aorg_p); assert(vars_p); assert(func_p);
                            assert(ivar>0); assert(ivar<=aorg_p->size());

                            Array           &a  = (Array &)(*aorg_p);
                            T               &ai = a[ivar];
                            const T          a0 = ai;
                            try
                            {
                                ai = atry;
                                const T ans = func_p->initialize(xvalue,a,*vars_p);
                                ai = a0;
                                return ans;
                            }
                            catch(...)
                            {
                                ai = a0; throw;
                            }
                        }
                    };
                };
            };

        }
    }

}

#endif


