//! \file
#ifndef Y_ASSOCIATIVE_INCLUDED
#define Y_ASSOCIATIVE_INCLUDED 1

#include "y/container/container.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    //! associative interface
    template <typename KEY,typename T>
    class associative : public container
    {
    public:
        Y_DECL_ARGS(T,type);       //!< alias
        Y_DECL_ARGS(KEY,key_type); //!< alias

        //! desctructor
        inline virtual ~associative() throw() {}

        //! search object by key, may optimize for next call
        virtual type       *search( param_key_type k ) throw()       = 0;

        //! search const object by key, untouched
        virtual const_type *search( param_key_type k ) const throw() = 0;

        //! remove object
        virtual bool        remove( param_key_type k ) throw()       = 0;

        //! ensure no key
        inline void no( param_key_type k ) throw()
        {
            while( remove(k) )
                ;
        }

    protected:
        inline explicit associative() throw() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(associative);
    };
    

}

#endif

