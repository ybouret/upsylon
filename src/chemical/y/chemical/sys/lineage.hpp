
//! \file
#ifndef Y_CHEMICAL_LINEAGE_INCLUDED
#define Y_CHEMICAL_LINEAGE_INCLUDED 1

#include "y/chemical/sys/primary.hpp"


namespace upsylon
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! lineage of a species
        //
        //______________________________________________________________________
        class Lineage : public Object, public authority<const Species>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arc_ptr<const Lineage>    Pointer; //!< alias
            typedef vector<Pointer,Allocator> Array;   //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Lineage(const Species &); //!< setup with memory
            virtual ~Lineage() throw();        //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void link(const Primary::Pointer &p) throw(); //!< append and check bounded

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const bool           bounded; //!< if all primary are bounded
            const Primary::Array primary; //!< list of depending primary

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lineage);
            };

    }

}

#endif

