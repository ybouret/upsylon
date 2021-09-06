
//! \file
#ifndef Y_CHEMICAL_PRIMARY_INCLUDED
#define Y_CHEMICAL_PRIMARY_INCLUDED 1

#include "y/chemical/equilibria.hpp"


namespace upsylon
{
    namespace Chemical
    {

        class Primary : public Object
        {
        public:
            typedef arc_ptr<const Primary>        Pointer;
            typedef vector<Pointer,Allocator>     Array;
            typedef vector<const Actor,Allocator> Limiting_;

            class Limiting : public Limiting_
            {
            public:
                virtual             ~Limiting()     throw();
                virtual const char  *symbol() const throw() = 0;
                virtual const Actor & operator()(double &x, const Accessible &C) const throw() = 0;
                virtual double       by(const double c) const throw() = 0;

            protected:
                explicit Limiting(const size_t);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Limiting);
            };

            class LimitingReac : public Limiting
            {
            public:
                virtual ~LimitingReac() throw();
                explicit LimitingReac(const size_t n);


                virtual const char  * symbol()           const throw(); //!< " <=  ";
                virtual double        by(const double c) const throw(); //!< c
                virtual const Actor & operator()(double &x, const Accessible &C) const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LimitingReac);
            };


            class LimitingProd : public Limiting
            {
            public:
                static const char Symbol[];
                virtual ~LimitingProd() throw();
                explicit LimitingProd(const size_t n);

                virtual const char *  symbol() const throw();           //!< " >= -";
                virtual double        by(const double c) const throw(); //!< -c

                virtual const Actor & operator()(double &x, const Accessible &C) const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LimitingProd);
            };



            explicit Primary(const Equilibrium &);
            virtual ~Primary() throw();

            size_t   count() const throw();

            const Equilibrium  &root;
            const LimitingReac  reac;
            const LimitingProd  prod;

            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os, const size_t indent) const
            {
                display(os,reac,indent);
                display(os,prod,indent);
                return os;
            }

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
                    Library::Indent(os,indent) << "| " << a.nuString() << root << l.symbol() << a.sp << " = " << l.by(C[a.sp.indx]) << '\n';
                }
            }

        };

    }

}




#endif

