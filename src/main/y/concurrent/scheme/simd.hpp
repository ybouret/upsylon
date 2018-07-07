//! \file
#ifndef Y_CONCURRENT_SIMD_INCLUDED
#define Y_CONCURRENT_SIMD_INCLUDED 1

#include "y/concurrent/threads.hpp"
#include "y/concurrent/scheme/for-each.hpp"

namespace upsylon
{
    namespace concurrent
    {

        //! Single Instruction Multiple Data
        class simd :  public for_each, public threads
        {
        public:
            //! construct threads with(out) verbosity flag
            explicit simd(const bool v=false);
            //! destructor
            virtual ~simd() throw();

            //! for_each interface: execute multiple copy of the kernel
            virtual void start( kernel user_code, void *user_data );

            //! for_each interface: use internal barrier and wait
            virtual void finish() throw();

            //! for_each interface: extract parallel context from thread
            inline virtual parallel       &operator[](const size_t i) throw()       { threads       &self = *this; return self[i]; }

            //! for_each interface: extract parallel context from thread
            inline virtual const parallel &operator[](const size_t i) const throw() { const threads &self = *this; return self[i]; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(simd);
            const size_t threshold; //!< threads.size() + 1
            size_t       counter;   //!< for cycle control
            condition    cycle;     //!< to wait...
            kernel       code;
            void        *data;
            virtual void run(parallel &) throw();
        };
    }
}

#endif
