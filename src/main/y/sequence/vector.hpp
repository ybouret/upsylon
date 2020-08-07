//! \file
#ifndef Y_VECTOR_INCLUDED
#define Y_VECTOR_INCLUDED 1

#include "y/memory/allocator/global.hpp"
#include "y/container/sequence.hpp"
#include "y/sequence/array.hpp"
#include "y/iterate/linear.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/cswap.hpp"
#include <cstring>

namespace upsylon {

#if defined(_MSC_VER)
#pragma warning ( disable : 4250 )
#endif
    //! vector of memory movable objects...
    template <typename T, typename ALLOCATOR = memory::global >
    class vector :  public array<T>, public sequence<T>
    {
    public:
        //! aliases
        Y_DECL_ARGS(T,type);

        //! initialize fields
#define Y_VECTOR_CTOR(n)                             \
collection(), addressable<T>(), object(), counted_object(),    \
array<T>(), sequence<T>(),                           \
maxi_(n),  bytes(0), hmem_( ALLOCATOR::instance() ), \
addr_( hmem_.acquire_as<mutable_type>(maxi_,bytes) )

        inline vector() : Y_VECTOR_CTOR(0) {}

        inline virtual ~vector() throw()
        {
            __release();
        }

        //! vector with empty memory
        inline vector(const size_t n, const as_capacity_t &) : Y_VECTOR_CTOR(n)
        {
            this->item_ = addr_-1;
        }

        //! vector with default memory
        inline vector(const size_t n) : Y_VECTOR_CTOR(n)
        {
            this->item_ = addr_-1;
            try
            {
                while(this->size_<n)
                {
                    new (addr_+this->size_) T();
                    ++(this->size_);
                }
            }
            catch(...)
            {
                __release();
                throw;
            }
        }

        //! vector with construction of each item with args
        inline vector(const size_t n, param_type args) : Y_VECTOR_CTOR(n)
        {
            this->item_ = addr_-1;
            try
            {
                while(this->size_<n)
                {
                    new (addr_+this->size_) T(args);
                    ++(this->size_);
                }
            }
            catch(...)
            {
                __release();
                throw;
            }
        }

        //! adjust size and pad if needed
        virtual void adjust( const size_t n, param_type pad )
        {
            while(this->size_>n) this->pop_back();
            this->ensure(n);
            while(this->size_<n) this->push_back_(pad);
        }

        //! make a vector of size n with same values
        inline void make(const size_t n, param_type v)
        {
            free();
            this->ensure(n);
            while(this->size_<n)
            {
                push_back_(v);
            }
        }

        //! vector with construction of each item with func(1...n)
        template <typename FUNC>
        inline vector(const size_t n, FUNC &func) : Y_VECTOR_CTOR(n)
        {
            this->item_ = addr_-1;
            try
            {
                while(this->size_<n)
                {
                    const size_t i = (this->size_)+1;
                    new (addr_+this->size_) T( func(i) );
                    this->size_ = i;
                }
            }
            catch(...)
            {
                __release();
                throw;
            }
        }

        //! copy constructor
        inline vector(const vector &other) : Y_VECTOR_CTOR(other.size_)
        {
            this->item_ = addr_-1;
            try
            {
                while(this->size_<other.size_)
                {
                    new (addr_+this->size_) T( other.addr_[this->size_] );
                    ++(this->size_);
                }
            }
            catch(...)
            {
                __release();
                throw;
            }
        }

        //! assign another vector
        inline vector & operator=( const vector &other )
        {
            if(this != &other)
            {
                vector tmp(other);
                swap_with(tmp);
            }
            return *this;
        }

        //! dynamic interface: capacity
        inline virtual size_t capacity() const throw() { return maxi_; }

        //! dynamic interface: size
        virtual size_t size() const throw() { return this->size_; }


        //! container interface: free
        virtual void free() throw() { __free(); }

        //! container interface: release
        virtual void release() throw() { __release(); }

        //! container interface: reserve
        virtual void reserve(const size_t n)
        {
            if(n>0)
            {
                vector tmp(maxi_+n,as_capacity);
                memcpy((void*)(tmp.addr_),(void*)addr_,(tmp.size_=this->size_)*sizeof(T));
                this->size_ = 0;
                swap_with(tmp);
            }
        }

