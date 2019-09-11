//! \file
#ifndef Y_ORDERED_INCLUDED
#define Y_ORDERED_INCLUDED 1

#include "y/container/container.hpp"
#include "y/type/args.hpp"
namespace upsylon
{

    //! base class for ordered container
    template <typename T>
    class ordered : public container
    {
    public:
        Y_DECL_ARGS(T,type);                 //!< aliases
        inline virtual ~ordered() throw() {} //!< cleanup

        //! search if object is already there
        virtual const_type *search( param_type args ) const throw() = 0;

        //! remove one object
        virtual bool remove( param_type args ) throw() = 0;

        //! remove all objects
        inline void no( param_type args ) throw()
        {
            while( remove(args) )
                ;
        }

    protected:
        inline explicit ordered() throw() {} //!< setup

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ordered);
    };

    //! use an ordered container to build a multiple ordered container
    template <typename ORDERED>
    class ordered_multiple : public ORDERED
    {
    public:
        typedef typename ORDERED::param_type param_type; //!< alias

        inline explicit ordered_multiple() throw() : ORDERED() {}                                  //!< setup
        inline explicit ordered_multiple(const size_t n, const as_capacity_t &_) : ORDERED(n,_) {} //!< setup with memory
        inline virtual ~ordered_multiple() throw() {}                                              //!< cleanup
        inline ordered_multiple( const ordered_multiple &other) : ORDERED(other) {}                //!< copy

        //! always insert
        inline void insert( param_type args )
        {
            this->insert_multiple(args);
        }

    private:
        Y_DISABLE_ASSIGN(ordered_multiple);
    };

    //! use an ordered container to build a single object ordered container
    /**
     multiple occurences are silently discarded
     */
    template <typename ORDERED>
    class ordered_single : public ORDERED
    {
    public:
        typedef typename ORDERED::param_type param_type;                                          //!< alias

        inline explicit ordered_single() throw() : ORDERED() {}                                   //!< setup
        inline explicit ordered_single(const size_t n, const as_capacity_t &_) : ORDERED(n,_) {}  //!< setup with memory
        inline virtual ~ordered_single() throw() {}                                               //!< cleanup
        inline ordered_single( const ordered_single &other) : ORDERED(other) {}                   //!< copy

        //! insert and discard if multiple objects
        inline void insert( param_type args )
        {
            (void)this->insert_single(args);
            assert( this->search(args) );
        }

    private:
        Y_DISABLE_ASSIGN(ordered_single);
    };

    //! use an ordered container to build a unique object ordered container
    /**
     multiple occurences are reported for counting or error reporting
     */
    template <typename ORDERED>
    class ordered_unique : public ORDERED
    {
    public:
        typedef typename ORDERED::param_type param_type;                                          //!< alias

        inline explicit ordered_unique() throw() : ORDERED() {}                                  //!< setup
        inline explicit ordered_unique(const size_t n, const as_capacity_t &_) : ORDERED(n,_) {} //!< setup with memory
        inline virtual ~ordered_unique() throw() {}                                              //!< cleanup
        inline ordered_unique( const ordered_unique &other) : ORDERED(other) {}                  //!< copy

        //! try to insert object
        inline bool insert( param_type args )
        {
            return this->insert_single(args);
        }

    private:
        Y_DISABLE_ASSIGN(ordered_unique);
    };



}


#endif
