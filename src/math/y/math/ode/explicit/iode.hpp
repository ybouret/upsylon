//! \file
#ifndef Y_MATH_ODE_ExODE_INCLUDED
#define Y_MATH_ODE_ExODE_INCLUDED 1

#include "y/math/ode/explicit/lssi.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {

            //! Integrated Ordinary Differential Equation
            template <typename T>
            class ExODE
            {
            public:

                //! the differential problem to solve
                class ProblemType : public counted_object
                {
                public:
                    inline virtual ~ProblemType() throw() {}

                    //! problem dimension
                    virtual size_t dimension() const throw() = 0;

                    //! initialize internal variables at initial coordinate
                    virtual void setup( array<T> & ) = 0;

                    //! return linear=starting point/log=first log point
                    virtual T init() const throw() = 0;

                    //! safe coordinate step
                    virtual T safe() const throw() = 0;

                    //! return final result at final time
                    virtual T finalize( const array<T> &, const T ) const = 0;

                    //! interface
                    virtual void compute( array<T> &, T, const array<T> &) = 0;

                protected:
                    inline explicit ProblemType() throw() {}

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(ProblemType);
                };

                typedef arc_ptr<ProblemType>         Problem;
                typedef typename Field<T>::Equation  Equation;
                typedef typename Field<T>::Callback  Callback;

                typedef ExplicitSolverInterface<T>   ESI;
                typedef typename ESI::Pointer        Solver;


                virtual ~ExODE() throw() {}


                //! perform integration init->value
                inline T at(const T value)
                {
                    P->setup(Y);
                    LSSI::LinearRun(*S,E,Y,P->init(),value,P->safe(),NULL);
                    return P->finalize(Y,value);
                }

                //! perform integration init->value
                inline T at_log(const T lnValue)
                {
                    P->setup(Y);
                    LSSI::LogarithmicRun(*S,E,Y,P->init(),lnValue,P->safe(),NULL);
                    return P->finalize(Y,exp(lnValue));
                }



                inline explicit ExODE(const Solver  &solver,
                                      const Problem &problem) :
                S(solver),
                P(problem),
                E( & *P, & ProblemType::compute ),
                Y( P->dimension(),0)
                {
                    S->start( Y.size() );
                }


            private:
                Solver         S;
                Problem        P;
                Equation       E;
                vector<T>      Y;


                Y_DISABLE_COPY_AND_ASSIGN(ExODE);
            };

        }
    }
}

#endif

