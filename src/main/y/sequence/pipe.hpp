//! \file
#ifndef Y_SEQUENCE_PIPE_INCLUDED
#define Y_SEQUENCE_PIPE_INCLUDED 1

#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"

#include "y/type/args.hpp"

namespace upsylon
{
    //! kind of pipe
    enum pipe_type
    {
        FIFO, //!< First In, First Out
        LIFO  //!< Last  In, Last  Out
    };

    //! base class for a pipe (FIFO/LIFO)
    template <
    typename  SEQUENCE,
    pipe_type TYPE
    >
    class pipe : public SEQUENCE
    {
    public:
        typedef typename SEQUENCE::type       type;       //!< alias
        typedef typename SEQUENCE::const_type const_type; //!< alias
        typedef typename SEQUENCE::param_type param_type; //!< alias

        //! constructor
        inline explicit pipe() throw() : SEQUENCE() {}

        //! constructor
        inline explicit pipe(const size_t n, const as_capacity_t &_) : SEQUENCE(n,_) {}

        //! copy if possible
        inline pipe( const pipe &other ) : SEQUENCE(other) {}

        //! push an item
        inline void push( param_type args )
        {
            this->push_back( args );
        }

        //! peek an item
        inline type       & peek() throw()       { assert(this->size()>0); return _peek( int2type<TYPE>() ); }

        //! peek an item, CONST
        inline const_type & peek() const throw() { assert(this->size()>0); return _peek( int2type<TYPE>() ); }

        //! remove an item
        inline void pop() throw() { assert(this->size()>0); _pop( int2type<TYPE>() ); }


    private:
        Y_DISABLE_ASSIGN(pipe);
        inline type       & _peek( int2type<FIFO> ) throw()       { return this->front(); }
        inline const_type & _peek( int2type<FIFO> ) const throw() { return this->front(); }
        inline void         _pop(  int2type<FIFO> ) throw() { this->pop_front(); }

        inline type       & _peek( int2type<LIFO> ) throw()       { return this->back();  }
        inline const_type & _peek( int2type<LIFO> ) const throw() { return this->back();  }
        inline void         _pop(  int2type<LIFO> ) throw() { this->pop_back(); }
    };

    //! default optimized queue
    template <typename T>
    class queue : public pipe< list<T>, FIFO >
    {
    public:
        typedef pipe< list<T>, FIFO > queue_type;

        //! destructor
        inline virtual ~queue() throw() {}

        //! default constructor
        inline explicit queue() throw() : queue_type() {}

        //! constructor
        inline explicit queue(const size_t n, const as_capacity_t &_) : queue_type(n,_) {}

        //! copy
        inline queue(const queue &other) : queue_type(other) {}


    private:
        Y_DISABLE_ASSIGN(queue);
    };

    //! default optimized stack
    template <typename T, typename ALLOCATOR = memory::global>
    class vstack : public pipe< vector<T,ALLOCATOR>, LIFO >
    {
    public:
        typedef pipe< vector<T,ALLOCATOR>, LIFO > stack_type;

        //! destructor
        inline virtual ~vstack() throw() {}

        //! default constructor
        inline explicit vstack() throw() : stack_type() {}

        //! constructor
        inline explicit vstack(const size_t n, const as_capacity_t &_) : stack_type(n,_) {}

        //! copy
        inline vstack(const vstack &other) : stack_type(other) {}

    private:
        Y_DISABLE_ASSIGN(vstack);
    };

    //! list based stack
    template <typename T>
    class lstack : public pipe< list<T>, LIFO >
    {
    public:
        typedef pipe< list<T>, LIFO > stack_type;

        //! destructor
        inline virtual ~lstack() throw() {}

        //! default constructor
        inline explicit lstack() throw() : stack_type() {}

        //! constructor
        inline explicit lstack(const size_t n, const as_capacity_t &_) : stack_type(n,_) {}

        //! copy
        inline lstack(const lstack &other) : stack_type(other) {}

    private:
        Y_DISABLE_ASSIGN(lstack);
    };


    
}

#endif

