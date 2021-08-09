
//! \file

#ifndef Y_ALCHEMY_REACTOR_INCLUDED
#define Y_ALCHEMY_REACTOR_INCLUDED 1

#include "y/alchemy/library.hpp"
#include "y/alchemy/equilibria.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        
        typedef vector<double,Allocator>       Vector;       //!< alias
        typedef vector<size_t,Allocator>       uVector;      //!< alias
        typedef vector<bool,Allocator>         Flags;        //!<  alias
        class                                  Reactor;      //!< forward

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
                      const Reactor &) throw();   //!< setup
            ~Condition() throw();                 //!< cleanup
            Condition(const Condition &) throw(); //!< copy

            const Type     id; //!< type of condition
            const size_t   eq; //!< equilibrium index
            const size_t   sp; //!< species     index
            const size_t   nu; //!< positive coefficient
            const Reactor &cs; //!< reactor


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! regulate extent according to concentration
            void operator()(Addressable &xi, const Accessible &C) const throw();


            //! default output
            template <typename OSTREAM> inline friend
            OSTREAM &operator<<(OSTREAM &os, const Condition c)
            {
                os << '\n' << '{';
                c.dspEq(os);
                switch(c.id)
                {
                    case LEQ: os << "<=  "; break;
                    case GEQ: os << ">= -"; break;
                }
                os << '[' << c.spID() << ']';
                for(size_t i=c.spNS();i>0;--i) os << ' ';

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
                    case LEQ: os << vformat("<= %.15g", C[sp]); break;
                    case GEQ: os << vformat(">= %.15g",-C[sp]); break;
                }
                return os;
            }


        private:
            Y_DISABLE_ASSIGN(Type);
            const string & eqID() const throw();
            const string & spID() const throw();
            size_t         eqNS() const throw();
            size_t         spNS() const throw();

            template <typename OSTREAM> inline
            void padEq(OSTREAM &os) const { for(size_t i=eqNS();i>0;--i) os << ' '; }

            template <typename OSTREAM> inline
            void dspNu(OSTREAM &os) const { if(nu<1) os << vformat("%lu ", (unsigned long)nu ); }

            template <typename OSTREAM> inline
            void dspEq(OSTREAM &os) const { dspNu(os); os << "xi_" << eqID(); padEq(os); }
            
        };

        typedef vector<Condition,Allocator> Conditions; //!< alias

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
            void displayState()               const; //!< info to debug
            bool isValid(const Accessible &C) const; //!< check active C are >0, display error
            bool balance(Addressable &C)    throw(); //!< balance current concentration

            //! output condition for one species
            template <typename OSTREAM> inline
            OSTREAM & showCondition(OSTREAM &os, const Species &sp, const Accessible &C) const
            {
                const size_t j = sp.indx;
                lib.print(os,sp);
                if(sp.active)
                {
                    os << " : ";
                    bool first = true;
                    for(size_t i=1;i<=N;++i)
                    {
                        const long nu = NuT[j][i];
                        if(nu==0) continue;

                        if(nu<0)
                        {
                            if(nu < -1) os << nu; else os << '-';
                        }
                        else
                        {
                            assert(nu>0);
                            if(!first) os << '+';
                            if(nu>1)
                            {
                                os << nu;
                            }
                        }
                        first = false;
                        os << "xi_" << eqs(i).name;
                    }
                    os << " >= " << -C[j];
                }
                else
                {
                    os << " = " << C[j];
                }
                return os << '\n';
            }

            //! use Cbad as workspace
            /**
             compute Cbad, xi,
             restrict xi, compute dC,
             return Psi
             */
            double  Psi(Addressable &C) throw();

            //! Psi at Ctry, using dC
            double  PsiTry(const Accessible &C, const double u) throw();

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
            const Vector      xi0;    //!< [N]   extents
            const Vector      xi1;    //!< [N]   extents
            const Vector      dC;     //!< [M]   delta C
            const Vector      Cbad;   //!< [M]   C minus
            const Vector      Ctry;   //!< [M]   trial C
            const iMatrix     Nu;     //!< [NxM] topology matrix
            const iMatrix     NuT;    //!< [MxN] transposed Nu
            const Vector      NuS;    //!< [M]   scaling for Psi
            const Conditions  Cond;   //!< ...
            const iMatrix     aNu2;   //!< [NxN] adjoint Nu*Nu'
            const long        dNu2;   //!<       determinant if Nu*Nu'
            const Matrix      Phi;    //!< [NxM] jacobian
            const Matrix      J;      //!< [NxN] PhiNuT
            const Matrix      W;      //!< [NxN] LU::build(J)
            const double      C2min;  //!< 2*minimum

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            Vector          Csqr;     //!< [0..M]   C square
            const   Freezer lfrz;
            const   Freezer efrz;



            //! restrain current xi from C values
            void    RestrainXi(const Accessible &C) throw();

            

        };

    }

}

#endif
