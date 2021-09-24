
//! \file
#ifndef Y_CHEM_SYS_CLUSTER_INCLUDED
#define Y_CHEM_SYS_CLUSTER_INCLUDED 1

#include "y/chemical/sys/lineage.hpp"

namespace upsylon
{
    namespace Chemical
    {


        //______________________________________________________________________
        //
        //
        //! cluster of coupled equilibria/primary
        //
        //______________________________________________________________________
        class Cluster : public Object, public dnode<Cluster>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! a component is a primary with its topology
            //__________________________________________________________________
            class Component : public Object, public authority<const Primary>
            {
            public:
                //______________________________________________________________
                //
                //  C++
                //______________________________________________________________
                explicit Component(const Primary &, const iAccessible &) throw(); //!< setup
                virtual ~Component() throw();                                     //!< cleanup

                //______________________________________________________________
                //
                //  members
                //______________________________________________________________
                const iAccessible &nu;    //!< bounded topology
                Component         *next;  //!< for list
                Component         *prev;  //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Component);
            };

            //__________________________________________________________________
            //
            //! alias
            //__________________________________________________________________
            typedef core::list_of_cpp<Component> Components;

            //__________________________________________________________________
            //
            //  C++
            //__________________________________________________________________
            explicit Cluster() throw();
            virtual ~Cluster() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     grow(const Primary &, const iAccessible &);    //!< new component
            bool     overlaps(const iAccessible &nu) const throw(); //!< test overlap

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Components components; //!< associated components



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
        };


        //______________________________________________________________________
        //
        //
        //! base class for clusters
        //
        //______________________________________________________________________
        typedef core::list_of_cpp<Cluster> Clusters_;


        //______________________________________________________________________
        //
        //
        //! list of independant clusters
        //
        //______________________________________________________________________
        class Clusters: public Clusters_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Clusters() throw();
            virtual ~Clusters() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! start a new cluster
            Cluster *start(const Primary &p, const iAccessible &nu);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
        };

    }

}

#endif
