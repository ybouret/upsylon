//! \file

#ifndef Y_GFX_ENTITY_INCLUDED
#define Y_GFX_ENTITY_INCLUDED 1

#include "y/ptr/counted.hpp"

namespace upsylon
{
    namespace memory
    {
        class embed;
    }

    namespace graphic
    {

        //______________________________________________________________________
        //
        //! base class for dynamic objects of graphic
        //______________________________________________________________________
        class entity : public object, public counted
        {
        public:
            explicit entity() throw(); //!< setup
            virtual ~entity() throw(); //!< cleanup


            //! suppress memory
            static void  suppress(void *&,size_t &) throw();

            //! allocate embeedded memory
            static void *allocate(memory::embed emb[], const size_t num, size_t &);

            //! throw exception
            static void  error(const char *when);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(entity);
        };
    }

}

#endif

