
//! \file

#ifndef Y_IOS_SCRIBES_INCLUDED
#define Y_IOS_SCRIBES_INCLUDED 1

#include "y/ios/scribe/tuple.hpp"
#include "y/associative/suffix-tree.hpp"

namespace upsylon {

    namespace ios {

        class scribes : public singleton<scribes>
        {
        public:
            typedef arc_ptr<const scribe>       writer;
            typedef arc_ptr<const basic_scribe> basic_writer; //!< handle of basic format
            typedef suffix_tree<writer>         writers_db;
            typedef suffix_tree<basic_writer>   basic_writers_db;

            static const at_exit::longevity life_time = 0;

            const writers_db       writers;
            const basic_writers_db basic_writers;

            

            bool has(const std::type_info &) const throw();
            bool has_basic(const std::type_info &) const throw();

            const scribe *search( const std::type_info & ) const throw();

            template <typename T>
            inline const scribe *search(const std::type_info &) const throw()
            {
                return search( typeid(T) );
            }

            const basic_scribe *search_basic( const std::type_info & ) const throw();

            template <typename T>
            inline const basic_scribe *search_basic(const std::type_info &) const throw()
            {
                return search_basic( typeid(T) );
            }

            const scribe & get( const std::type_info & ) const;

            template <typename T>
            inline const scribe & get() const
            {
                return get( typeid(T) );
            }

            const basic_scribe & get_basic( const std::type_info & ) const;

            template <typename T>
            inline const basic_scribe & get_basic() const
            {
                return get_basic( typeid(T) );
            }

            template <typename T> inline
            bool has() const throw()
            {
                return has( typeid(T) );
            }

            template <typename T> inline
            bool has_basic() const throw()
            {
                return has_basic( typeid(T) );
            }

            string & format( const std::type_info &);

            template <typename T> inline
            string & format()
            {
                return format( typeid(T) );
            }

            template < template <typename> class TUPLE, typename T>
            void tuple()
            {
                const basic_writer  h = & get_basic<T>();
                const writer        w = new tuple_scribe_for<TUPLE,T>(h);
                insert(w);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(scribes);
            friend class singleton<scribes>;
            explicit scribes();
            virtual ~scribes() throw();



            void insert(const basic_writer &w);
            void insert(const writer       &w);
            
        };

    }
}

#endif

