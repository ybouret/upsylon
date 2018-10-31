//! \file
#ifndef Y_PROPERTY_INCLUDED
#define Y_PROPERTY_INCLUDED 1

#include "y/string.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    //! base class for property implementation
    class ppty
    {
    public:
        const string name; //!< property name

        virtual ~ppty() throw(); //! destructor
        explicit ppty(const char *id, const bool rw); //!< setup

        bool is_writable() const throw(); //!< return internal fal
        void check_writable() const;      //!< throw exception if not writable
        
    protected:
        //void set_writable(const bool rw) throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ppty);
        bool writable;
    };

    //! property wrapper
    template <typename T>
    class property : public ppty
    {
    public:
        Y_DECL_ARGS(T,type); //!< alias

        //! destructor
        inline virtual ~property() throw() {}

        //! setup
        inline property( T &args, const char *id, const bool rw ) :
        ppty(id,rw), host( (mutable_type&)args )
        {
        }

        //! cast operator
        inline operator  const_type & () const throw() { return host;  }

        //! assign operator
        inline property & operator=( param_type args )
        {
            check_writable();
            host = args;
            return *this;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(property);
        mutable_type &host;
    };

}

#endif

