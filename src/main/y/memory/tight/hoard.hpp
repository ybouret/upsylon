//! \file

#ifndef Y_MEMORY_TIGHT_HOARD_INCLUDED
#define Y_MEMORY_TIGHT_HOARD_INCLUDED 1

#include "y/lockable.hpp"


namespace upsylon {

    namespace memory {

        namespace tight {

            //__________________________________________________________________
            //
            //
            //! proxy to safely acquire/release items with same block_size
            //
            //__________________________________________________________________
            class hoard
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~hoard() throw();         //!< cleanup

                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                void    *acquire();               //!< lock/on_acquire/unlock
                void     release(void *) throw(); //!< lock/on_release/unlock

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                lockable     &access;     //!< shared lock
                const size_t  block_size; //!< common block size

            protected:
                //! setup with persitent lockable and block size
                explicit hoard(lockable &l, const size_t bs) throw();

            private:
                virtual void *on_acquire()               =0;
                virtual void  on_release(void *) throw() =0;
                Y_DISABLE_COPY_AND_ASSIGN(hoard);
            };

            //__________________________________________________________________
            //
            //
            //! generic MT proxy based on a host type
            //
            //__________________________________________________________________
            template <typename HOST>
            class mt  : public hoard
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup with persistent lockable and HOST
                inline explicit mt(lockable &l, HOST &usr) throw() :
                hoard(l,usr.block_size),
                host(usr)
                {}

                //! cleanup
                inline virtual ~mt() throw() {}

            private:
                HOST &host;
                Y_DISABLE_COPY_AND_ASSIGN(mt);
                inline virtual void *on_acquire()                   { return host.acquire(); }
                inline virtual void  on_release(void *addr) throw() { host.release(addr);    }
            };
            
        }
    }
}

#endif


