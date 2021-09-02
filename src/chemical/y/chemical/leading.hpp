
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

            static const char  GeqText[]; //!< " >= -"
            static const char  LeqText[]; //!< " <=  "


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
                os << "    " << *leading.root << " [" << leading.kindText() << "] {";
                for(size_t i=1;i<=leading.reac.size();++i)
                {
                    os << ' ' << leading.reac[i].sp.name;
                }
                for(size_t i=1;i<=leading.prod.size();++i)
                {
                    os << ' ' << leading.prod[i].sp.name;
                }
                os << " }\n";
                leading.display(os,leading.reac,LeqText);
                leading.display(os,leading.prod,GeqText);
                return os;
            }

            //! display numerical
            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os, const Accessible &C, const size_t indent) const
            {
                for(size_t i=1;i<=reac.size();++i)
                {
                    const Actor &a = reac[i];
                    Library::Indent(os,indent) << *root << LeqText << a.sp;
                    displayDivBy(os,a.nu);
                    os << " = " << C[a.sp.indx]/a.nu << '\n';
                }
                for(size_t i=1;i<=prod.size();++i)
                {
                    const Actor &a = prod[i];
                    Library::Indent(os,indent) << *root << GeqText << a.sp;
                    displayDivBy(os,a.nu);
                    os << " = " << -C[a.sp.indx]/a.nu << '\n';
                }
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


            bool isJammed(const double x, const Accessible &C) const throw();

            //! return type for moveXXX functions
#define Y_CHEMICAL_LEADING_MOVE_RET bool

            //! parameters for moveXXX functions
#define Y_CHEMICAL_LEADING_MOVE_API \
const double   x,                   \
Addressable   &C,                   \
const iMatrix &NuT,                 \
Addressable   &xi 

            //! try to move all extent or cut
            Y_CHEMICAL_LEADING_MOVE_RET moveAll(Y_CHEMICAL_LEADING_MOVE_API) const throw();

            void   ensurePositive(Addressable &C) const throw(); //!< of leading species
            size_t count() const throw(); //!< reac.size() + prod.size()

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

            template <typename OSTREAM> static inline
            void displayDivBy(OSTREAM &os, const size_t nu)
            {
                if(nu>1)
                {
                    os << vformat("/%u",unsigned(nu));
                }
                else
                {
                    os << "  ";
                }

            }

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

            Y_CHEMICAL_LEADING_MOVE_RET moveLimitedByNone(Y_CHEMICAL_LEADING_MOVE_API) const throw();
            Y_CHEMICAL_LEADING_MOVE_RET moveLimitedByReac(Y_CHEMICAL_LEADING_MOVE_API) const throw();
            Y_CHEMICAL_LEADING_MOVE_RET moveLimitedByProd(Y_CHEMICAL_LEADING_MOVE_API) const throw();
            Y_CHEMICAL_LEADING_MOVE_RET moveLimitedByBoth(Y_CHEMICAL_LEADING_MOVE_API) const throw();

            static void EnsurePositive(Addressable &C, const Limiting &actors) throw();
            void        limitedBy(const Actor &a, const double x, Addressable &C, const iMatrix &NuT, Addressable   &xi) const throw();
            void        updateAll(const double x, Addressable &C, const iMatrix &NuT, Addressable &xi) const throw();
        };

    }

}

#endif

