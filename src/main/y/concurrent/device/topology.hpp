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
            virtual ~topology() throw();
            explicit topology();
            

            void add(const size_t start,
                     const size_t width,
                     const size_t every);

            void add(const string &description);

            void add(const char   *description);

            friend std::ostream & operator<<(std::ostream &, const topology &);


            const nucleus::cluster::list clusters;
            const size_t                 size; //!< total size


        private:
            Y_DISABLE_COPY_AND_ASSIGN(topology);
            void add_(nucleus::cluster *cls) throw();
        };

    }
}

#endif
