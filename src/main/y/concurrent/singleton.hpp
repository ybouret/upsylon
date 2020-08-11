//! \file
#ifndef Y_SINGLETON_INCLUDED
#define Y_SINGLETON_INCLUDED 1


#include "y/concurrent/mutex.hpp"
#include "y/concurrent/at-exit.hpp"
#include "y/code/round.hpp"
#include <new>

namespace upsylon
{
    namespace core
    {
        class singleton
        {
        public:
            static bool verbose;
            virtual ~singleton() throw();
            
        protected:
            explicit singleton() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(singleton);
        };
    }
    //! singleton of T
    /**
     T must define a static at_exit::longevity life_time
     */
    template <typename T>
    class singleton
    {
    public:
        //! a mutex for thread safe handling
        static concurrent::mutex access;

        //! create or recall instance
        static T & instance()
        {
            static volatile bool     register_ = true;
            static volatile uint64_t location_[ Y_U64_FOR_ITEM(T) ];
            //------------------------------------------------------------------
            // Double Check Pattern
            //------------------------------------------------------------------
            Y_LOCK(access);
            if ( !instance_ )
            {
                Y_LOCK(access);
                if ( !instance_ )
                {
                    //----------------------------------------------------------
                    // once in a lifetime registering
                    //----------------------------------------------------------
                    if( register_ )
                    {
                        at_exit::perform( __release, T::life_time);
                        register_ = false;
                        reserved_ = sizeof(location_);
                    }

                    //----------------------------------------------------------
                    // access protected static memory
                    //----------------------------------------------------------
                    void *addr = (void *) &location_[0];
                    instance_  = static_cast<volatile T *>( addr );
                    try{ new ( addr ) T(); }
                    catch(...)
                    {
                        instance_ = NULL;
                        __clear( addr );
                        throw;
                    }
                }
            }
            return *(T *)instance_;
        }

        //! assuming exists
        static inline  T &location() throw()
        {
            assert( singleton::exists() );
            return *(T *)instance_;
        }

        //! test if allocated
        static inline bool exists() throw()
        {
            return instance_ != NULL;
        }

        //! create the singleton, no matter what...
        static inline void create()
        {
            (void) instance();
        }

        //! bytes for instance storage
        inline size_t reserved() const throw()
        {
            return reserved_;
        }


    protected:
        inline explicit singleton() throw() {}
        inline virtual ~singleton() throw() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(singleton);

        static volatile T *instance_; //!< not NULL if created
        static size_t      reserved_; //!< sizeof(instance)

        //! register in at_exit
        static void __release() throw()
        {
            if (instance_)
            {
                instance_->~T();
                __clear((void*)instance_);
                instance_ = NULL;
            }
        }

        //! just cleaning up
        static void __clear(void *addr) throw()
        {
            assert(addr);
            uint64_t *q = static_cast<uint64_t *>(addr);
            for(size_t i=0;i<Y_U64_FOR_ITEM(T);++i) q[i] = 0;
        }
    };

    template <typename T> volatile T *      singleton<T>::instance_ = 0;
    template <typename T> concurrent::mutex singleton<T>::access;
    template <typename T> size_t            singleton<T>::reserved_ = 0;
}

#endif

