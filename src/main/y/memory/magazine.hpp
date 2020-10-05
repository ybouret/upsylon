#ifndef Y_MEMORY_MAGAZINE_INCLUDED
#define Y_MEMORY_MAGAZINE_INCLUDED 1

#include "y/memory/tight/supply.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/ptr/ptr.hpp"

namespace upsylon
{
    
    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! create specialized magazine for very dynamic same objects
        //
        //______________________________________________________________________
        template <typename T>
        class magazine : public singleton< magazine<T> >, public tight::supply_of<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);                              //!< aliases
            typedef singleton< magazine<T> > singleton_type;  //!< alias
            typedef tight::supply_of<T>      supply_type;     //!< alias
            Y_SINGLETON_DECL(magazine);                       //!< prepare singleton

            //__________________________________________________________________
            //
            // single object memory management
            //__________________________________________________________________

            //! no arguments acquire
            inline type *acquire()
            {
                Y_LOCK(this->access);
                mutable_type *ptr = this->zquery();
                try { return new(ptr) type(); } catch(...) { this->zstore(ptr); throw; }
            }

            //! one argument acquire
            template <typename U>
            inline type *acquire(typename type_traits<U>::parameter_type argU)
            {
                Y_LOCK(this->access);
                mutable_type *ptr = this->zquery();
                try { return new(ptr) type(argU); } catch(...) { this->zstore(ptr); throw; }
            }

            //! two arguments acquire
            template <typename U, typename V>
            inline type *acquire(typename type_traits<U>::parameter_type argU,
                                 typename type_traits<V>::parameter_type argV)
            {
                Y_LOCK(this->access);
                mutable_type *ptr = this->zquery();
                try { return new(ptr) type(argU,argV); } catch(...) { this->zstore(ptr); throw; }
            }

            //! copy another object
            inline type *copycat(const_type *other)
            {
                Y_LOCK(this->access);
                assert(NULL!=other);
                mutable_type *ptr = this->zquery();
                try { return new(ptr) type(*other); } catch(...) { this->zstore(ptr); throw; }
            }

            //! release a previously aquired object
            inline void release(type *obj) throw()
            {
                Y_LOCK(this->access);
                assert(obj!=NULL);
                mutable_type *ptr = (mutable_type *)obj;
                ptr->~mutable_type();
                this->zstore(ptr);
            }

            //__________________________________________________________________
            //
            // memory management
            //__________________________________________________________________

            //! keep at most n items
            inline void at_most(const size_t n)
            {
                Y_LOCK(this->access);
                this->limit(n);
            }

            //! reserve extra n items
            inline void reserve(const size_t n)
            {
                Y_LOCK(this->access);
                this->fetch(n);
            }


            //__________________________________________________________________
            //
            //! base class for list
            //__________________________________________________________________
            typedef core::list_of<T> list__;

            //__________________________________________________________________
            //
            //! specialized list
            //__________________________________________________________________
            class list_ : public list__, public releasable
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! setup empty
                inline explicit     list_()   throw() : list__() {}

                //! cleanup
                inline virtual     ~list_()   throw() { clr(); }

                //! release all node
                inline virtual void release() throw() { clr(); }

                //! full copy
                inline              list_(const list_ &other) : list__(), releasable()
                {
                    try {
                        static magazine &mgr = magazine::instance();
                        for(const_type *node=other.head;node;node=node->next)
                        this->push_back( mgr.copycat(node) );
                    }
                    catch(...) { clr(); throw; }
                }

                //! copy/swap
                inline list_ & operator=( const list_ &other )
                {
                    list_ tmp(other);
                    this->swap_with(tmp);
                    return *this;
                }

                //______________________________________________________________
                //
                // helpers
                //______________________________________________________________
                inline list_ & operator<<(type *obj)
                {
                    this->push_back( (mutable_type*) obj );
                    return *this;
                }



            private:
                inline void clr() throw() {
                    while(this->size)
                    {
                        static magazine &mgr = magazine::location();
                        mgr.release(this->pop_back());
                    }
                }
            };

            //__________________________________________________________________
            //
            //! specialized auto_ptr
            //__________________________________________________________________
            class auto_ptr : public ptr<T>
            {
            public:
                //! setup
                inline explicit auto_ptr(type *obj) throw() : ptr<T>(obj) {}

                //! cleanup
                inline virtual ~auto_ptr() throw() { clr(); }

                //! yield content
                inline type    *yield() throw()
                {
                    type *addr    = this->pointee;
                    this->pointee = NULL;
                    return addr;
                }

                //! assign another pointer
                inline auto_ptr & operator=(type *obj) throw()
                {
                    if(obj!=this->pointee)
                    {
                        clr();
                        this->pointee = (mutable_type *)obj;
                    }
                    return *this;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(auto_ptr);
                inline void clr() throw()
                {
                    if(this->pointee)
                    {
                        static magazine &mgr = magazine::location();
                        mgr.release(this->pointee);
                        this->pointee=0;
                    }
                }

            };

        private:
            Y_DISABLE_COPY_AND_ASSIGN(magazine);

            inline explicit magazine() : singleton_type(), supply_type()
            {
            }

            inline virtual ~magazine() throw()
            {
            }
        };
    }
    
}

#endif
