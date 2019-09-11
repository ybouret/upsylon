//! \file
#ifndef Y_ORDERED_INCLUDED
#define Y_ORDERED_INCLUDED 1

#include "y/container/container.hpp"
#include "y/type/args.hpp"
namespace upsylon
{

    
    template <typename T>
    class ordered : public container
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases
        
        inline virtual ~ordered() throw() {}

        virtual const_type *search( param_type args ) const throw() = 0;

    protected:
        inline explicit ordered() throw() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ordered);
    };

    template <typename ORDERED>
    class ordered_multiple : public ORDERED
    {
    public:
        typedef typename ORDERED::param_type param_type;

        inline explicit ordered_multiple() throw() : ORDERED() {}
        inline explicit ordered_multiple(const size_t n, const as_capacity_t &_) : ORDERED(n,_) {}
        inline virtual ~ordered_multiple() throw() {}
        inline ordered_multiple( const ordered_multiple &other) : ORDERED(other) {}

        inline void insert( param_type args )
        {
            this->insert_multiple(args);
        }

    private:
        Y_DISABLE_ASSIGN(ordered_multiple);
    };

    template <typename ORDERED>
    class ordered_single : public ORDERED
    {
    public:
        typedef typename ORDERED::param_type param_type;

        inline explicit ordered_single() throw() : ORDERED() {}
        inline explicit ordered_single(const size_t n, const as_capacity_t &_) : ORDERED(n,_) {}
        inline virtual ~ordered_single() throw() {}
        inline ordered_single( const ordered_single &other) : ORDERED(other) {}

        inline void insert( param_type args )
        {
            (void)this->insert_single(args);
            assert( this->search(args) );
        }

    private:
        Y_DISABLE_ASSIGN(ordered_single);
    };

    template <typename ORDERED>
    class ordered_unique : public ORDERED
    {
    public:
        typedef typename ORDERED::param_type param_type;

        inline explicit ordered_unique() throw() : ORDERED() {}
        inline explicit ordered_unique(const size_t n, const as_capacity_t &_) : ORDERED(n,_) {}
        inline virtual ~ordered_unique() throw() {}
        inline ordered_unique( const ordered_unique &other) : ORDERED(other) {}

        inline bool insert( param_type args )
        {
            return this->insert_single(args);
        }

    private:
        Y_DISABLE_ASSIGN(ordered_unique);
    };



}


#endif