        //! push back with enough memory
        inline void push_back_(param_type args)
        {
            assert(this->has_space());
            // try to build in place
            new (addr_+this->size_) T(args);
            ++(this->size_);
        }

        //! sequence interface : push_back
        virtual void push_back( param_type args )
        {
            if(this->is_filled())
            {
                vector tmp( container::next_capacity(maxi_), as_capacity );
                // will keep memory in any case
                memcpy( (void*)(tmp.addr_), (void*)addr_,this->size_*sizeof(T));
                try
                {
                    new (tmp.addr_+this->size_) T(args); // put into position
                    tmp.size_ = this->size_ + 1;         // success
                    this->size_ = 0;
                    swap_with(tmp);
                }
                catch(...)
                {
                    tmp.size_   = this->size_; // failure
                    this->size_ = 0;
                    swap_with(tmp);
                    throw;
                }
            }
            else
            {
                push_back_(args);
            }
        }

        //! sequence interface : pop_front
        virtual void push_front( param_type args)
        {
            const size_t len = this->size_ * sizeof(T);
            if(this->is_filled())
            {
                vector tmp( container::next_capacity(maxi_), as_capacity );
                try
                {
                    new(tmp.addr_) T(args);
                    memcpy( (void*)(tmp.addr_+1), (void*)addr_,len);
                    tmp.size_   = this->size_ + 1;
                    this->size_ = 0;
                    swap_with(tmp);
                }
                catch(...)
                {
                    // keep memory
                    memcpy( (void*) tmp.addr_, (void *)addr_, len );
                    tmp.size_   = this->size_;
                    this->size_ = 0;
                    swap_with(tmp);
                    throw;
                }
            }
            else
            {
                memmove((void*)(addr_+1), (void*)(addr_), len);
                try
                {
                    new (addr_) T(args);
                }
                catch(...)
                {
                    memmove((void*)addr_,(void *)(addr_+1),len);
                    throw;
                }
                ++(this->size_);
            }

        }

        //! swap all descriptive data, array included
        inline void swap_with( vector &other ) throw()
        {
            cswap(this->item_,other.item_);
            cswap(this->size_,other.size_);
            cswap(this->addr_,other.addr_);
            cswap(this->maxi_,other.maxi_);
            cswap(this->bytes,other.bytes);
        }

        typedef iterate::linear<type,iterate::forward>        iterator;        //!< forward iterator
        typedef iterate::linear<const_type,iterate::forward>  const_iterator;  //!< forward iterator, const

        inline iterator begin() throw() { return iterator(addr_);             } //!< begin forward
        inline iterator end()   throw() { return iterator(addr_+this->size_); } //!< end forward

        inline const_iterator begin() const throw() { return const_iterator(addr_);             } //!< begin forward const
        inline const_iterator end()   const throw() { return const_iterator(addr_+this->size_); } //!< end forward const

        typedef iterate::linear<type,iterate::reverse>        reverse_iterator;        //!< reverse iterator
        typedef iterate::linear<const_type,iterate::reverse>  const_reverse_iterator;  //!< reverse iterator, const

        inline reverse_iterator rbegin() throw() { return reverse_iterator(this->item_+this->size_); } //!< begin reverse
        inline reverse_iterator rend()   throw() { return reverse_iterator(this->item_);             } //!< end reverse

        inline const_reverse_iterator rbegin() const throw() { return const_reverse_iterator(this->item_+this->size_); } //!< begin reverse const
        inline const_reverse_iterator rend()   const throw() { return const_reverse_iterator(this->item_);             } //!< end reverse const

        inline virtual type       & back()  throw()       { assert(this->size_>0); return this->item_[this->size_]; } //!< sequence interface
        inline virtual const_type & back()  const throw() { assert(this->size_>0); return this->item_[this->size_]; } //!< sequence interface
        inline virtual type       & front() throw()       { assert(this->size_>0); return this->addr_[0]; }           //!< sequence interface
        inline virtual const_type & front() const throw() { assert(this->size_>0); return this->addr_[0]; }           //!< sequence interface

        //! sequence interface
        inline virtual void pop_back() throw()
        {
            // destruct last object
            assert(this->size_>0);
            self_destruct( this->item_[this->size_--] );
        }

        //! sequence interface
        inline virtual void pop_front() throw()
        {
            // destruct first object
            assert(this->size_>0);
            self_destruct( *(this->addr_) );
            // adjust memory
            memmove( static_cast<void *>(this->addr_), static_cast<const void *>(this->addr_+1), --(this->size_) * sizeof(T) );
        }

