
//! \file
#ifndef Y_CHEMICAL_STRAIN_INCLUDED
#define Y_CHEMICAL_STRAIN_INCLUDED 1

#include "y/chemical/sys/primary.hpp"

namespace upsylon
{
    namespace Chemical
    {


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
            virtual ~Strain() throw();          //!< cleanup
            explicit Strain(const Species &);   //!< setup with memory

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! link and updated flow state
            void  link(const Primary::Pointer &) throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Primary::Array primary; //!< equilibria involved


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Strain);
        };

    }

}

#endif
