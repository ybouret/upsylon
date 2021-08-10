
//! \file

#ifndef Y_ALCHEMY_CONDITION_INCLUDED
#define Y_ALCHEMY_CONDITION_INCLUDED 1

#include "y/alchemy/library.hpp"
#include "y/alchemy/equilibria.hpp"

namespace upsylon
{
    namespace Alchemy
    {


        class Reactor;      //!< forward

        //______________________________________________________________________
        //
        //
        //! primary condition
        //
        //______________________________________________________________________
        class Condition
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________

            //! limiting condition
            enum Type
            {
                LEQ, //!< xi[eq] * nu <=   A[sp]
                GEQ  //!< xi[eq] * nu >=  -A[sp]
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            Condition(const size_t,
                      const size_t,
                      const long,
                      const Library    &,
                      const Equilibria &) throw();   //!< setup
            ~Condition() throw();                 //!< cleanup
            Condition(const Condition &) throw(); //!< copy

            const Type        id;  //!< type of condition
            const size_t      eq;  //!< equilibrium index
            const size_t      sp;  //!< species     index
            const size_t      nu;  //!< positive coefficient
            const Library    &lib; //!< to read species
            const Equilibria &eqs; //!< to read equilibrium

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! regulate extent[eq] according to concentration[sp]
            void operator()(Addressable &xi, const Accessible &C) const throw();


            //! default output
            template <typename OSTREAM> inline friend
            OSTREAM &operator<<(OSTREAM &os, const Condition c)
            {
                os << '\n' << '{';
                c.dspEq(os);
                switch(c.id)
                {
                    case LEQ: os << " <=  "; break;
                    case GEQ: os << " >= -"; break;
                }
                c.dspSp(os);
                os << '}';
                return os;
            }

            //! specific output
            template <typename OSTREAM> inline
            OSTREAM & show(OSTREAM &os, const Accessible &C) const
            {
                dspEq(os);
                switch(id)
                {
                    case LEQ: os << vformat(" <= %.15g", C[sp]); break;
                    case GEQ: os << vformat(" >= %.15g",-C[sp]); break;
                }
                return os;
            }


        private:
            Y_DISABLE_ASSIGN(Type);
            
            template <typename OSTREAM> inline
            OSTREAM &dspNu(OSTREAM &os) const { if(nu<1) os << vformat("%lu ", (unsigned long)nu ); return os; }

            template <typename OSTREAM> inline
            OSTREAM & dspEq(OSTREAM &os) const {
                const string &eid = eqs(eq).name;
                dspNu(os) << "xi_" << eid;
                for(size_t i=eid.size();i<eqs.enw;++i) os << ' ';
                return os;
            }

            template <typename OSTREAM> inline
            OSTREAM & dspSp(OSTREAM &os) const {
                return lib.print(os, *(lib->fetch(sp-1)));
            }

        };

        typedef vector<Condition,Allocator> Conditions; //!< alias

    }

}

#endif

