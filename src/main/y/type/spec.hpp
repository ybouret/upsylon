// \file
#ifndef Y_TYPE_SPEC_INCLUDED
#define Y_TYPE_SPEC_INCLUDED 1

#include "y/string.hpp"
#include "y/type/traits.hpp"
#include <typeinfo>
#include "y/core/list.hpp"

namespace upsylon
{

    //! type specification
    class type_spec : public counted_object
    {
    public:

        //! a mode for an alias
        class alias : public object
        {
        public:
            explicit alias(const string &); //!< set up the name
            virtual ~alias() throw();       //!< cleanup
            alias       *next;              //!< for list
            alias       *prev;              //!< for list
            const string name;              //!< alias name
        private:
            Y_DISABLE_COPY_AND_ASSIGN(alias);
        };


        friend bool operator==(const type_spec &lhs, const type_spec &rhs) throw(); //!< equality
        friend bool operator==(const type_spec &lhs, const string    &rhs);         //!< test name
        friend bool operator==(const type_spec &lhs, const char      *rhs);         //!< test name
        friend bool operator==(const string    &lhs, const type_spec &rhs);         //!< test name
        friend bool operator==(const char      *lhs, const type_spec &rhs);         //!< test name

        //! list of aliases
        typedef core::list_of_cpp<alias> aliases;

        const std::type_info &info; //!< the unique system type_info
        const string          uuid; //!< UUID from system=mangled name
        const aliases         user; //!< user aliases

        explicit type_spec(const std::type_info &tid);                     //!< setup
        explicit type_spec(const std::type_info &id, const string &known); //!< setup and add a default alias
        explicit type_spec(const std::type_info &id, const char   *known); //!< steup and add a default alias
        virtual ~type_spec() throw(); //!< cleanup

        bool aka(const string &usr); //!< check and append an alias
        bool aka(const char   *usr); //!< check and append an alias

        const string         & name() const throw(); //!< default name : uuid of first alias
        const std::type_info & key() const throw();  //!< default key

        static const type_spec & declare(const std::type_info &tid );                 //!< fill in database
        static const type_spec & aka(const std::type_info &tid, const string &known); //!< fill in database with alias
        static const type_spec & aka(const std::type_info &tid, const char   *known); //!< fill in database with alias

        //! get/set type spec for type
        template <typename T> inline
        static const type_spec & of()
        {
            return declare( typeid( typename type_traits<T>::mutable_type ) );
        }

        //! get/set type spec for type, append an alias
        template <typename T> inline
        static const type_spec & of(const string &known)
        {
            return aka(typeid( typename type_traits<T>::mutable_type ),known);
        }

        //! get/set type spec for type, append an alias
        template <typename T>
        static const type_spec & of(const char *known)
        {
            const string _(known);return of<T>(_);
        }

        //! display a type_spec
        friend std::ostream & operator<<( std::ostream &, const type_spec & );

        //! display registered type spec
        static void display( std::ostream &);


    private:
        Y_DISABLE_COPY_AND_ASSIGN(type_spec);
    };

    template <typename T> static inline
    const type_spec & type_spec_of(void)
    {
        return type_spec::of<T>();
    }

    template <typename T> static inline
    const type_spec & type_spec_of(T &)
    {
        return type_spec_of<T>();
    }

    template <typename T> static inline
    const string & type_name_of(void)
    {
        return type_spec_of<T>().name();
    }

    template <typename T> static inline
    const string & type_name_of( T & )
    {
        return type_name_of<T>();
    }




}

#endif

