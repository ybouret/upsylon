#ifndef Y_MEMORY_MAGAZINE_INCLUDED
#define Y_MEMORY_MAGAZINE_INCLUDED 1

#include "y/memory/tight/supply.hpp"
#include "y/concurrent/singleton.hpp"

namespace upsylon
{
    
    namespace memory
    {
        
        template <typename T>
        class magazine : public singleton< magazine<T> >, public tight::supply_of<T>
        {
        public:
            Y_DECL_ARGS(T,type);

            typedef singleton< magazine<T> > singleton_type;
            typedef tight::supply_of<T>      supply_type;

            Y_SINGLETON_DECL(magazine);

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

            //! release a previously aquired object
            inline void release(type *obj) throw()
            {
                Y_LOCK(this->access);
                assert(obj!=NULL);
                mutable_type *ptr = (mutable_type *)obj;
                ptr->~mutable_type();
                this->zstore(ptr);
            }

            inline void reserve(const size_t n)
            {
                Y_LOCK(this->access);
                this->fetch(n);
            }

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