        //! specific method
        inline void insert_at( const size_t idx, param_type args )
        {
            size_t &sz = this->size_;
            if(idx<=1)
            {
                push_front(args);
            }
            else if(idx>sz)
            {
                push_back(args);
            }
            else
            {
                assert(idx>1);
                assert(idx<=sz);
                if(this->is_filled())
                {
                    //std::cerr << "insert_at[" << idx << "] but filled" << std::endl;
                    vector        temp( container::next_capacity(maxi_), as_capacity );
                    mutable_type *source = & temp.item_[idx];
                    new (source) type(args);
                    ++sz;
                    memcpy( temp.addr_,  this->addr_,      (idx-1) *sizeof(type));
                    memcpy( source+1,   &this->item_[idx], (sz-idx)*sizeof(type));
                    temp.size_  = sz;
                    sz          = 0;
                    swap_with(temp);
                }
                else
                {
                    //std::cerr << "insert_at[" << idx << "] with space" << std::endl;
                    mutable_type *source = & this->item_[idx];
                    mutable_type *target = source+1;
                    ++sz;
                    const size_t  block  = (sz-idx)*sizeof(type);
                    memmove(target,source,block);
                    try { new (source) type(args); } catch(...) { memmove(source,target,block); --sz; throw; }
                }
            }
        }

        //! remove object
        inline void remove_at( const size_t idx ) throw()
        {
            size_t &sz = this->size_;
            assert(sz>0);
            assert(idx>=1);
            assert(idx<=sz);
            mutable_type *target = &this->item_[idx];
            self_destruct(*target);
            memmove((void*)target,(const void*)(target+1), (sz-idx) * sizeof(type) );
            --sz;
        }

        //! optimized put method
        virtual size_t put(const T *addr, const size_t numObjects)
        {
            assert(!(0==addr&&numObjects>0));
            const size_t old_size = this->size_;
            const size_t new_size = old_size + numObjects;
            if(new_size>this->maxi_)
            {
                vector temp( new_size, as_capacity );
                append_to( temp.addr_ + old_size,addr,numObjects);
                memcpy( (void*)(temp.addr_), (const void*) addr_, old_size * sizeof(T) );
                this->size_ = 0;
                swap_with(temp);
            }
            else
            {
                append_to( addr_+old_size,addr,numObjects);
            }
            this->size_ = new_size;
            return numObjects;
        }


    private:
        size_t             maxi_;
        size_t             bytes;
        memory::allocator &hmem_;
        mutable_type      *addr_;

        //! append and erase if failure
        inline void append_to( mutable_type *target,
                              const T       *addr,
                              const size_t   numObjects)
        {
            size_t        i      = 0;
            try {
                for(;i<numObjects;++i)
                {
                    new (&target[i]) mutable_type( addr[i] );
                }
            }
            catch(...)
            {
                while(i>0) self_destruct(target[i--]);
                throw;
            }

        }

        inline void __free() throw()
        {
            while(this->size_>0)
            {
                self_destruct(addr_[--(this->size_)]);
            }
        }

        inline void __release() throw()
        {
            __free();
            hmem_.release_as(this->addr_,maxi_,bytes);
            this->item_ = 0;
        }
    };


    //! wrapper for template class
    template <typename ALLOCATOR = memory::global >
    struct vector_for
    {
        //! the internal class
        template <typename T> class device : public vector<T,ALLOCATOR>
        {
        public:
            typedef vector<T,ALLOCATOR> self_type;                              //!< alias
            Y_DECL_ARGS(T,type);                                                //!< aliases
            inline explicit device(): self_type() {}                            //!< setup
            inline explicit device( const size_t n, const as_capacity_t &_) :   //|
            self_type(n,_) {}                                                   //!< setup with capacity
            inline explicit device( const size_t n, param_type args) :          //|
            self_type(n,args) {}                                                //!< setup with same value
            inline virtual ~device() throw() {}                                 //!< cleanup
            inline          device(const device &other) : self_type(other) {}   //!< copy
            inline          device & operator=( const device &other)            //|
            {                                                                   //|
                self_type       & target = *this;                               //|
                const self_type & source = other;                               //|
                target = source;                                                //|
                return *this;                                                   //|
            }                                                                   //!< assign
        };
    };
}
#endif

