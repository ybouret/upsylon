//! \file

#ifndef Y_MATH_ADJUST_SAMPLE_TYPE_INCLUDED
#define Y_MATH_ADJUST_SAMPLE_TYPE_INCLUDED 1

#include "y/math/adjust/sample/info.hpp"
#include "y/math/adjust/sequential/function.hpp"
#include "y/math/adjust/sequential/gradient.hpp"
#include "y/math/kernel/atom.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            template <typename> class Sample;       //!< forward declaration
            template <typename> class LeastSquares; //!< forward declaration


#define Y_MATH_ADJUST_VALIDATE TL1(const Context &)

            //==================================================================
            //
            //
            //! interface for adjustable sample
            //
            //
            //==================================================================
            template <typename T>
            class SampleType : public SampleInfo
            {
            public:
                //==============================================================
                //
                // types and definitions
                //
                //==============================================================
                typedef typename Type<T>::Array      Array;        //!< alias
                typedef typename Type<T>::Matrix     Matrix;       //!< alias
                typedef typename Type<T>::Function   Function;     //!< alias
                typedef typename Type<T>::Parameters Parameters;   //< alias
                typedef Sample<T>                   *Address;      //!< alias
                typedef accessible<Address>          Addresses;    //!< alias

                //! context to validate/modify fit
                class Context
                {
                public:
                    inline ~Context() throw() {}

                    inline Context(const SampleType<T> & _self,
                                   const Parameters    & _aorg,
                                   const Flags         & _used) :
                    _data(),
                    data(_data),
                    aorg( _aorg ),
                    used( _used ),
                    cycle(0)
                    {
                        _self.collect(_data);
                    }
                    
                private:
                    vector<Address,Allocator> _data;
                public:
                    const Addresses  &data;
                    const Parameters &aorg;
                    const Flags      &used;
                    const size_t      cycle;


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Context);
                };

                typedef functor<bool,Y_MATH_ADJUST_VALIDATE> Validate;



                //==============================================================
                //
                // virtual interface
                //
                //==============================================================

                //! cleanup
                inline virtual ~SampleType() throw() {}

                //! compute with a sequential function
                virtual T compute(Sequential<T> &F, const Array &aorg) const = 0;

                //! full computation with a sequential function
                virtual T  computeAndUpdate(Matrix          &alpha,
                                            Array           &beta,
                                            Sequential<T>   &F,
                                            const Array     &aorg,
                                            const Flags     &used,
                                            Gradient<T>     &grad) const = 0;

                //! flags activation
                virtual void activate( addressable<bool> &target, const accessible<bool> &source ) const = 0;




                //==============================================================
                //
                // non virtual interface
                //
                //==============================================================

                //! initialize matrix/vector from used statius
                inline void initialize(Matrix      &alpha,
                                       Array       &beta,
                                       const Flags &used ) const throw()
                {
                    assert( alpha.rows  == alpha.cols  );
                    assert( beta.size() == alpha.rows );
                    assert( used.size() == beta.size() );
                    assert( variables.sweep() <= used.size() );

                    // assuming no one is used
                    atom::ld(beta,0);
                    alpha.Id();

                    // cleanup used variables
                    size_t                    nv = variables.size();
                    Variables::const_iterator it = variables.begin();
                    while(nv>0)
                    {
                        const size_t i = (**it).index();
                        if( used[i] )
                        {
                            alpha[i][i] = 0;
                        }
                        --nv;
                        ++it;
                    }
                }
                //! wrapper to use a C++ function
                inline T compute_( Function &F, const Array &aorg ) const
                {
                    SequentialFunction<T> call(F);
                    return compute(call,aorg);
                }

                //! compute D2 overloaded
                inline T computeD2(Sequential<T>   &F,
                                   const Array     &aorg) const
                {
                    return compute(F,aorg);
                }



                //! compute D2 overloaded
                inline T computeD2(Matrix          &alpha,
                                   Array           &beta,
                                   Sequential<T>   &F,
                                   const Array     &aorg,
                                   const Flags     &used,
                                   Gradient<T>     &grad) const
                {
                    initialize(alpha,beta,used);
                    const T D2 = computeAndUpdate(alpha,beta,F,aorg,used,grad);
                    Type<T>::Regularize(alpha);
                    return D2;
                }
                
              


            protected:
                //! setup
                inline explicit SampleType() throw() : SampleInfo() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleType);
                friend class LeastSquares<T>;

                //! multiple samples collection
                virtual void collect( sequence<Address> & ) const = 0;
            };

        }

    }
}


#endif

