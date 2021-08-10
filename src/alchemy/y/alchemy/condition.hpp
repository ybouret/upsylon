
//! \file

#ifndef Y_ALCHEMY_CONDITION_INCLUDED
#define Y_ALCHEMY_CONDITION_INCLUDED 1

#include "y/alchemy/library.hpp"
#include "y/alchemy/equilibria.hpp"
#include "y/container/tuple.hpp"

namespace upsylon
{
    namespace Alchemy
    {


        class Reactor;      //!< forward
        
        Y_PAIR_DECL(STANDARD,Limit,bool,on,double,xi);
        inline      Limit() throw() : on(false), xi(0) {}
        inline void reset() throw() { on=false; xi=0; }
        Y_PAIR_END();

        //! Limits possibilities
        enum LimitsState
        {
            LimitsUnbounded, //!< lower.off and upper.off
            LimitsUpperOnly, //!< lower.off and upper.on
            LimitsLowerOnly, //!< lower.on  and upper.off
            LimitsWithRange, //!< lower.on  and upper.on, lower.xi <= upper.xi
            LimitsExclusive  //!< lower.on and upper.on,  lower.xi >  upper.xi
        };

        Y_TRIPLE_DECL(STANDARD,Limits,
                      Limit,      lower,
                      Limit,      upper,
                      LimitsState,state);
        static const char * LimitsText(const LimitsState)   throw();

        inline      Limits() throw() : lower(), upper(), state(LimitsUnbounded) {}
        inline void reset() throw() { lower.reset(); upper.reset(); state=LimitsUnbounded; }
        inline bool update() throw()
        {
            if(lower.on)
            {
                if(upper.on)
                {
                    if(lower.xi<=upper.xi)
                    {
                        state = LimitsWithRange;
                    }
                    else
                    {
                        state = LimitsExclusive;
                    }
                }
                else
                {
                    assert(!upper.on);
                    state = LimitsLowerOnly;
                }
            }
            else
            {
                assert(!lower.on);
                if(upper.on)
                {
                    state = LimitsUpperOnly;
                }
                else
                {
                    assert(!upper.on);
                    state = LimitsUnbounded;
                }
            }
            return state != LimitsExclusive;
        }

        template <typename OSTREAM> inline
        OSTREAM & show(OSTREAM &os) const
        {
            switch(state)
            {
                case LimitsUnbounded:
                    assert(!lower.on);
                    assert(!upper.on);
                    os << "Unbounded";
                    break;

                case LimitsUpperOnly:
                    assert(!lower.on);
                    assert(upper.on);
                    os << "UpperOnly <= " << upper.xi;
                    break;

                case LimitsLowerOnly:
                    assert(lower.on);
                    assert(!upper.on);
                    os << "LowerOnly >= " << lower.xi;
                    break;

                case LimitsWithRange:
                    assert(lower.on);
                    assert(upper.on);
                    assert(lower.xi<=upper.xi);
                    os << "WithRange [" << lower.xi << "->" << upper.xi << "]";
                    break;

                case LimitsExclusive:
                    assert(lower.on);
                    assert(upper.on);
                    assert(lower.xi>upper.xi);
                    os << "Exclusive " << lower.xi << " > " << upper.xi;
                    break;
            }
            return os;
        }

        Y_TRIPLE_END();

        typedef vector<Limits,Allocator> XiLimits;
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

            static const char xi_[]; //!< "xi_";

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
            void operator()(Addressable      &xi,
                            const Accessible &C) const throw();

            void operator()(XiLimits &limits, const Accessible &C) const throw();

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
                dspNu(os) << xi_;
                eqs.print(os, eqs(eq) );
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

