
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
            //! a control is a primary with its topology
            //__________________________________________________________________
            class Control : public Object, public authority<const Primary>
            {
            public:
                //______________________________________________________________
                //
                //  C++
                //______________________________________________________________
                explicit Control(const Primary &, const iAccessible &) throw(); //!< setup
                virtual ~Control() throw();                                     //!< cleanup

                //______________________________________________________________
                //
                //  members
                //______________________________________________________________
                const iAccessible &nu;    //!< bounded topology
                Control           *next;  //!< for list
                Control           *prev;  //!< for list


                //______________________________________________________________
                //
                //!  display
                //______________________________________________________________
                template <typename OSTREAM> inline
                void display(OSTREAM &os, const size_t indent) const
                {
                    const Equilibrium &eq = ***this;
                    Library::Indent(os,indent) << eq << " : " << nu << '\n';
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Control);
            };

            //__________________________________________________________________
            //
            //! alias
            //__________________________________________________________________
            typedef core::list_of_cpp<Control> Controls;

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
            const Controls controls; //!< associated equilibria
            const size_t   involved; //!< associated species
            void compile(const Lineage::Array &lineage) const;


            //__________________________________________________________________
            //
            //!  display
            //__________________________________________________________________
            template <typename OSTREAM> inline
            void display(OSTREAM &os, const size_t indent) const
            {
                Library::Indent(os,indent) << "<Cluster controls=" << controls.size << ">\n";
                const size_t sub = indent+2;
                for(const Control *ctrl=controls.head;ctrl;ctrl=ctrl->next)
                {
                    ctrl->display(os,sub);
                }
                Library::Indent(os,indent) << "<Cluster/>\n";
            }

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

            //! display
            template <typename OSTREAM> inline
            void display(OSTREAM &os, const size_t indent) const
            {
                Library::Indent(os,indent) << "<Clusters count=" << size << ">\n";

                const size_t sub = indent+2;
                for(const Cluster *c=head;c;c=c->next)
                {
                    c->display(os,sub);
                }

                Library::Indent(os,indent) << "<Clusters/>\n";
            }
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
        };

    }

}

#endif
