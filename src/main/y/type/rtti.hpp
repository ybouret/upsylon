
//! \file

#ifndef Y_TYPE_RTTI_INCLUDED
#define Y_TYPE_RTTI_INCLUDED 1

#include "y/string.hpp"
#include "y/core/list.hpp"
#include <typeinfo>

namespace upsylon
{

    class rtti : public counted, public object
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

        typedef core::list_of<alias> aliases;


        static size_t hash(const char           *) throw();
        static size_t hash(const std::type_info &) throw();
        static size_t hash(const string         &) throw();

        const string   name; //!< type_info.name
        const size_t   code; //!< from internal hash
        const aliases  user; //!< user names
        
        virtual ~rtti() throw();
        explicit rtti(const std::type_info &);
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(rtti);
    };

}

#endif
