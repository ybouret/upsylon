
//! \file

#ifndef Y_TYPE_RTTI_INCLUDED
#define Y_TYPE_RTTI_INCLUDED 1

#include "y/string.hpp"
#include "y/core/list.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/hash/set.hpp"
#include "y/associative/suffix/map.hpp"
#include <typeinfo>

namespace upsylon
{

    class rtti : public counted, public object
    {
    public:
        typedef intr_ptr<string,rtti>      pointer;
        typedef hash_set<string,pointer>   db_type;
        typedef suffix_map<string,pointer> id_type;
        typedef db_type::const_iterator    db_iter;
        typedef id_type::const_iterator    id_iter;


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


        static size_t hash(const char           *) throw();
        static size_t hash(const std::type_info &) throw();
        static size_t hash(const string         &) throw();

        const string   uuid; //!< type_info.name
        const size_t   code; //!< from internal hash
        const aliases  user; //!< user names

        const string & key()  const throw(); //!< uuid
        const string & name() const throw(); //!< uuid or first alias

        virtual ~rtti() throw();
        explicit rtti(const std::type_info &);

        class repo : public singleton<repo>
        {
        public:
            const db_type db; //!< database
            const id_type id; //!< glossary
            const size_t  mx; //!< uuid max length

            void operator()(const std::type_info &);
            void operator()(const std::type_info &, const string &);
            void operator()(const std::type_info &, const char   *);

            template <typename T> inline
            void use() { (*this)( typeid(T) ); }

            template <typename T, typename TAG> inline
            void use(const TAG &tag) { (*this)( typeid(T), tag ); }





        private:
            explicit repo();
            virtual ~repo() throw();
            Y_SINGLETON_DECL_WITH(string::life_time-4,repo);
        };

    private:
        Y_DISABLE_COPY_AND_ASSIGN(rtti);
    };





}

#endif
