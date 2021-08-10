#include "y/alchemy/condition.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{

    namespace Alchemy
    {

        const char Condition::xi_[] = "xi_" ;

        Condition:: Condition(const size_t      eq_,
                              const size_t      sp_,
                              const long        nu_,
                              const Library    &lib_,
                              const Equilibria &eqs_) throw() :
        id(nu_>0?GEQ:LEQ),
        eq(eq_),
        sp(sp_),
        nu(0),
        lib(lib_),
        eqs(eqs_)
        {
            assert(nu_!=0);
            assert(lib.frozen);
            assert(eqs.frozen);
            assert(eq>0); assert(eq<=eqs->size());
            assert(sp>0); assert(eq<=lib->size());

            switch(id)
            {
                case GEQ: aliasing::_(nu) = static_cast<size_t>( nu_); break;
                case LEQ: aliasing::_(nu) = static_cast<size_t>(-nu_); break;
            }

        }

        Condition:: ~Condition() throw()
        {

        }

        Condition:: Condition(const Condition &_) throw() :
        id(_.id),
        eq(_.eq),
        sp(_.sp),
        nu(_.nu),
        lib(_.lib),
        eqs(_.eqs)
        {
        }
        
        void Condition:: operator()(Addressable &xi, const Accessible &C) const throw()
        {
            double      &x = xi[eq];
            const double c = C[sp];
            //dspEq(std::cerr); std::cerr <<  " = " << x << " => ";
            switch(id)
            {
                case GEQ:
                    x = max_of(x,-c/nu);
                    break;

                case LEQ:
                    x = min_of(x,c/nu);
                    break;
            }
            //std::cerr << x << std::endl;
        }

    }

}
