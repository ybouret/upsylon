//! \file

#ifndef Y_CONCURRENT_TOPOLOGY_INCLUDED
#define Y_CONCURRENT_TOPOLOGY_INCLUDED 1


#include "y/concurrent/nucleus/cluster.hpp"
#include "y/ptr/auto.hpp"


//______________________________________________________________________________
//
//
//! environment variable to parse topology
//
//______________________________________________________________________________
#define Y_NUM_THREADS "Y_NUM_THREADS"

//______________________________________________________________________________
//
//
//! manual setup
//
//______________________________________________________________________________
#define Y_CONCURRENT_TOPO_ARGS \
const size_t start,            \
const size_t width,            \
const size_t every

//______________________________________________________________________________
//
//
//! manual setup
//
//______________________________________________________________________________
#define Y_CONCURRENT_TOPO_ARGS_ start,width,every

namespace upsylon
{
    namespace concurrent
    {
        
        //______________________________________________________________________
        //
        //
        //! topology with different clusters
        //
        //______________________________________________________________________
        class topology : public object
        {
        public:
            typedef const auto_ptr<const topology> pointer; //!< alias

            //__________________________________________________________________
            //
            //! compile available ranks
            //__________________________________________________________________
            class node : public object
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit node(const size_t) throw(); //!< initialize
                virtual ~node() throw();             //!< cleanup

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                node        *next; //!< for list
                node        *prev; //!< for list
                const size_t rank; //!< worker's rank

                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(node);
            };

            //__________________________________________________________________
            //
            //! C++
            //__________________________________________________________________
            virtual ~topology() throw();                //!< cleanup
            explicit topology();                        //!< default or use env
            explicit topology(Y_CONCURRENT_TOPO_ARGS);  //!< overrides

            //__________________________________________________________________
            //
            //! parse a cluster
            //__________________________________________________________________
            template <typename ID>
            inline explicit topology(const ID &id) : nodes(), clusters()
            {
                parse(id);
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! add a cluster start:width:every
            void add(const size_t start,
                     const size_t width,
                     const size_t every);


            void   add(const string &description); //!< parse a cluster
            void   add(const char   *description); //!< parse a cluster
            size_t size()         const throw();   //!< nodes.size
            size_t primary_rank() const throw();   //!< nodes.head->rank

            //! display
            friend std::ostream & operator<<(std::ostream &, const topology &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const core::list_of_cpp<node>  nodes;    //!< all nodes (by ranks)
            const nucleus::cluster::list   clusters; //!< all clusters

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(topology);
			void add_(nucleus::cluster *cls);
            void parse(const string &info);
            void parse(const char   *info);
        };

    }
}

#endif
