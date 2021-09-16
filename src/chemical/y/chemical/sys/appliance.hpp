
//! \file
#ifndef Y_CHEMICAL_APPLIANCE_INCLUDED
#define Y_CHEMICAL_APPLIANCE_INCLUDED 1

#include "y/chemical/sys/primary.hpp"

namespace upsylon
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! appliance to transform strain
        //
        //______________________________________________________________________
        class Appliance : public Object, public authority<const Primary>, public dnode<Appliance>
        {
        public:
            explicit Appliance(const unit_t, const Primary&) throw(); //!< setup
            virtual ~Appliance() throw(); //!< cleanup

            const unit_t   nu; //!< nu!=0

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Appliance);
        };

        //______________________________________________________________________
        //
        //
        //! list of appliances
        //
        //______________________________________________________________________
        typedef core::list_of_cpp<Appliance> Appliances;

    }

}

#endif
