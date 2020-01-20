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

        void aka(const string &usr);
        void aka(const char   *usr);

        const string & name() const throw(); //!< default name

    private:
        Y_DISABLE_COPY_AND_ASSIGN(type_spec);
    };

  



}

#endif

