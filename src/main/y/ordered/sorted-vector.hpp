
//! \file
#ifndef Y_SORTED_VECTOR_INCLUDED
#define Y_SORTED_VECTOR_INCLUDED 1

#include "y/container/ordered.hpp"
#include "y/memory/global.hpp"
#include "y/comparator.hpp"
#include "y/core/locate.hpp"
#include "y/iterate/linear.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/cswap.hpp"
#include <cstring>

namespace upsylon {

    //! setup sorted vector
#define Y_SORTED_VECTOR(N) \
size_(0), maxi_(N), bytes(0), hmem( ALLOCATOR::instance() ), addr( hmem.acquire_as<mutable_type>(maxi_,bytes) ), item(addr-1), compare()

    //! a vector of sorted objects
    template <typename T,
    typename COMPARATOR = increasing_comparator<T>,
    typename ALLOCATOR = memory::global>
    class sorted_vector : public ordered<T>
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases

        //----------------------------------------------------------------------
        //
        // C++ interfaces
        //
        //----------------------------------------------------------------------

        //! cleanup
        inline virtual ~sorted_vector() throw() { release__(); }

        //! setup
        inline explicit sorted_vector() throw() :
        ordered<T>(), size_(0), maxi_(0), bytes(0), hmem( ALLOCATOR::instance() ), addr(0), item(addr-1), compare()
        {}

        //! setup with memory
        inline explicit sorted_vector(const size_t n, const as_capacity_t &) throw() :  ordered<T>(), Y_SORTED_VECTOR(n) {}

        //! copy
        inline  sorted_vector( const sorted_vector &other ) : collection(), ordered<T>(other), Y_SORTED_VECTOR( other.size_ ) { duplicate(other); }

        //! copy with extra memory
        inline  sorted_vector( const sorted_vector &other, const size_t extra ) : ordered<T>(other), Y_SORTED_VECTOR( other.size_+extra ) { duplicate(other); }

        //----------------------------------------------------------------------
        //
        // dynamic/collection interface
        //
        //----------------------------------------------------------------------
        inline virtual size_t  size()     const throw() { return size_; }
        inline virtual size_t  capacity() const throw() { return maxi_; }

        //----------------------------------------------------------------------
        //
        // container interface
        //
        //----------------------------------------------------------------------

        inline virtual void    free()    throw() { free__();    }
        inline virtual void    release() throw() { release__(); }
        inline virtual void    reserve(const size_t n)
        {
            if(n>0)
            {
                sorted_vector temp( *this, n );
                swap_with(temp);
            }
        }

        //----------------------------------------------------------------------
        //
        // ordered interface
        //
        //----------------------------------------------------------------------
        inline virtual const_type *search( param_type args ) const throw()
        {
            size_t      idx = 0;
            return core::locate(args,addr,size_,compare,idx);
        }

        //! remove one occurence
        inline virtual bool remove( param_type args ) throw()
        {
            size_t        indx   = 0;
            mutable_type *target = core::locate(args,addr,size_,compare,indx);
            if( target )
            {
                self_destruct( *target  );
                __move(target, target+1, (--size_-indx) * sizeof(type) );
                __zset(addr+size_,sizeof(type) );
                return true;
            }
            else
            {
                return false;
            }
        }

        //! head object in this order
        inline virtual const_type &head() const throw()
        {
            assert(size_>0);
            return addr[0];
        }

        //! tail object in this order
        inline virtual const_type &tail() const throw()
        {
            assert(size_>0);
            return item[size_];
        }

        //! check memory localisation
        inline virtual bool owns( const_type &obj ) const throw()
        {
            const_type *p = &obj;
            return (p>=addr) && ( p < addr+size_);
        }

        //----------------------------------------------------------------------
        //
        // iterators
        //
        //----------------------------------------------------------------------

        typedef iterate::linear<const_type,iterate::forward> iterator;                           //!< forward iterator
        typedef iterator                                     const_iterator;                     //!< const forward iterator
        inline  iterator begin() const throw() { return iterator(addr);       }                  //!< begin forward
        inline  iterator end()   const throw() { return iterator(addr+size_); }                  //!< end forward
        typedef iterate::linear<const_type,iterate::reverse> reverse_iterator;                   //!< reverse iterator
        typedef reverse_iterator                             const_reverse_iterator;             //!< const reverse iterator
        inline  reverse_iterator rbegin() const throw() { return reverse_iterator(item+size_); } //!< begin reverse
        inline  reverse_iterator rend()   const throw() { return reverse_iterator(item);       } //!< end reverse

