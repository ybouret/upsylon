
//! \file

#ifndef Y_MEMORY_SHACK_INCLUDED
#define Y_MEMORY_SHACK_INCLUDED 1

#include "y/type/args.hpp"
#include "y/memory/buffer.hpp"
#include "y/type/collapse.hpp"
#include "y/type/releasable.hpp"
#include "y/sequence/array.hpp"
#include <typeinfo>

namespace upsylon {

    namespace memory {

        //______________________________________________________________________
        //
        //
        //! flexible cache for one or more objects of SAME type
        //
        //______________________________________________________________________
        class shack : public memory::rw_buffer, public releasable
        {
        public:

            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef void (*erase)(void *); //!< delete ONE object at given address

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            explicit shack() throw(); //!< setup empty
            virtual ~shack() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // buffer interface
            //__________________________________________________________________
            size_t      length() const throw(); //!< block_size
            const void *ro()     const throw(); //!< block_addr

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool         is_anonymous() const throw(); //!< true is flat memory
            bool         is_cplusplus() const throw(); //!< true if a C++ object is stored
            bool         is_(const std::type_info &) const throw(); //!< true if labeled as...
            void         acquire(size_t n);    //!< length() >= n
            virtual void release() throw();    //!< kill() and release all
            void         free() throw();       //!< kill() and zero()
            size_t       tell() const throw(); //!< number of C++ obejct

            //! test types
            template <typename T>
            bool is() const throw()
            {
                return is_( ops<T>::id() );
            }

            //! make ONE object
            template <typename T> inline
            T &make()
            {
                ops<T>::make(*this,1);
                return *(T *)block_addr;
            }

            //! make n objects
            template <typename T> inline
            void make(const size_t n)
            {
                ops<T>::make(*this,n);
            }

            //! cast with check
            template <typename T> inline
            T & _() throw()
            {
                assert( is<T>() );
                return *(T*)block_addr;
            }

            //! cast with check, const
            template <typename T> inline
            const T & _() const throw()
            {
                assert( is<T>() );
                return *(const T *)block_addr;
            }



            //! cast with check to array
            template <typename T> inline
            lightweight_array<T> & __() throw()
            {
                assert(is<T>());
                return *static_cast<lightweight_array<T> *>( vaddr() );
            }

            //! cast with check to array, const
            template <typename T> inline
            const lightweight_array<T> & __() const throw()
            {
                assert(is<T>());
                return *static_cast<const lightweight_array<T> *>( vaddr() );
            }

            //! direct access to multiple items
            template <typename T> inline
            T & _(const size_t j) throw()
            {
                return __<T>()[j];
            }

            //! direct access to multiple items
            template <typename T> inline
            const T & _(const size_t j) const throw()
            {
                return __<T>()[j];
            }

            //! build from data
            template <typename T, typename U>
            T &build(const U &args)
            {
                ops<T>:: template build<U>(*this,args);
                return *(T *)block_addr;
            }
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(shack);
            void kill()         throw(); //!< kill objects
            void zero()         throw(); //!< set memory to zero
            void *vaddr() const throw(); //!< &vdata

            //__________________________________________________________________
            //
            // flat memory
            //__________________________________________________________________
            void            *block_addr;
            size_t           block_size;
            size_t           block_exp2;

            //__________________________________________________________________
            //
            // C++ memory
            //__________________________________________________________________
            size_t                 count; //!< C++ objects
            size_t                 width; //!< C++ object widht
            erase                  clear; //!< delete one object
            const std::type_info * label; //!< type recognition
            lightweight_array<int> vdata; //!< ABI for an array



            template <typename T>
            struct ops
            {
                Y_DECL_ARGS(T,type);
                typedef lightweight_array<mutable_type> vtype;

                static inline
                const std::type_info &id() throw()
                {
                    return  typeid(mutable_type);
                }

                static inline
                void no(void *args) throw()
                {
                    assert(args);
                    collapse( *static_cast<mutable_type *>(args) );
                }

                static inline void hook(shack &self, const size_t n) throw()
                {
                    assert(n>0);
                    assert(n*sizeof(T)<=self.block_size);
                    self.count = n;
                    self.width = sizeof(T);
                    self.clear = no;
                    self.label = &id();
                    new ( self.vaddr() ) vtype( & self._<mutable_type>(), self.count );
                }

                static inline void make(shack &self, const size_t n)
                {
                    if(n>0)
                    {
                        self.acquire(n*sizeof(T));
                        mutable_type *addr = static_cast<mutable_type *>(self.block_addr);
                        size_t        done = 0;
                        try
                        {
                            while(done<n)
                            {
                                new (addr+done) T();
                                ++done;
                            }
                        }
                        catch(...)
                        {
                            while(done>0)
                            {
                                collapse(addr[--done]);
                            }
                            throw;
                        }
                        hook(self,n);
                    }
                    else
                    {
                        self.free();
                    }
                }

                template <typename U>
                static inline void build(shack &self, const U &args)
                {
                    self.acquire(sizeof(T));
                    new (self.block_addr) T(args);
                    hook(self,1);
                }

            };

        };

    }
}

#endif
