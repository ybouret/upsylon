
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
        //! appliance to transform species
        //
        //______________________________________________________________________
        class Appliance : public Object, public authority<const Primary>
        {
        public:
            virtual ~Appliance() throw();                             //!< cleanup
            const unit_t   nu; //!< nu!=0

        protected:
            explicit Appliance(const unit_t, const Primary&) throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Appliance);
        };

        //______________________________________________________________________
        //
        //
        //! appliance: reactant to equilibria = forward edge
        //
        //______________________________________________________________________
        class Consumer : public Appliance, public dnode<Consumer>
        {
        public:
            explicit Consumer(const unit_t, const Primary &) throw(); //!< setup
            virtual ~Consumer() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Consumer);
        };

        //______________________________________________________________________
        //
        //
        //! appliance: product to equilibria = reverse edge
        //
        //______________________________________________________________________
        class Producer : public Appliance, public dnode<Producer>
        {
        public:
            explicit Producer(const unit_t, const Primary &) throw(); //!< setup
            virtual ~Producer() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Producer);
        };


        typedef core::list_of_cpp<Consumer> Consumers; //!< aliases
        typedef core::list_of_cpp<Producer> Producers; //!< aliases

    }

}

#endif
