//! \file

#ifndef Y_CONCURRENT_TOPOLOGY_INCLUDED
#define Y_CONCURRENT_TOPOLOGY_INCLUDED 1


#include "y/concurrent/nucleus/cluster.hpp"

namespace upsylon
{
    namespace concurrent
    {

#define Y_NUM_THREADS "Y_NUM_THREADS"

        class topology : public object
        {
        public:
            class node : public object
            {
            public:
                node   *next;
                node   *prev;
                const size_t rank;
                explicit node(const size_t) throw();
                virtual ~node() throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(node);
            };
            
            virtual ~topology() throw();
            explicit topology();
            explicit topology(const size_t start,
                              const size_t width,
                              const size_t every);

            template <typename ID>
            inline explicit topology(const ID &id) : nodes(), clusters()
            {
                add(id);
            }
            
            void add(const size_t start,
                     const size_t width,
                     const size_t every);

            void add(const string &description);
            void add(const char   *description);
            
            size_t size() const throw();
            
            friend std::ostream & operator<<(std::ostream &, const topology &);


            const core::list_of_cpp<node>  nodes;    //!< all nodes
            const nucleus::cluster::list   clusters; //!< all clusters

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(topology);
            void add_(nucleus::cluster *cls) throw();
        };

    }
}

#endif
