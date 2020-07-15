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
        //! flexible cache for one or more objects of SAME type
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
            // management methods
            //__________________________________________________________________
            void                   free()    throw();                           //!< return to flat memory
            void                   release() throw();                           //!< free and release memory
            const std::type_info & type_id() const throw();                     //!< from user or null_type
            void                   acquire(const storage::model, const size_t); //!< acquire zeroed bytes for a given model
            friend std::ostream & operator<<(std::ostream &, const groove &);   //!< display info
            bool                   is_cplusplus() const throw();                //!< handling object ?
            bool                   is_zeroed()    const throw();                //!< all bytes are zero ?
            const char *           model_text()   const throw();                //!< storage::text(model)
            
            //__________________________________________________________________
            //
            // build methods
            //__________________________________________________________________

            //! make n default objects
            template <typename T> inline
            groove & make(const storage::model which, const size_t n)
            {
                ops<T>:: make(which,*this,n);
                return *this;
            }

            //! make one default object
            template <typename T> inline
            groove &make(const storage::model which)
            {
                ops<T>:: make(which,*this,1);
                return *this;
            }

            //! make one default object, returned
            template <typename T> inline
            T & single(const storage::model which)
            {
                ops<T>:: make(which,*this,1);
                return *(T*)entry;
            }
            

            //! build n objects with same parameter
            template <typename T,typename U> inline
            groove &build(const storage::model which, const size_t n, typename type_traits<U>::parameter_type argU)
            {
                ops<T>:: template make(which,*this,n,argU);
                return *this;
            }

            //! build one object with one parameter
            template <typename T, typename U> inline
            groove & build(const storage::model which,const typename type_traits<U>::parameter_type argU)
            {
                ops<T>:: template make(which,*this,1,argU);
                return *this;
            }

            //! copy
            template <typename T>
            groove & copy(const storage::model which, typename type_traits<T>::parameter_type args)
            {
                ops<T>:: template make(which,*this,1,args);
                return *this;
            }

            //__________________________________________________________________
            //
            // access methods
            //__________________________________________________________________
            //! flat memory access
            template <typename T>
            inline T & get() throw()
            {
                assert(bytes>=sizeof(T));assert(entry!=NULL);
                return *(T*)entry;
            }

            //! flat memory access
            template <typename T>
            inline const T & get() const throw()
            {
                assert(bytes>=sizeof(T));assert(entry!=NULL);
                return *(const T*)entry;
            }

            //! access first entry with type checking
            template <typename T>
            inline T &as()
            {
                check_same_than( typeid(T) );
                return get<T>();
            }

            //! access first entry with type checking
            template <typename T>
            inline const T &as() const
            {
                check_same_than( typeid(T) );
                return get<T>();
            }

            //! [0..count-1] access
            template <typename T>
            inline T & get(size_t indx) throw()
            {
                assert(bytes>=(indx+1)*sizeof(T));assert(entry!=NULL);
                T *p = (T *)entry;
                return p[indx];
            }

            //! [0..count-1] access, const
            template <typename T>
            inline const T & get(size_t indx) const throw()
            {
                assert(bytes>=(indx+1)*sizeof(T));assert(entry!=NULL);
                T *p = (T *)entry;
                return p[indx];
            }

            //! [0..count-1] access with type checking
            template <typename T>
            inline T &as(const size_t indx)
            {
                check_same_than( typeid(T) );
                return get<T>(indx);
            }

            //! [0..count-1] access with type checking
            template <typename T>
            inline const T &as(const size_t indx) const
            {
                check_same_than( typeid(T) );
                return get<T>(indx);
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
            void check_same_than(const std::type_info &) const;

            //__________________________________________________________________
            //
            // internal templated operations
            //
            template <typename T> struct ops
            {
                Y_DECL_ARGS(T,type);

                static inline void make(const storage::model which,
                                        groove              &target,
                                        const size_t         count)
                {
                    target.acquire(which,count*sizeof(type));
                    mutable_type *addr = (mutable_type *)(target.entry);
                    if(count>0)
                    {
                        build(addr,count);
                        setup(target,count);
                    }
                }

                template <typename U>
                static inline void make(const storage::model  which,
                                        groove               &target,
                                        const size_t          count,
                                        const U              &argU)
                {
                    target.acquire(which,count*sizeof(type));
                    mutable_type *addr = (mutable_type *)(target.entry);
                    if(count>0)
                    {
                        build<U>(addr,count,argU);
                        setup(target,count);
                    }
                }


                static inline void setup(groove &target,const size_t count) throw()
                {
                    aliasing::_(target.count) = count;
                    aliasing::_(target.width) = sizeof(type);
                    aliasing::_(target.clear) = destruct;
                    target.label              = &typeid(mutable_type);
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
                        suppr(addr,built);
                        throw;
                    }
                }

                template <typename U>
                static inline void build(mutable_type *addr, const size_t count, const U &argU)
                {
                    size_t built=0;
                    try {
                        while(built<count)
                        {
                            new (addr+built) mutable_type(argU);
                            ++built;
                        }
                    }
                    catch(...)
                    {
                        suppr(addr,built);
                        throw;
                    }
                }

                static inline void suppr(mutable_type *addr, size_t built ) throw()
                {
                    while(built>0) self_destruct( addr[--built] );
                }

                static inline void destruct(void *addr) throw()
                {
                    assert(addr);
                    self_destruct( *static_cast<mutable_type*>(addr) );
                }
            };
            //__________________________________________________________________


        };

    }
    
}


#endif
