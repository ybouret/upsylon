//! \file

#ifndef Y_MEMORY_GROOVE_INCLUDED
#define Y_MEMORY_GROOVE_INCLUDED 1

#include "y/memory/storage.hpp"
#include "y/type/args.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/self-destruct.hpp"
#include <typeinfo>
#include <iosfwd>

namespace upsylon {

    namespace memory {

        //______________________________________________________________________
        //
        //
        //! flexible cache for one or more object of same type
        //
        //______________________________________________________________________
        class groove
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            typedef void (*erase)(void*); //!< per object cleanup

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            groove()  throw(); //!< setup
            ~groove() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void                   free()    throw();       //!< return to flat memory
            void                   release() throw();       //!< free and release memory
            const std::type_info & type_id() const throw(); //!< from user or null_type

            void                   acquire(const size_t n, const storage::model which);
            friend std::ostream & operator<<(std::ostream &, const groove &);

            template <typename T> inline
            T &make(const storage::model which)
            {
                return *ops<T>:: make(*this,1,which);
            }

            template <typename T> inline
            void make(const size_t n, const storage::model which)
            {
                (void) ops<T>:: make(*this,n,which);
            }


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            // flat memory
            const storage::model  model; //!< memory storage
            const void           *entry; //!< memory entry
            const size_t          bytes; //!< accessible bytes
            const size_t          owned; //!< effectively owned>=bytes

            // C++ layer
            const size_t          count; //!< C++ number of objects
            const size_t          width; //!< C++ object size
            const erase           clear; //!< C++ destruct  one object
            const std::type_info *label; //!< C++ type_info of objects

        private:
            Y_DISABLE_COPY_AND_ASSIGN(groove);
            template <typename T>
            struct ops
            {
                Y_DECL_ARGS(T,type);

                static inline mutable_type *make(groove              &target,
                                                 const size_t         count,
                                                 const storage::model which)
                {
                    target.acquire(count*sizeof(type),which);
                    mutable_type *addr = (mutable_type *)(target.entry);
                    if(count>0)
                    {
                        build(addr,count);
                        aliasing::_(target.count) = count;
                        aliasing::_(target.width) = sizeof(type);
                        aliasing::_(target.clear) = destruct;
                        target.label              = &typeid(mutable_type);
                    }
                    return addr;
                }

                static inline void build(mutable_type *addr, const size_t count)
                {
                    size_t built=0;
                    try {
                        while(built<count)
                        {
                            new (addr+built) mutable_type();
                            ++built;
                        }
                    }
                    catch(...)
                    {
                        suppress(addr,built);
                        throw;
                    }
                }

                static inline void suppress(mutable_type *addr, size_t built ) throw()
                {
                    while(built>0)
                    {
                        self_destruct( addr[--built] );
                    }
                }

                static inline void destruct(void *addr) throw()
                {
                    assert(addr);
                    self_destruct( *static_cast<mutable_type*>(addr) );
                }
            };

        };

    }
    
}


#endif
