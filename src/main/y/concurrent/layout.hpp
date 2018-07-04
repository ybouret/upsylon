#ifndef Y_CONCURRENT_LAYOUT_INCLUDED
#define Y_CONCURRENT_LAYOUT_INCLUDED 1

#include "y/object.hpp"

namespace upsylon
{
    namespace concurrent
    {

        class layout : public object
        {
        public:
            const size_t procs; //!< total number of cores
            const size_t cores; //!< user's choice
            const size_t shift; //!< shifted by
            const size_t width; //!< the cores' width

            explicit layout(const size_t num_procs,
                            const size_t num_cores,
                            const size_t starting);

            virtual ~layout() throw();

            size_t core_index_of(const size_t thread_index) const throw();

            static layout *create(); //!< layout(#cpus,#cpus,0) or Y_THREADING=#cpus[:shift]


        private:
            Y_DISABLE_ASSIGN(layout);
            layout(const layout &) throw();
        };

    }
}

#endif
