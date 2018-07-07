//! \file
#ifndef Y_DYNAMIC_INCLUDED
#define Y_DYNAMIC_INCLUDED 1

#include "y/object.hpp"

namespace upsylon
{
    //! common functions for dynamic containers
    class dynamic
    {
    public:
        //! destructor
        virtual ~dynamic() throw();

        //! currently available objects
        virtual size_t size()     const throw() = 0;

        //! maximum available objects
        virtual size_t capacity() const throw() = 0;

        //! available objects
        inline  size_t available() const throw() { assert(size()<=capacity()); return capacity() - size(); }
        
        //! helper
        inline bool is_filled() const throw()  { return size()>=capacity(); }

        //! helper
        inline bool has_space() const throw()  { return size()<capacity();  }

    protected:
        //! constructor
        explicit dynamic() throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dynamic);
    };


}

#include "y/type/args.hpp"
namespace upsylon
{
    template <typename T>
    class dynamic_container_of : public dynamic
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases

        inline virtual ~dynamic_container_of() throw() {}

        inline virtual type       & operator[](const size_t)  throw()       = 0;
        inline virtual const_type & operator[](const size_t ) const throw() = 0;

    protected:
        inline explicit dynamic_container_of() throw() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dynamic_container_of);
    };
}

#endif


