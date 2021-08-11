//! \file

#ifndef Y_ALCHEMY_REACTOR_INCLUDED
#define Y_ALCHEMY_REACTOR_INCLUDED 1

#include "y/alchemy/condition.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        
        typedef vector<double,Allocator>       Vector;       //!< alias
        typedef vector<size_t,Allocator>       uVector;      //!< alias
        typedef vector<bool,Allocator>         Flags;        //!<  alias

    

        //______________________________________________________________________
        //
        //
        //! chemical reactor
        //
        //______________________________________________________________________
        class Reactor
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static  const char                     CLID[];       //!< "Reactor"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            explicit Reactor(Library    &,
                             Equilibria &);

            //! cleanup
            virtual ~Reactor() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void   displayState()               const; //!< info to debug
            bool   isValid(const Accessible &C) const; //!< check active C are >0, display error
            bool   balance(Addressable &C)    throw();                           //!< balance current concentration
            void   restrain(Addressable & , const Accessible & ) const throw(); //!< apply all conditions
            bool   topology(const Accessible &) throw();                        //!< check compatible extent

            //! initialize
            double Psi(const Accessible &C) throw();

            //! look-up
            double Psi(const Accessible &C, const double u) throw();

            //__________________________________________________________________
            //
            // methods/display
            //__________________________________________________________________

            //! output condition for one species
            template <typename OSTREAM> inline
            OSTREAM & showCondition(OSTREAM &os, const Species &sp, const Accessible &C) const
            {
                const size_t j = sp.indx;
                lib.print(os,sp);
                if(sp.active)
                {
                    os << " :";
                    bool first = true;
                    for(size_t i=1;i<=N;++i)
                    {
                        const long nu = NuT[j][i];
                        if(nu==0) continue;
                        os << ' ';
                        if(nu<0)
                        {
                            if(nu < -1) os << vformat("%ld",nu); else os << '-';
                        }
                        else
                        {
                            assert(nu>0);
                            if(!first) os << '+';
                            if(nu>1)
                            {
                                os << vformat("%ld",nu);
                            }
                        }
                        first = false;
                        os << Condition::pfx << '<' << eqs(i).name << '>';

                    }
                    os << " >= " << -C[j];
                }
                else
                {
                    os << " = " << C[j];
                }
                return os << '\n';
            }

            //! show all conditions
            template <typename OSTREAM> inline
            OSTREAM & showConditions(OSTREAM &os, const Accessible &C) const
            {
                os << "<Conditions>\n";
                os << " <General>\n";
                for(const Species::Node *node=lib->head();node;node=node->next)
                {
                    const Species &sp = ***node;
                    showCondition(os << "  ",sp,C);
                }
                os << " <General/>\n";
                os << " <Primary>\n";
                for(size_t i=cond.size();i>0;--i)
                {
                    cond[i].show(os << "  ",C) << '\n';
                }
                os << " <Primary/>\n";
                os << "<Conditions/>\n";
                return os;
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Library    &lib;    //!< support library
            const Equilibria &eqs;    //!< support equlibria
            const size_t      N;      //!< number of equilibria
            const size_t      M;      //!< number of components
            const Flags       active; //!< [M]   active flags
            const size_t      NA;     //!< number of active species
            const Vector      K;      //!< [N]   constants
            const Vector      Gam;    //!< [N]   indicators
            const Limits::Array limits; //!< [N]   limits of extent
            const Vector      Cpsi;   //!< [M]   to buildPsi
            const Vector      Xpsi;   //!< [N]   search extent = nu*Cpsi
            const Vector      Xtry;   //!< [N]   trial extents
            const Vector      Ctry;   //!< [M]   trial concentrations
            const iMatrix     Nu;     //!< [NxM] topology matrix
            const iMatrix     NuT;    //!< [MxN] transposed Nu
            const Vector      NuS;    //!< [M]   scaling for Psi
            const Conditions  cond;   //!< [0..N] conditions
            const iMatrix     aNu2;   //!< [NxN] adjoint Nu*Nu'
            const long        dNu2;   //!<       determinant if Nu*Nu'
            const Matrix      Phi;    //!< [NxM] jacobian
            const Matrix      J;      //!< [NxN] PhiNuT
            const Matrix      W;      //!< [NxN] LU::build(J)



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            Vector          Csqr;     //!< [0..M]   C square
            const   Freezer lfrz;
            const   Freezer efrz;
        };

    }

}

#endif
