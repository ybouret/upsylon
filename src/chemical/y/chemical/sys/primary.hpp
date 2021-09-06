
//! \file
#ifndef Y_CHEMICAL_PRIMARY_INCLUDED
#define Y_CHEMICAL_PRIMARY_INCLUDED 1

#include "y/chemical/equilibria.hpp"


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
        class Primary : public Object
        {
        public:
            //__________________________________________________________________
            //
            // Types and definitions
            //__________________________________________________________________
            typedef arc_ptr<const Primary>        Pointer;   //!< alias
            typedef vector<Pointer,Allocator>     Array;     //!< alias
            typedef vector<const Actor,Allocator> Limiting_; //!< base class

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
            explicit Primary(const Equilibrium &); //!< setup
            virtual ~Primary() throw();            //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            size_t   count() const throw(); //!< reac.size() + prod.size()

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Equilibrium  &root;  //!< underlying equilibirum
            const LimitingReac  reac;  //!< unit rating reactant(s)
            const LimitingProd  prod;  //!< unit rating product(s)

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! formal display
            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os, const size_t indent) const
            {
                display(os,reac,indent);
                display(os,prod,indent);
                return os;
            }

            //! numerical display
            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os, const Accessible &C, const size_t indent) const
            {
                display(os,C,reac,indent);
                display(os,C,prod,indent);
                return os;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Primary);

            template <typename OSTREAM> inline
            void display(OSTREAM &os, const Limiting &l, const size_t indent) const
            {
                for(size_t i=1;i<=l.size();++i)
                {
                    const Actor &a = l[i];
                    Library::Indent(os,indent) << "| " << a.nuString() << root << l.symbol() << a.sp << '\n';
                }
            }

            template <typename OSTREAM> inline
            void display(OSTREAM &os, const Accessible &C, const Limiting &l, const size_t indent) const
            {
                for(size_t i=1;i<=l.size();++i)
                {
                    const Actor &a = l[i];
                    Library::Indent(os,indent) << "| " << a.nuString() << root << l.symbol() << a.sp << " = " << l.rh_val(C[a.sp.indx]) << '\n';
                }
            }

        };

    }

}




#endif

