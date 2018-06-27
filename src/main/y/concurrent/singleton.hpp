#ifndef Y_SINGLETON_INCLUDED
#define Y_SINGLETON_INCLUDED 1


#include "y/concurrent/mutex.hpp"
#include "y/concurrent/at-exit.hpp"
#include "y/code/round.hpp"

namespace upsylon
{
    //! singleton of T
    /**
     T must define:
     - a static at_exit::longevity life_time
     */
    template <typename T>
    class singleton
    {
    public:
        static concurrent::mutex access;

        //! create of recall instance
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
                    }

                    //----------------------------------------------------------
                    // access protected static memory
                    //----------------------------------------------------------
                    void     *instance_addr = (void *) &location_[0];
                    instance_ = static_cast<volatile T *>( instance_addr );
                    try
                    {
                        new ( instance_addr ) T();
                    }
                    catch(...)
                    {
                        instance_ = NULL;
                        __clear( (void*)location_ );
                        throw;
                    }
                }
            }
            return *(T *)instance_;
        }

        //! assuming exists
        static inline  T *location() throw()
        {
            assert( singleton::exists() );
            return (T *)instance_;
        }

        //! test if allocated
        static inline bool exists() throw()
        {
            return instance_ != NULL;
        }

    protected:
        inline explicit singleton() throw() {}
        inline virtual ~singleton() throw() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(singleton);

        static volatile T *instance_;       //!< not NULL if created

        static void __release() throw()
        {
            if (instance_ != NULL)
            {
                instance_->~T();
                __clear((void*)instance_);
                instance_ = NULL;
            }
        }

        static void __clear(void *addr) throw()
        {
            assert(addr!=NULL);
            uint64_t *q = static_cast<uint64_t *>(addr);
            for(size_t i=Y_U64_FOR_ITEM(T);i>0;--i) q[i] = 0;
        }
    };

    template <typename T> volatile T *      singleton<T>::instance_ = NULL;
    template <typename T> concurrent::mutex singleton<T>::access;
}

#endif

