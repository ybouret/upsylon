//! \file

#ifndef Y_MEMORY_GROOVE_INCLUDED
#define Y_MEMORY_GROOVE_INCLUDED 1

#include "y/type/args.hpp"
#include "y/type/self-destruct.hpp"
#include <typeinfo>
#include <iosfwd>

namespace upsylon {

    namespace memory {

        //______________________________________________________________________
        //
        //
        //! handle user's memory, designed for one (small) object
        //
        //______________________________________________________________________
        class groove
        {
        public:
            typedef void (*destructor)(void*);

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            ~groove() throw(); //!< release all
            groove()  throw(); //!< empty groove

            //__________________________________________________________________
            //
            // management methods
            //__________________________________________________________________
            void dismiss() throw(); //!< destroy, zero, preserve bytes
            void release() throw(); //!< destroy, zero, discard  bytes
            void prepare(size_t n); //!< make n bytes

            //__________________________________________________________________
            //
            // context methods
            //__________________________________________________________________
            bool                  cpp() const throw(); //!< if C++
            const std::type_info &tid() const throw(); //!< typeid or null_type
            bool                  is0() const throw(); //!< all zero

            //! bytes >= sizeof(T)
            template <typename T> inline
            bool may_hold() const throw()
            {
                return bytes >= sizeof(T);
            }

            //__________________________________________________________________
            //
            // building methods
            //__________________________________________________________________

            //! make with default constructor
            template <typename T> inline
            T & make()
            {
                prepare(sizeof(T));
                return ops<T>::make(address,destroy,type_id);
            }

            //! make with copy constructor
            template <typename T> inline
            T & make(typename type_traits<T>::parameter_type args)
            {
                prepare(sizeof(T));
                return ops<T>::template make<T>(address,destroy,type_id,args);
            }

             //! build from one args
            template <typename T, typename U> inline
            T & build(typename type_traits<U>::parameter_type argU)
            {
                prepare(sizeof(T));
                return ops<T>::template make<U>(address,destroy,type_id,argU);
            }

            //! build from two args
            template <typename T, typename U, typename V> inline
            T & build(typename type_traits<U>::parameter_type argU,
                      typename type_traits<V>::parameter_type argV)
            {
                prepare(sizeof(T));
                return ops<T>::template make<U,V>(address,destroy,type_id,argU,argV);
            }


            //__________________________________________________________________
            //
            // access methods
            //__________________________________________________________________

            //! direct cast
            template <typename T>
            T & get() throw()
            {
                assert(may_hold<T>());
                return *static_cast<T*>(address);
            }

            //! direct cast, const
            template <typename T>
            const T & get() const throw()
            {
                assert(may_hold<T>());
                return *static_cast<const T*>(address);
            }

            //! cast with type checking
            template <typename T>
            T & as()
            {
                static const std::type_info &against = typeid(T);
                check_type(against);
                return get<T>();
            }

            //! cast with type checking, const
            template <typename T>
            const T & as() const 
            {
                static const std::type_info &against = typeid(T);
                check_type(against);
                return get<T>();
            }

            //! type checking, raise exception on error
            void check_type( const std::type_info & ) const;

            //! output, mostly for debug
            friend std::ostream & operator<<(std::ostream &, const groove&);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t    bytes;                   //!< maximum usable bytes

        private:
            Y_DISABLE_COPY_AND_ASSIGN(groove);

            void                  *address;          //!< address of data
            destructor             destroy;          //!< destructor if C++
            const std::type_info  *type_id;          //!< type_id    if C++


            template <typename T>
            class ops
            {
            public:
                Y_DECL_ARGS(T,type);

                static inline type & make(void                   *addr,
                                          destructor             &dtor,
                                          const std::type_info * &tinf)
                {

                    assert(addr!=NULL); assert(NULL==dtor); assert(NULL==tinf);
                    new (addr) mutable_type();
                    return setup(addr,dtor,tinf);
                }

                template <typename U>
                static inline type & make(void                                   *addr,
                                          destructor                             &dtor,
                                          const std::type_info *                 &tinf,
                                          typename type_traits<U>::parameter_type argU)
                {
                    assert(addr!=NULL); assert(NULL==dtor); assert(NULL==tinf);
                    new (addr) mutable_type(argU);
                    return setup(addr,dtor,tinf);
                }

                template <typename U,typename V>
                static inline type & make(void                                   *addr,
                                          destructor                             &dtor,
                                          const std::type_info *                 &tinf,
                                          typename type_traits<U>::parameter_type argU,
                                          typename type_traits<V>::parameter_type argV)
                {
                    assert(addr!=NULL); assert(NULL==dtor); assert(NULL==tinf);
                    new (addr) mutable_type(argU,argV);
                    return setup(addr,dtor,tinf);
                }

            private:
                static inline type & setup(void                   *addr,
                                           destructor             &dtor,
                                           const std::type_info * &tinf) throw()
                {
                    static const std::type_info * __type_id = & typeid(mutable_type);
                    dtor = __destroy;
                    tinf = __type_id;
                    return *static_cast<type*>(addr);
                }

                static inline const std::type_info * __type_id() throw()
                {
                    return & typeid(mutable_type);
                }

                static inline void                   __destroy(void *addr) throw()
                {
                    assert(addr);
                    self_destruct( *static_cast<mutable_type *>(addr) );
                }

            };

        };


       


    }

}

#endif

