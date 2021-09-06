//! \file
#ifndef Y_CHEMICAL_SYSTEM_INCLUDED
#define Y_CHEMICAL_SYSTEM_INCLUDED 1

#include "y/chemical/sys/primary.hpp"


namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! System
        //
        //______________________________________________________________________
        class System : public Object
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char CLID[];         //!< "Chemical::System"
            static const char PrimaryEnter[]; //!< "<Primary>"
            static const char PrimaryLeave[]; //!< "<Primary/>"


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            explicit System(const Library &, const Equilibria &, const unsigned);

            //! cleanup
            virtual ~System() throw();

            //__________________________________________________________________
            //
            // memebers
            //__________________________________________________________________
            const Library       &lib;     //!< (frozen) library
            const Equilibria    &eqs;     //!< (frozen) equilibria
            const size_t         N;       //!< eqs size
            const size_t         M;       //!< lib size
            const Matrix         Nu;      //!< [NxM] topology
            const Matrix         NuT;     //!< [MxN] Nu'
            const Primary::Array primary; //!< [N]
            const size_t         NP;      //!< primary species [0..N]


            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! display numerical primary constraints
            template <typename OSTREAM>
            void showPrimary(OSTREAM &os, const Accessible &C, const size_t indent) const
            {
                Library::Indent(os,indent) << PrimaryEnter << std::endl;
                const size_t sub = indent+2;
                for(size_t i=1;i<=N;++i)
                {
                    primary[i]->display(os,C,sub);
                }
                Library::Indent(os,indent) << PrimaryLeave << std::endl;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(System);

            const Freezable::Latch libLatch;
            const Freezable::Latch eqsLatch;

        };
    }

}


#endif

