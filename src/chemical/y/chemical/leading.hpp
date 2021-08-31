
//! \file

#ifndef Y_CHEMICAL_LEADING_INCLUDED
#define Y_CHEMICAL_LEADING_INCLUDED 1

#include "y/chemical/equilibrium.hpp"


namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! leading information
        //
        //______________________________________________________________________
        class Leading : public Object
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //! kind of leading conditions
            enum Kind
            {
                LimitedByNone, //!< free
                LimitedByReac, //!< <= [someting]
                LimitedByProd, //!< >= -[something]
                LimitedByBoth  //!< in [-something;something_else]
            };

            //! helper
            static const char *KindText(const Kind) throw();

            //! status of solve
            enum Status
            {
                Accepted,
                Modified,
                Rejected
            };

            //! human readable text for a status
            static const char *StatusText(const Status) throw();


            typedef arc_ptr<const Leading>             Pointer;  //!< alias
            typedef vector<Leading::Pointer,Allocator> Array;    //!< alias
            typedef vector<const Actor,Allocator>      Limiting; //!< array alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Leading(const Equilibrium::Pointer &); //!< build
            virtual ~Leading() throw();                     //!< cleanup


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const char *kindText() const throw(); //!< display kind

            //! display
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Leading &leading)
            {
                os << "    " << *leading.root << " [" << leading.kindText() << "]\n";
                leading.display(os,leading.reac," <=  ");
                leading.display(os,leading.prod," >= -");
                return os;
            }

            //! get limiting reactant, set xmax
            const Actor &maxFromReac(const Accessible &C) const throw();

            //! get limiting reactant, set xmin
            const Actor &minFromProd(const Accessible &C) const throw();

            //! Check C, using NuT and xi to modify
            /**
             \return Accepted: untouched C | Modified: moved C | Rejected: invalid C
             */
            Status solve(Addressable   &C,
                         const iMatrix &NuT,
                         Addressable   &xi) const throw();

            
            bool moveAll(const double   x,
                         Addressable   &C,
                         const iMatrix &NuT,
                         Addressable   &xi,
                         const Library &lib) const throw();
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Equilibrium::Pointer root; //!< root equilibrium
            const Limiting             reac; //!< limiting reactant(s)
            const Limiting             prod; //!< limiting product(s)
            const Kind                 kind; //!< kind according to structure
            mutable double             xmax; //!< last computed from reactant(s)
            mutable double             xmin; //!< last computed from product(s)



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Leading);

            template <typename OSTREAM> inline
            void display(OSTREAM        &os,
                         const Limiting &limiting,
                         const char     *cmp) const
            {
                assert(cmp);
                const size_t n = limiting.size();
                for(size_t i=1;i<=n;++i)
                {
                    const Actor &a = limiting[i];
                    os << "    | ";
                    if(a.nu>1)
                    {
                        os << vformat("%u*", unsigned(a.nu));
                    }
                    os << '@' << *root << cmp << a.sp << '\n';
                }
            }

            Status limitedByReac(Addressable &C, const iMatrix &NuT, Addressable   &xi) const throw();
            Status limitedByProd(Addressable &C, const iMatrix &NuT, Addressable   &xi) const throw();
            Status limitedByBoth(Addressable &C, const iMatrix &NuT, Addressable   &xi) const throw();
        };

    }

}

#endif

