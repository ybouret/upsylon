//! \file

#ifndef Y_ALCHEMY_REACTOR_INCLUDED
#define Y_ALCHEMY_REACTOR_INCLUDED 1

#include "y/alchemy/equilibria.hpp"
#include "y/alchemy/library.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        
        typedef vector<double,Allocator>       Vector;       //!< alias
        typedef vector<size_t,Allocator>       uVector;      //!< alias
        typedef vector<bool,Allocator>         Flags;        //!<  alias

        class Primary
        {
        public:
            static const char Prefix[];
            typedef vector<Primary,Allocator> Array;

            virtual ~Primary() throw()
            {
            }

            explicit Primary(const Primary &_) throw():
            eq(_.eq),
            sp(_.sp),
            nu(_.nu)
            {
            }

            explicit Primary(const size_t eq_, const size_t sp_, const size_t nu_) throw() :
            eq(eq_),
            sp(sp_),
            nu(nu_)
            {
            }

            const size_t eq;
            const size_t sp;
            const size_t nu;

            template <typename OSTREAM> inline
            OSTREAM & show(OSTREAM &os,
                           const Library     &lib,
                           const Equilibria  &eqs,
                           const Accessible  &C,
                           const bool         leq) const
            {
                if(nu>1)
                    os << vformat("%2u*",unsigned(nu));
                eqs.print(os<<Prefix,eqs(eq));
                const Species &s = *(lib->fetch(sp-1));
                if(leq)
                {
                    os << " <=  ";
                    lib.print(os,s) << vformat(" = %.15g", C[sp]);
                }
                else
                {
                    os << " >= -";
                    lib.print(os,s) << vformat(" = %.15g", -C[sp]);
                }
                return os;
            }

        private:
            Y_DISABLE_ASSIGN(Primary);
        };

        class Sentry : public object, public counted
        {
        public:
            typedef arc_ptr<Sentry>            Pointer;
            typedef vector<Pointer,Allocator>  Array_;

            class Array : public Array_
            {
            public:
                explicit Array(size_t n) : Array_(n,as_capacity)
                {
                    while(n-- >0 )
                    {
                        const Pointer p = new Sentry();
                        push_back_(p);
                    }
                }
                virtual ~Array() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Array);
            };

            explicit Sentry() throw() : leq(), geq() {}
            virtual ~Sentry() throw()
            {
            }

            Primary::Array leq;
            Primary::Array geq;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Sentry);
        };



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
            bool   balance(Addressable &C)    throw(); //!< balance current concentration

            //! initialize
            double Psi(const Accessible &C) throw();

            //! look-up
            double Psi(const Accessible &C, const double u) throw();

            //__________________________________________________________________
            //
            // methods/display
            //__________________________________________________________________


            //! show all conditions
            template <typename OSTREAM> inline
            OSTREAM & showConditions(OSTREAM &os, const Accessible &C) const
            {
                os << "<Conditions>\n";
                os << "  <General>\n";
                for(const Species::Node *node=lib->head();node;node=node->next)
                {
                    const Species &sp = ***node;
                    showCondition(os << "    ",sp,C);
                }
                os << "  <General/>\n";
                showPrimary(os,C);
                os << "<Conditions/>\n";
                return os;
            }



            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Library        &lib;    //!< support library
            const Equilibria     &eqs;    //!< support equlibria
            const size_t          N;      //!< number of equilibria
            const size_t          M;      //!< number of components
            const Flags           active; //!< [M]   active flags
            const size_t          NA;     //!< number of active species
            const Vector          K;      //!< [N]   constants
            const Vector          Gam;    //!< [N]   indicators
            const Primary::Array  leq;    //!<
            const Primary::Array  geq;    //!<
            const Sentry::Array   sentries;
            const Vector          Cpsi;   //!< [M]   to buildPsi
            const Vector          Xpsi;   //!< [N]   search extent = nu*Cpsi
            const Vector          Xtry;   //!< [N]   trial extents
            const Vector          Ctry;   //!< [M]   trial concentrations
            const iMatrix         Nu;     //!< [NxM] topology matrix
            const iMatrix         NuT;    //!< [MxN] transposed Nu
            const Vector          NuS;    //!< [M]   scaling for Psi
            const iMatrix         aNu2;   //!< [NxN] adjoint Nu*Nu'
            const long            dNu2;   //!<       determinant if Nu*Nu'
            const Matrix          Phi;    //!< [NxM] jacobian
            const Matrix          J;      //!< [NxN] PhiNuT
            const Matrix          W;      //!< [NxN] LU::build(J)



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            Vector          Csqr;     //!< [0..M]   C square
            const   Freezer lfrz;
            const   Freezer efrz;


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
                        os << Primary::Prefix << '<' << eqs(i).name << '>';

                    }
                    os << " >= " << -C[j];
                }
                else
                {
                    os << " = " << C[j];
                }
                return os << '\n';
            }

            template <typename OSTREAM> inline
            OSTREAM & showPrimary(OSTREAM &os, const Accessible &C) const
            {
                os << "  <Primary>\n";
                for(size_t i=1;i<=N;++i)
                {
                    const Equilibrium &eq     = eqs(i);
                    const Sentry      &sentry = *sentries[i];
                    os << "    <" << eq.name << ">\n";
                    for(size_t j=sentry.leq.size();j>0;--j)
                    {
                        const Primary &p = sentry.leq[j];
                        assert(p.eq==i);
                        assert(p.nu>0);
                        p.show(os << "      ",lib,eqs,C,true) << '\n';
                    }
                    for(size_t j=sentry.geq.size();j>0;--j)
                    {
                        const Primary &p = sentry.geq[j];
                        assert(p.eq==i);
                        assert(p.nu>0);
                        p.show(os << "      ",lib,eqs,C,false) << '\n';
                    }
                    os << "    <" << eq.name << "/>\n";

                }
                os << " <Primary>\n";
                return os;
            }

            template <typename OSTREAM> inline
            OSTREAM & showPrimary2(OSTREAM &os, const Accessible &C) const
            {
                os << "  <Primary>\n";
                os << "    <LEQ>\n";
                for(size_t i=leq.size();i>0;--i)
                {
                    const Primary     &p  = leq[i];
                    const Equilibrium &eq = eqs(p.eq);
                    assert(p.nu>0);
                    os << "      ";
                    if(p.nu>1) os << vformat("%u*", unsigned(p.nu));
                    eqs.print(os << Primary::Prefix,eq) << " <= " << C[p.sp] << '\n';
                }
                os << "    <LEQ/>\n";
                os << "    <GEQ>\n";
                for(size_t i=geq.size();i>0;--i)
                {
                    const Primary     &p  = geq[i];
                    const Equilibrium &eq = eqs(p.eq);
                    assert(p.nu>0);
                    os << "      ";
                    if(p.nu>1) os << vformat("%u*", unsigned(p.nu));
                    eqs.print(os << Primary::Prefix,eq) << " >= " << -C[p.sp] << '\n';
                }
                os << "    <GEQ/>\n";
                os << " <Primary>\n";
                return os;
            }

            void applyGEQ(Addressable &C) const throw();

        };

    }

}

#endif
