
//! \file

#ifndef Y_ALCHEMY_PRIMARY_INCLUDED
#define Y_ALCHEMY_PRIMARY_INCLUDED 1

#include "y/alchemy/equilibria.hpp"
#include "y/alchemy/library.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        typedef vector<double,Allocator>       Vector;       //!< alias

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
                const Species &s = lib(sp);
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
            enum Type
            {
                HasNoBound,
                HasOnlyLEQ,
                HasOnlyGEQ,
                IsBothWays
            };



            enum State
            {
                WasValid,
                WasMoved,
                IsJammed
            };

            static const char *StateText(const State s) throw()
            {
                switch(s)
                {
                    case WasValid: return "WasValid";
                    case WasMoved: return "WasMoved";
                    case IsJammed: return "IsJammed";
                }
                return "???";
            }


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

            explicit Sentry() throw() : leq(), geq(), type(HasNoBound) {}
            virtual ~Sentry() throw()
            {
            }

            const char *typeText() const throw()
            {
                switch(type)
                {
                    case HasNoBound: return "HasNoBound";
                    case HasOnlyLEQ: return "HasOnlyLEQ";
                    case HasOnlyGEQ: return "HasOnlyGEQ";
                    case IsBothWays: return "IsBothWays";
                }
                return "???";
            }

            void addLEQ(const Primary &p)
            {
                aliasing::_(leq).push_back(p);
                update();
            }

            void addGEQ(const Primary &p)
            {
                aliasing::_(geq).push_back(p);
                update();
            }


            //! getMax = min of leq, leq.size()>0
            const Primary &getMax(const Accessible &C) const throw()
            {
                assert(leq.size()>0);
                assert(HasOnlyLEQ==type||IsBothWays==type);
                const Primary *opt = &leq[1];
                for(size_t i=leq.size();i>1;--i)
                {
                    const Primary *tmp = &leq[i];
                    const double xi_opt = C[opt->sp]/opt->nu;
                    const double xi_tmp = C[tmp->sp]/tmp->nu;
                    if(xi_tmp<xi_opt)
                    {
                        opt = tmp;
                    }
                }
                return *opt;

            }

            //! getMin = max of geq, geq.size()>0
            const Primary &getMin(const Accessible &C) const throw()
            {
                assert(geq.size()>0);
                assert(HasOnlyGEQ==type||IsBothWays==type);
                const Primary *opt = &geq[1];
                for(size_t i=geq.size();i>1;--i)
                {
                    const Primary *tmp    = &geq[i];
                    const double   xi_opt = -C[opt->sp]/opt->nu;
                    const double   xi_tmp = -C[tmp->sp]/tmp->nu;
                    if(xi_tmp>xi_opt)
                    {
                        opt = tmp;
                    }
                }
                return *opt;
            }


            State solveHasOnlyGEQ(Addressable &C, const iMatrix &NuT, Vector &xi ) const throw();
            State solveHasOnlyLEQ(Addressable &C, const iMatrix &NuT, Vector &xi ) const throw();
            State solveIsBothWays(Addressable &C, const iMatrix &NuT, Vector &xi ) const throw();



            const Primary::Array leq;
            const Primary::Array geq;
            const Type           type;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Sentry);
            void update() throw()
            {
                if(leq.size()>0)
                {
                    if(geq.size()>0)
                    {
                        aliasing::_(type) = IsBothWays;
                    }
                    else
                    {
                        aliasing::_(type) = HasOnlyLEQ;
                    }
                }
                else
                {
                    if(geq.size()>0)
                    {
                        aliasing::_(type) = HasOnlyGEQ;
                    }
                    else
                    {
                        aliasing::_(type) = HasNoBound;
                    }
                }
            }


        };

    }

}

#endif
