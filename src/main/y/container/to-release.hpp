//! \file
#ifndef Y_TO_RELEASE_INCLUDED
#define Y_TO_RELEASE_INCLUDED 1

#include "y/type/releasable.hpp"
#include "y/core/list.hpp"
#include "y/object.hpp"

namespace upsylon
{

    //! manage multiple releasable objects
    class to_release
    {
    public:
        //! node to store host
        class node_type : public object
        {
        public:
            releasable &host; //!< reference to object
            node_type  *next; //!< for list
            node_type  *prev; //!< for list
            //! initialize
            inline node_type( releasable &ref ) throw() : host(ref), next(0), prev(0) {}
            //! desctructor
            inline virtual ~node_type() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(node_type);
        };

        //! register
        template <typename T> inline
        T & operator()( T &host )
        {
            hosts.push_back( new node_type(host) );
            return host;
        }

        //! constructor
        inline explicit to_release() throw() : hosts() {}
        //! destructor
        inline virtual ~to_release() throw() {}

        //! release all releasable
        void release_all() throw()
        {
            for(node_type *node=hosts.tail;node;node=node->prev)
            {
                node->host.release();
            }
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(to_release);
        core::list_of_cpp<node_type> hosts;
    };
};

#endif
