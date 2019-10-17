//! \file
#ifndef Y_SEQUENCE_INCLUDED
#define Y_SEQUENCE_INCLUDED 1

#include "y/container/container.hpp"
#include "y/sequence/addressable.hpp"
#include "y/type/args.hpp"
#include "y/ptr/counted.hpp"

namespace upsylon
{
    //! a sequence of objects
    template <typename T>
    class sequence :
    public virtual   counted_object,
    public           container,
    public virtual   addressable<T>
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases
        //! destructor
        inline virtual ~sequence() throw() {}

        //! push an object at the back of the sequence
        virtual void push_back( param_type ) = 0;
        
        //! push an object at the front of the sequence
        virtual void push_front( param_type ) = 0;

        //! get the back item
        virtual type &back() throw() = 0;
        
        //! get the back item, const
        virtual const_type & back() const throw() = 0;

        //! get the front item
        virtual type &front() throw() = 0;

        //! get the front item, const
        virtual const_type & front() const throw() = 0;

        //! remove last item
        virtual void pop_back() throw() = 0;

        //! remove first item
        virtual void pop_front() throw() = 0;

        //! syntactic helper
        inline sequence & operator<<( param_type args ) { push_back(args); return *this; }
        
        //! adjust size, pad if not enough
        virtual void adjust( const size_t n, param_type pad ) = 0;
        
    protected:
        //! constructor
        explicit sequence() throw() : container() {}
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(sequence);
    };
}

#endif
