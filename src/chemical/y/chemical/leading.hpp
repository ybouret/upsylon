
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
            enum Kind
            {
                LimitedByNone,
                LimitedByReac,
                LimitedByProd,
                LimitedByBoth
            };
            static const char *KindText(const Kind) throw();

            typedef arc_ptr<Leading>               Pointer;  //!< alias
            typedef vector<const Actor,Allocator>  Limiting; //!< array alias

            explicit Leading(const Equilibrium::Pointer &);
            virtual ~Leading() throw();

            const Equilibrium::Pointer root;
            const Limiting             reac;
            const Limiting             prod;
            const Kind                 kind;

            const char *kindText() const throw();

            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Leading &leading)
            {
                os << "  " << *leading.root << ' ' << leading.kindText() << '\n';
                leading.display(os,leading.reac," <=  ");
                leading.display(os,leading.prod," >= -");
                return os;
            }


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
                    os << "    ";
                    if(a.nu>1)
                    {
                        os << vformat("%u*", unsigned(a.nu));
                    }
                    os << '@' << *root << cmp << a.sp << '\n';
                }
            }



        };

    }

}

#endif

