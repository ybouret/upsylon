//! \file
#ifndef Y_CONCURRENT_LAYOUT_INCLUDED
#define Y_CONCURRENT_LAYOUT_INCLUDED 1

#include "y/object.hpp"

namespace upsylon
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! info to assign threads if possible
        //
        //______________________________________________________________________
        class layout : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! compute from those parameters, allowing multiple pinning
            /**
             \param num_procs total number of processors
             \param num_cores desired number of cores
             \param main_core where to assign first thread
             procs=max_of(1,num_procs)
             cores=clamp(1,num_cores,proc)
             shift=min_of(0,starting,procs-1)
             width=min_of(cores,procs-shift)
             */
            explicit layout(const size_t num_procs,
                            const size_t num_cores,
                            const size_t starting) throw();


            virtual ~layout()      throw();//!<destructor
            layout(const layout &) throw();//!< no throw copy

            const size_t procs; //!< total number of cores
            const size_t cores; //!< user's choice
            const size_t shift; //!< shifted by
            const size_t width; //!< the cores' width




            //! compute where to put the thread w.r.t its index
            size_t core_index_of(const size_t thread_index) const throw();

            static layout *create(); //!< layout(nprocs,nprocs,0) or Y_THREADING=cores[:shift]

            //! layout(nprocs,replica,primary)
            static layout *create(const size_t replica,
                                  const size_t primary);

        private:
            Y_DISABLE_ASSIGN(layout);
        };

    }
}

#endif
