//! \file
#ifndef Y_INFORMATION_MODULATION_INCLUDED
#define Y_INFORMATION_MODULATION_INCLUDED 1

#include "y/memory/buffer.hpp"
#include "y/ptr/arc.hpp"
#include "y/ptr/counted.hpp"

namespace upsylon {

    namespace information {

        //! basic information modulation interface
        class modulation : public counted_object
        {
        public:
            typedef arc_ptr<modulation> pointer; //!< shared pointer
            
            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------
            virtual            ~modulation()  throw();            //!< cleanup
            virtual void        reset()              throw() = 0; //!< restart state
            virtual uint8_t     fetch(const uint8_t) throw() = 0; //!< byte wise process
            virtual modulation *clone() const                = 0; //!< clone with status
            virtual const char *name()  const        throw() = 0; //!< identifier

            //------------------------------------------------------------------
            //
            // non virtual interface
            //
            //------------------------------------------------------------------

            //! reset and substitute
            void apply(void *target, const void *source, const size_t length) throw();

            //! reset and in place substitute
            void apply(void *target, const size_t length) throw();

            //! reset and in place substitute
            void apply( memory::rw_buffer &buffer ) throw();

        protected:
            explicit modulation() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(modulation);
        };
    }

#define Y_INFORMATION_MODULATION(NAME) \
const char * NAME:: name() const throw() { return CLID; }\
const char   NAME:: CLID[] = #NAME

}

#endif

