
//! \file
#ifndef Y_CHEMICAL_LINEAGE_INCLUDED
#define Y_CHEMICAL_LINEAGE_INCLUDED 1

#include "y/chemical/sys/appliance.hpp"

namespace upsylon
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! all possible linkages
        //
        //______________________________________________________________________
        enum Linkage
        {
            Single, //!< bounded, no apps (->spectator)
            Intake, //!< bounded, consumed only
            Inside, //!< bounded, consumed and produced
            Output, //!< bounded, produced only
            Source, //!< endless, produded only
            Siphon  //!< endless, consumed only
        };

        //______________________________________________________________________
        //
        //
        //! human readible linkage
        //
        //______________________________________________________________________
        const char * LinkageText(const Linkage) throw();

        //______________________________________________________________________
        //
        //
        //! species withing a system
        //
        //______________________________________________________________________
        class Lineage : public Object, public authority<const Species>, public Flow
        {
        public:
            //__________________________________________________________________
            //
            // Types and definitions
            //__________________________________________________________________
            typedef arc_ptr<const Lineage>    Pointer; //!< alias
            typedef vector<Pointer,Allocator> Array;   //!< alias



            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Lineage() throw();                //!< cleanup
            explicit Lineage(const Species &) throw(); //!< setup with no memory

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________


            void        link(const unit_t, const Primary &); //!< link and UPDATE flow state
            void        finalize()           throw();        //!< set linkage
            const char *linkageText()  const throw();        //!< textual value for linkage


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Linkage    linkage;   //!< once finalized, default is vacant
            const Appliances consumers; //!< strain is consumed by these equilibria
            const Appliances producers; //!< strain is produced by this equilibria

            //__________________________________________________________________
            //
            // display
            //__________________________________________________________________

            //! display
            template <typename OSTREAM>
            inline friend OSTREAM &operator<<(OSTREAM &os, const Lineage &S)
            {
                return S.display(os);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lineage);
            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os) const
            {
                os << **this << " : " << stateText() << " and " << linkageText();
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
