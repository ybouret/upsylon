//! \file
#ifndef Y_CONCURRENT_LAYOUT_INCLUDED
#define Y_CONCURRENT_LAYOUT_INCLUDED 1

#include "y/object.hpp"

namespace upsylon
{
    namespace concurrent
    {

        //! info to assign threads if possible
        class layout : public object
        {
        public:
            const size_t procs; //!< total number of cores
            const size_t cores; //!< user's choice
            const size_t shift; //!< shifted by
            const size_t width; //!< the cores' width

            //! compute from those parameters
            explicit layout(const size_t num_procs,
                            const size_t num_cores,
                            const size_t starting);

            //! desctructor
            virtual ~layout() throw();

            //! compute where to put the thread w.r.t its index
            size_t core_index_of(const size_t thread_index) const throw();

            static layout *create(); //!< layout(nprocs,nprocs,0) or Y_THREADING=cores[:shift]


        private:
            Y_DISABLE_ASSIGN(layout);
            layout(const layout &) throw();
        };

    }
}

#endif
