
//! \file
#ifndef Y_CHEMICAL_STRAIN_INCLUDED
#define Y_CHEMICAL_STRAIN_INCLUDED 1

#include "y/chemical/sys/primary.hpp"

namespace upsylon
{
    namespace Chemical
    {

        //! species withing a system
        class Strain : public Object, public authority<const Species>, public Flow
        {
        public:
            typedef arc_ptr<const Strain>     Pointer;
            typedef vector<Pointer,Allocator> Array;


            virtual ~Strain() throw();
            explicit Strain(const Species &);

            void  link(const Primary::Pointer &) throw();

            const Primary::Array primary; //!< equilibria involved


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Strain);
        };

    }

}

#endif
