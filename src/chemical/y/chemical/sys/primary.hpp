
//! \file
#ifndef Y_CHEMICAL_PRIMARY_INCLUDED
#define Y_CHEMICAL_PRIMARY_INCLUDED 1

#include "y/chemical/equilibria.hpp"
#include "y/type/authority.hpp"

namespace upsylon
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! primary constraint definition
        //
        //______________________________________________________________________
        class Primary : public Object, public authority<const Equilibrium>
        {
        public:
            //__________________________________________________________________
            //
            // Types and definitions
            //__________________________________________________________________
            typedef arc_ptr<const Primary>        Pointer;   //!< alias
            typedef vector<Pointer,Allocator>     Array;     //!< alias
            typedef vector<const Actor,Allocator> Limiting_; //!< base class

            //! kind of primary equilibria
            enum Kind
            {
                LimitedByNone, //!< no primary species in equilibria
                LimitedByReac, //!< limited by reactant(s)
                LimitedByProd, //!< limited by product(s)
                LimitedByBoth  //!< limitted by reactant(s) and product(s)
            };


            //__________________________________________________________________
            //
            //! interface to limiting actors
            //__________________________________________________________________
            class Limiting : public Limiting_
            {
            public:
                virtual             ~Limiting()     throw();                   //!< cleanup
                virtual const char  *symbol() const throw() = 0;               //!< textual comparison
                virtual double       rh_val(const double c) const throw() = 0; //!< right hand value

                //! compute limiting actor and extent (in x) from C
                virtual const Actor & operator()(double &x, const Accessible &C) const throw() = 0;

            protected:
                explicit Limiting(const size_t); //!< setup with memory

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Limiting);
            };

            //__________________________________________________________________
            //
            //! limiting reactant(s)
            //__________________________________________________________________
            class LimitingReac : public Limiting
            {
            public:
                virtual ~LimitingReac() throw();        //!< cleanup
                explicit LimitingReac(const size_t n);  //!< setup


                virtual const char  * symbol()               const throw(); //!< " <=  ";
                virtual double        rh_val(const double c) const throw(); //!< c

                //! min extent(s)
                virtual const Actor & operator()(double &x, const Accessible &C) const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LimitingReac);
            };


            //__________________________________________________________________
            //
            //! limiting product(s)
            //__________________________________________________________________
            class LimitingProd : public Limiting
            {
            public:
                virtual ~LimitingProd() throw();       //!< cleanup
                explicit LimitingProd(const size_t n); //!< setup

                virtual const char *  symbol()               const throw(); //!< " >= -";
                virtual double        rh_val(const double c) const throw(); //!< -c
                virtual const Actor & operator()(double &x, const Accessible &C) const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LimitingProd);
            };


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Primary(const Equilibrium &, const iMatrix &); //!< setup
            virtual ~Primary() throw();                             //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            size_t      count()    const throw(); //!< reac.size() + prod.size()
            const char *kindText() const throw(); //!< to add after "limited by "

            //! move invalid primary concentrations
            bool        solve(Addressable &C, Addressable &xi) const throw();

            //! can equilibrium accept a positive extent ?
            bool        queryForward(const Accessible &C) const throw();

            //! can equilibrium accept a negative extent ?
            bool        queryReverse(const Accessible &C) const throw();

            //! best effort move
            void        move(Addressable &C, const double x, Addressable &xi) const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const iMatrix      &NuT;   //!< topology matrix
            const LimitingReac  reac;  //!< unit rating reactant(s)
            const LimitingProd  prod;  //!< unit rating product(s)
            const Kind          kind;  //!< from reac/prod
            
            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! formal display
            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os, const size_t indent) const
            {
                prolog(os,indent);
                display(os,reac,indent);
                display(os,prod,indent);
                return os;
            }

            //! numerical display
            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os, const Accessible &C, const size_t indent) const
            {
                prolog(os,indent);
                display(os,C,reac,indent);
                display(os,C,prod,indent);
                return os;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Primary);
            
            void update(Addressable &C, const double x, const Actor &a, Addressable &xi) const throw();
            bool solveLimitedByReac(Addressable &C, Addressable &xi) const throw();
            bool solveLimitedByProd(Addressable &C, Addressable &xi) const throw();
            bool solveLimitedByBoth(Addressable &C, Addressable &xi) const throw();


            template <typename OSTREAM> inline
            void prolog(OSTREAM &os, const size_t indent) const
            {
                Library::Indent(os,indent) << "|_" << **this << " is limited by " << kindText();
                if(LimitedByNone!=kind)
                {
                    os << " {";
                    for(size_t i=1;i<=reac.size();++i)
                    {
                        os << ' ' << reac[i].sp.name;
                    }
                    for(size_t i=1;i<=prod.size();++i)
                    {
                        os << ' ' << prod[i].sp.name;
                    }
                    os << " }";
                }
                os << '\n';
            }


            template <typename OSTREAM> inline
            void display(OSTREAM &os, const Limiting &l, const size_t indent) const
            {
                for(size_t i=1;i<=l.size();++i)
                {
                    const Actor &a = l[i];
                    Library::Indent(os,indent) << " |_" << a.nuString() << **this << l.symbol() << a.sp << '\n';
                }
            }

            template <typename OSTREAM> inline
            void display(OSTREAM &os, const Accessible &C, const Limiting &l, const size_t indent) const
            {
                for(size_t i=1;i<=l.size();++i)
                {
                    const Actor &a = l[i];
                    Library::Indent(os,indent) << " |_" << a.nuString() << **this << l.symbol() << a.sp << " = " << l.rh_val(C[a.sp.indx]) << '\n';
                }
            }

        };

    }

}




#endif

