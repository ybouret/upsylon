//! \file
#ifndef Y_CHEMICAL_SYSTEM_INCLUDED
#define Y_CHEMICAL_SYSTEM_INCLUDED 1

#include "y/chemical/sys/lineage.hpp"
#include "y/chemical/sys/replica.hpp"
#include "y/core/rlinked.hpp"

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
            static const char ReplicaEnter[]; //!< "<Replica>"
            static const char ReplicaLeave[]; //!< "<Replica/>"
            static const char Success[];      //!< "Success"
            static const char Failure[];      //!< "Failure"

            const char * Outcome(const bool) throw(); //!< Success/Failure
            
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
            // methods
            //__________________________________________________________________
           
            //! balance primary constraints
            bool balancePrimary(Addressable &C) throw();


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Library       &lib;        //!< (frozen) library
            const Equilibria    &eqs;        //!< (frozen) equilibria
            const size_t         N;          //!< number of equilibria
            const size_t         M;          //!< number of species
            const size_t         MW;         //!< working species [0..M]
            const size_t         Nc;         //!< complementary constraints
            const size_t         MP;         //!< primary species [0..M]
            const size_t         MR;         //!< replica species [0..M]
            const size_t         MS;         //!< spectators      [0..M]
            const size_t         ME;         //!< endless working species
            const size_t         MB;         //!< bounded working species
            const iMatrix        Nu;         //!< [NxM] topology
            const iMatrix        NuT;        //!< [MxN] Nu'
            const Primary::Array primary;    //!< [N]
            const Lineage::Array lineage;    //!< [M]
            const Replica::Array replica;    //!< [MR]
            const iVector        Z;          //!< [M] vector of charges
            const bool           charged;    //!< |Z| != 0
            const iMatrix        Omega;      //!< [...xM]
            Vector               xi;         //!< [N] helper to move
            Flags                ok;         //!< [N] primary status
            Actor::Array         who;        //!< [0..N], at most one per equilibria
            
            
            
            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! display numerical primary constraints
            template <typename OSTREAM> inline
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

            //! display numerical primary constraints
            template <typename OSTREAM> inline 
            void showReplica(OSTREAM &os, const Accessible &C, const size_t indent) const
            {
                Library::Indent(os,indent) << ReplicaEnter << std::endl;
                const size_t sub = indent+2;
                for(size_t i=1;i<=MR;++i)
                {
                    replica[i]->display(os,C,sub);
                }
                Library::Indent(os,indent) << ReplicaLeave << std::endl;
            }

            //! output a graphiz representation, with or without endless
            void graphViz(const string &fileName,const bool boundedOnly=false) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(System);

            const Freezable::Latch libLatch;
            const Freezable::Latch eqsLatch;

            void buildClusters();
            void buildOmega();



        };
    }

}


#endif

