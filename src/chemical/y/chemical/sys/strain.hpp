
//! \file
#ifndef Y_CHEMICAL_STRAIN_INCLUDED
#define Y_CHEMICAL_STRAIN_INCLUDED 1

#include "y/chemical/sys/primary.hpp"

namespace upsylon
{
    namespace Chemical
    {

        class Appliance : public Object, public authority<const Primary>, public dnode<Appliance>
        {
        public:
            explicit Appliance(const unit_t, const Primary&) throw();
            virtual ~Appliance() throw();

            const unit_t   nu; //!< nu!=0

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Appliance);
        };

        typedef core::list_of_cpp<Appliance> Appliances;

        //______________________________________________________________________
        //
        //
        //! species withing a system
        //
        //______________________________________________________________________
        class Strain : public Object, public authority<const Species>, public Flow
        {
        public:
            //__________________________________________________________________
            //
            // Types and definitions
            //__________________________________________________________________
            typedef arc_ptr<const Strain>     Pointer; //!< alias
            typedef vector<Pointer,Allocator> Array;   //!< alias



            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Strain() throw();                //!< cleanup
            explicit Strain(const Species &) throw(); //!< setup with no memory

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! link and updated flow state
            void  link(const unit_t nu, const Primary &primary);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Appliances consumers; //!< strain is consumed by these equilibria
            const Appliances producers; //!< strain is produced by this equilibria

            //__________________________________________________________________
            //
            // display
            //__________________________________________________________________

            template <typename OSTREAM>
            inline friend OSTREAM &operator<<(OSTREAM &os, const Strain &S)
            {
                return S.display(os);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Strain);
            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os) const
            {
                os << **this << " :";
                if(consumers.size)
                {
                    os << " ->{";
                    dispApps(os,consumers);
                    os << "}";
                }
                if(producers.size)
                {
                    os << " <-{";
                    dispApps(os,producers);
                    os << "}";
                }
                return os;
            }
            template <typename OSTREAM> static inline
            void dispApps(OSTREAM &os, const Appliances &apps)
            {
                for(const Appliance *app=apps.head;app;app=app->next)
                {
                    os << ' ' << vformat("(%d)",int(app->nu));
                    os << ***app;
                }
            }

        };

    }

}

#endif
