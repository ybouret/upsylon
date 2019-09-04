//! \file
#ifndef Y_COUNT_DANCING_INCLUDED
#define Y_COUNT_DANCING_INCLUDED 1

#include "y/core/list.hpp"
#include "y/object.hpp"
#include <iostream>

namespace upsylon
{
    //! solving the dancing party problem for parallelisation
    class dancing
    {
    public:
        //----------------------------------------------------------------------
        //
        // a guest has a unique identifier
        //
        //----------------------------------------------------------------------

        //! a guest for a group
        class guest : public object
        {
        public:
            typedef core::list_of_cpp<guest> list_type;
            const size_t label; //!< identifier
            guest       *next;  //!< for list
            guest       *prev;  //!< for list

            explicit guest(const size_t i) throw(); //!< setup
            virtual ~guest() throw(); //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(guest);
        };

        typedef guest::list_type guests; //!< alias

        //----------------------------------------------------------------------
        //
        // a group is a list of distinct guests
        //
        //----------------------------------------------------------------------
        //! a group of guests
        class group : public object, public guests
        {
        public:
            typedef core::list_of_cpp<group> list_type; //!< alias
            group *next; //!< for list
            group *prev; //!< for list

            explicit group() throw();
            virtual ~group() throw();
            

            bool has_guest_with_label( const size_t label ) const throw(); //!< check amongst guests
            bool is_distinct_from( const group *grp )       const throw(); //!< check no common guest

            //! display for information
            friend std::ostream & operator<<( std::ostream &os, const group &grp );

            //! create a  group with a single guest
            static group * single( const size_t label );

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(group);
        };

        typedef group::list_type groups; //!< alias

        //----------------------------------------------------------------------
        //
        // a configuration is a list of groups and singles
        //
        //----------------------------------------------------------------------
        class configuration : public groups
        {
        public:
            typedef core::list_of_cpp<configuration> list_type; //!< alias
            const size_t   workgroup_size; //!< working group size
            const size_t   workgroups;     //!< groups with matching size
            const size_t   extraneous;     //!< single, if something to do
            configuration *next;           //!< for list of configurations
            configuration *prev;           //!< for list of configuration

            explicit configuration(const size_t wgs) throw();    //!< setup
            virtual ~configuration() throw();                    //!< cleanup
            bool would_accept( const group *grp ) const throw(); //!< would accept if no common guest
            void finalize( const size_t n );                     //!< check all singles, update values

            //!display
            friend std::ostream & operator<<( std::ostream &os, const configuration &cfg );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(configuration);
        };
        

        const configuration::list_type configurations; //!< all the possible configuration

        //! build a set of parallel configurations for n guest to be combined in k-sized groups
        explicit dancing(const size_t n,
                         const size_t k);

        //! cleanup
        virtual ~dancing() throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dancing);
    };
}

#endif

