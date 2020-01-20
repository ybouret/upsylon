// \file
#ifndef Y_TYPE_SPEC_INCLUDED
#define Y_TYPE_SPEC_INCLUDED 1

#include "y/string.hpp"
#include "y/type/traits.hpp"
#include <typeinfo>
#include "y/core/list.hpp"

namespace upsylon
{




    class type_spec : public counted_object
    {
    public:
        class alias : public object
        {
        public:
            explicit alias(const string &);
            virtual ~alias() throw();
            alias       *next;
            alias       *prev;
            const string name;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(alias);
        };

        typedef core::list_of_cpp<alias> aliases;

        const std::type_info &info; //!< the unique system type_info
        const string          uuid; //!< UUID from system=mangled name
        const aliases         user; //!< user aliases

        explicit type_spec(const std::type_info &tid);
        explicit type_spec(const std::type_info &id, const string &known);
        explicit type_spec(const std::type_info &id, const char   *known);
        virtual ~type_spec() throw();

        bool aka(const string &usr);
        bool aka(const char   *usr);

        const string         & name() const throw(); //!< default name
        const std::type_info & key() const throw();  //!< default key

        static const type_spec & declare(const std::type_info &tid );
        static const type_spec & aka(const std::type_info &tid, const string &known);
        static const type_spec & aka(const std::type_info &tid, const char   *known);

        template <typename T> inline
        static const type_spec & of()
        {
            return declare( typeid( typename type_traits<T>::mutable_type ) );
        }

        template <typename T> inline
        static const type_spec & of(const string &known)
        {
            return aka(typeid( typename type_traits<T>::mutable_type ),known);
        }

        template <typename T>
        static const type_spec & of(const char *known)
        {
            const string _(known);
            return of<T>(_);
        }

        friend std::ostream & operator<<( std::ostream &, const type_spec & );

        static void display( std::ostream &);


    private:
        Y_DISABLE_COPY_AND_ASSIGN(type_spec);
    };

  



}

#endif

