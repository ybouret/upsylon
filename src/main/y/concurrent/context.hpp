//! \file

#ifndef Y_CONCURRENT_CONTEXT_INCLUDED
#define Y_CONCURRENT_CONTEXT_INCLUDED 1

#include "y/parops.hpp"
#include "y/lockable.hpp"

namespace upsylon
{

    namespace concurrent
    {

        //! MPI style context for threads
        class context
        {
        public:
            explicit context() throw(); //! size=1
            explicit context(const size_t sz,
                             const size_t rk) throw();
            virtual ~context() throw();

            //! get the work portion according to rank/size
            template <typename T> inline
            void split(T &user_length, T &user_offset) const throw()
            {
                parops::split_any(user_length,user_offset,size,rank);
            }

            //! get system ticks with lock
            uint64_t     ticks(lockable &) const throw();

            const size_t size;      //!< size>=1
            const size_t rank;      //!< 0<=rank<size
            const size_t indx;      //!< rank+1
            const char   label[16]; //!< formated label

        private:
            Y_DISABLE_ASSIGN(context);
            void fmt() throw();
        };

        typedef void (*executable)(void *, const context &, lockable &);
        
    }

}

#endif

