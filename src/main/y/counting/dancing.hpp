//! \file
#ifndef Y_COUNT_DANCING_INCLUDED
#define Y_COUNT_DANCING_INCLUDED 1

#include "y/object.hpp"
#include "y/core/list.hpp"
#include "y/core/inode.hpp"
#include <iosfwd>

namespace upsylon
{
    //--------------------------------------------------------------------------
    //
    //! solving the dancing party problem for parallelisation
    //
    //--------------------------------------------------------------------------
    class dancing
    {
    public:
        //----------------------------------------------------------------------
        //
        //! a guest has a unique identifier
        //
        //----------------------------------------------------------------------
        class guest : public object, public inode<guest>
        {
        public:
            explicit     guest(const size_t) throw(); //!< setup
            virtual     ~guest() throw();             //!< cleanup
            const size_t label;                       //!< identifier

        private:
            Y_DISABLE_COPY_AND_ASSIGN(guest);
        };

        typedef core::list_of_cpp<guest> guests; //!< alias


        //----------------------------------------------------------------------
        //
        //! a group is a list of distinct guests
        //
        //----------------------------------------------------------------------
        class group : public object, public guests, public inode<group>
        {
        public:
            explicit group() throw();                                  //!< setup
            virtual ~group() throw();                                  //!< cleanup
            bool     has_guest_with_label(const size_t) const throw(); //!< check amongst guests
            bool     is_distinct_from(const group *)    const throw(); //!< check no common guest
            static group * single(const size_t);                       //!< create a  group with a single guest

            //! display for information
            friend std::ostream & operator<<(std::ostream &, const group &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(group);
        };

        typedef core::list_of_cpp<group> groups; //!< alias

        //----------------------------------------------------------------------
        //
        // a frame is a list of groups and singles
        //
        //----------------------------------------------------------------------
        //! description of a configuration
        class frame : public groups, public inode<frame>
        {
        public:
            typedef core::list_of_cpp<frame> list_type; //!< alias
            const size_t   workgroup_size;              //!< working group size
            const size_t   workgroups;                  //!< groups with matching workgroup_size
            const size_t   extraneous;                  //!< single, if something to do
            
            explicit frame(const size_t) throw();            //!< setup
            virtual ~frame() throw();                        //!< cleanup
            bool would_accept(const group *) const throw();  //!< would accept if no common guest
            void finalize(const size_t);                     //!< check all singles, update values

            //!display
            friend std::ostream & operator<<( std::ostream &os, const frame &cfg );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(frame);
        };
        

        //----------------------------------------------------------------------
        //
        // information about distribution
        //
        //----------------------------------------------------------------------
        const frame::list_type frames; //!< all the possible configuration, decreasing workgroups order
        const size_t           wg_max; //!< maximum effective workgroups
        const size_t           wg_min; //!< minimum effective workgroups
        const size_t           amount; //!< total number of sequential groups

        //! build a set of parallel configurations
        /**
         Each configuration is a set of k-uplets and singles
         */
        explicit dancing(const size_t n,
                         const size_t k);

        //! cleanup
        virtual ~dancing() throw();

        //! find configurations with same worgroups
        size_t   find(const size_t  workgroups,
                      const frame * &ini,
                      const frame * &end) const throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dancing);
    };
}

#endif

