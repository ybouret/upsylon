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
        //----------------------------------------------------------------------
        //
        // types and definitions
        //
        //----------------------------------------------------------------------

        Y_DECL_ARGS(T,type);                 //!< aliases

        //----------------------------------------------------------------------
        //
        // virtual interface
        //
        //----------------------------------------------------------------------

        inline virtual ~ordered() throw() {}//!< cleanup


        virtual const_type *search( param_type  )   const throw() = 0; //!< search if object is already there
        virtual bool        remove( param_type  )         throw() = 0; //!< remove one object
        virtual const_type &head()                  const throw() = 0; //!< head object in this order
        virtual const_type &tail()                  const throw() = 0; //!< tail object in this order
        virtual bool        owns( const_type &obj ) const throw() = 0; //!< test ownership

        //----------------------------------------------------------------------
        //
        // non-virtual interface
        //
        //----------------------------------------------------------------------
        //! remove all objects
        inline void no( param_type args ) throw()
        {
            while( remove(args) )
                ;
        }
        
        //! access
        inline const_type & operator[]( const size_t indx ) const throw()
        {
            assert(indx>0); assert(indx<=size());
            return getObjectAt(indx);
        }

        //! exclude a range of keys
        template <typename ITERATOR>
        inline void exclude( ITERATOR curr, const ITERATOR last ) throw()
        {
            if( curr != last )
            {
                const_type &obj = *curr;
                if( this->owns( obj ) )
                {
                    // iterator based on this
                    this->free();
                }
                else
                {
                    no(obj);
                    for( ++curr; curr != last; ++curr )
                    {
                        no(*curr);
                    }
                }
            }
        }

        //! exclude all keys
        template <typename ORDERED>
        inline void exclude( const ORDERED &bad ) throw()
        {
            exclude( bad.begin(), bad.end() );
        }

    protected:
        inline explicit ordered()        throw() : container()               {} //!< setup
        inline ordered(const ordered & ) throw() : collection(), container() {} //!< copy



    private:
        Y_DISABLE_ASSIGN(ordered);
        virtual const_type & getObjectAt(const size_t indx) const throw() = 0;
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
        inline ordered_single( const ordered_single &other) : collection(), ORDERED(other) {}                   //!< copy

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
        inline ordered_unique( const ordered_unique &other) : dynamic(), ORDERED(other) {}                  //!< copy

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
