//! \file
#ifndef Y_CHEMICAL_SYSTEM_INCLUDED
#define Y_CHEMICAL_SYSTEM_INCLUDED 1

#include "y/chemical/sys/primary.hpp"


namespace upsylon
{
    namespace Chemical
    {

        class System : public Object
        {
        public:
            static const char CLID[];
            static const char PrimaryEnter[];
            static const char PrimaryLeave[];


            explicit System(const Library &, const Equilibria &, const unsigned);
            virtual ~System() throw();

            const Library       &lib; //!< (frozen) library
            const Equilibria    &eqs; //!< (frozen) equilibria
            const size_t         N;   //!< eqs size
            const size_t         M;   //!< lib size
            const Matrix         Nu;  //!< [NxM] topology
            const Matrix         NuT;     //!< [MxN] Nu'
            const Primary::Array primary; //!< [N]
            const size_t         NP;      //!< primary species [0..N]

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

