
//! \file

#ifndef Y_TYPE_RTTI_INCLUDED
#define Y_TYPE_RTTI_INCLUDED 1

#include "y/string.hpp"
#include "y/core/list.hpp"
#include "y/ptr/intr.hpp"
#include "y/ptr/auto.hpp"
#include "y/associative/hash/set.hpp"
#include "y/associative/hash/map.hpp"
#include "y/associative/suffix/address.hpp"
#include <typeinfo>

namespace upsylon
{
    
    //__________________________________________________________________________
    //
    //
    //! Run Time Type Information
    //
    //__________________________________________________________________________
    class rtti : public object, public counted, public suffix_address
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef intr_ptr<string,rtti>      pointer; //!< smart pointer
        typedef hash_set<string,pointer>   db_type; //!< database of pointers
        typedef hash_map<string,pointer>   id_type; //!< index of tag => pointers
        typedef db_type::const_iterator    db_iter; //!< iterator on database
        typedef id_type::const_iterator    id_iter; //!< iterator on index

        //______________________________________________________________________
        //
        //! alias node
        //______________________________________________________________________
        class alias : public object
        {
        public:
            explicit alias(const string &); //!< setup
            virtual ~alias() throw();       //!< cleanup
            alias       *next;              //!< for list
            alias       *prev;              //!< for list
            const string name;              //!< alias name
        private:
            void *priv;                      //!< data alignment only
            Y_DISABLE_COPY_AND_ASSIGN(alias);
        };

        //______________________________________________________________________
        //
        //! alias list
        //______________________________________________________________________
        typedef core::list_of_cpp<alias> aliases;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~rtti() throw();               //!< cleanup
        explicit rtti(const std::type_info &); //!< full setup
        

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        const string & key()  const throw();                              //!< uuid
        const string & name() const throw();                              //!< uuid or first alias
        const char   * text() const throw();                              //!< from name()
        friend bool operator==(const rtti &lhs, const rtti &rhs) throw(); //!< test uuids
        friend bool operator!=(const rtti &lhs, const rtti &rhs) throw(); //!< test uuids
        friend std::ostream & operator<<( std::ostream &, const rtti &);  //!< display
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const string   uuid; //!< type_info.name
        const aliases  user; //!< user names
        
        //______________________________________________________________________
        //
        //! repository
        //______________________________________________________________________
        class repo : public singleton<repo>
        {
        public:
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! register a new primary type info
            /**
             ensure that rtti is register only once in db and id
             */
            const rtti & operator()(const std::type_info &);
            
            //! alias a new or existing rtti
            const rtti & operator()(const std::type_info &, const string &);
            
            //! alias a new or existing rtti, wrapper
            const rtti & operator()(const std::type_info &, const char   *);

            //! wrapper for type computation
            template <typename T> inline
            const rtti & use() { return (*this)( typeid(T) ); }

            //! wrapper for type computation
            template <typename T, typename TAG> inline
            const rtti & use(const TAG &tag) { return (*this)( typeid(T), tag ); }
            
            //! wrapper for type computation
            template <typename T> inline
            const rtti & of(const T & ) { return (*this)( typeid(T) ); }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const db_type db; //!< database
            const id_type id; //!< glossary
            const size_t  mx; //!< uuid max length

        private:
            explicit repo();
            virtual ~repo() throw();
            Y_SINGLETON_DECL_WITH(string::life_time-4,repo);
            const rtti & update_mx(const rtti &) throw();
            
            void import();
            
        };

    private:
        Y_DISABLE_COPY_AND_ASSIGN(rtti);
    
    public:

        //______________________________________________________________________
        //
        // static API to use global repo
        //______________________________________________________________________

        //! fetch rtti for typeid
        static const rtti & of(const std::type_info &);
        
        //! fetch rtti for type
        template <typename T> inline
        static const rtti & of()
        {
            return of( typeid(T) );
        }
        
        //! fetch rtti name for typeid
        static const string & name_of(const std::type_info &);
        
        //! fetch rtti name for type
        template <typename T> inline
        static const string & name_of()
        {
            return name_of( typeid(T) );
        }
        
        //! name() -> filename
        static string to_file_name(const string &);
        
    };
    
    




}

#endif
