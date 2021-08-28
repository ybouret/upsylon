
//! \file

#ifndef Y_CHEMICAL_LIBRARY_INCLUDED
#define Y_CHEMICAL_LIBRARY_INCLUDED 1

#include "y/chemical/species.hpp"
#include "y/chemical/freezable.hpp"
#include "y/type/gateway.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! information about a (unique) species
        //
        //______________________________________________________________________
        class Library : public Freezable, public gateway<const Species::Set>
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char CLID[]; //!< "Chemical::Library"
            
            explicit Library();         //!< setup empty
            virtual ~Library() throw(); //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! declare a new species, allows multiple SAME
            template <typename NAME> inline
            const Species & operator()(const NAME   &name,
                                       const unit_t  charge)
            {
                return use( new Species(name,charge) );
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            virtual const_type &bulk() const throw();
            
            Species::Set sdb;
            const Species & use(Species *);
            void update() throw();
            
        };
    }
}

#endif