        //----------------------------------------------------------------------
        //
        // specific
        //
        //----------------------------------------------------------------------


        //! display
        inline friend std::ostream & operator<< ( std::ostream &os, const sorted_vector &v )
        {
            os << '[';
            for(size_t i=0;i<v.size_;++i)
            {
                os << ' ' << v.addr[i];
            }
            return os << ']' << '\'';
        }

        //! no throw swap
        inline void swap_with( sorted_vector &_ ) throw()
        {
            cswap(size_,_.size_);
            cswap(maxi_,_.maxi_);
            cswap(bytes,_.bytes);
            cswap(addr,_.addr);
            cswap(item,_.item);
        }


    protected:
        //! put args a a valid place
        inline void insert_multiple( const_type &args )
        {
            size_t where = 0;
            (void) core::locate(args,addr,size_,compare,where);
            insert_at(where,args);
        }

        //! put args at its only possible place
        inline bool insert_single( const_type &args )
        {
            size_t where = 0;
            if( core::locate(args,addr,size_,compare,where) )
            {
                return false; // exists
            }
            else
            {
                insert_at(where,args);
                return true; // did not exist
            }
        }

    private:
        Y_DISABLE_ASSIGN(sorted_vector);
        size_t             size_;
        size_t             maxi_;
        size_t             bytes;
        memory::allocator &hmem;
        mutable_type      *addr;
        mutable_type      *item;
        mutable COMPARATOR compare;

        inline void free__() throw()
        {
            while(this->size_>0)
            {
                self_destruct(addr[--(this->size_)]);
            }
        }

        inline void release__() throw()
        {
            free__();
            hmem.release_as(addr,maxi_,bytes);
            item = 0;
        }

        inline void duplicate( const sorted_vector &other )
        {
            assert(0==size_);
            assert(maxi_>=other.size_);
            while(size_<other.size_)
            {
                new ( &addr[size_] ) mutable_type( other.addr[size_] );
                ++size_;
            }
        }

        static inline void __copy( mutable_type *target, const mutable_type *source, const size_t number_of_bytes ) throw()
        {
            assert(0==number_of_bytes%sizeof(type));
            memcpy( memory::io::__addr(target), memory::io::__addr(source), number_of_bytes );
        }

        static inline void __move( mutable_type *target, const mutable_type *source, const size_t number_of_bytes ) throw()
        {
            assert(0==number_of_bytes%sizeof(type));
            memmove( memory::io::__addr(target), memory::io::__addr(source), number_of_bytes );
        }

        static inline void __zset(mutable_type *target,const size_t number_of_bytes ) throw()
        {
            assert(0==number_of_bytes%sizeof(type));
            memset( memory::io::__addr(target), 0, number_of_bytes);
        }

        inline void insert_at( const size_t where, const_type &args )
        {
            const size_t   full_bytes = size_ * sizeof(T);
            const size_t   prev_bytes = where * sizeof(T);
            const size_t   next_bytes = full_bytes-prev_bytes;

            if( this->is_filled() )
            {
                // get memory
                sorted_vector temp( this->next_capacity(maxi_), as_capacity );

                // get target to build object
                mutable_type *target = &temp.addr[where];
                new ( target ) mutable_type(args);
                // move memory
                __copy(temp.addr,&addr[0],    prev_bytes);
                __copy(target+1, &addr[where],next_bytes);
                __zset(addr,full_bytes);
                temp.size_ = size_+1;
                size_ = 0;
                swap_with(temp);
            }
            else
            {
                mutable_type *target = &addr[where];
                mutable_type *destin = target+1;
                __move(destin,target,next_bytes);
                try
                {
                    new (target) mutable_type(args);
                }
                catch(...)
                {
                    __move(target,destin,next_bytes);
                    __zset(addr+size_,sizeof(type));
                    throw;
                }
                ++size_;
            }
        }

        virtual const_type & getObjectAt(const size_t indx) const throw()
        {
            return item[indx];
        }


    };
}

#endif

