
//! \file

#ifndef Y_CONCURRENT_NUCLEUS_CLUSTER_INCLUDED
#define Y_CONCURRENT_NUCLEUS_CLUSTER_INCLUDED 1

#include "y/object.hpp"
#include "y/core/inode.hpp"
#include "y/core/list.hpp"
#include "y/strfwd.hpp"
#include <iosfwd>

namespace upsylon
{

    namespace concurrent
    {

        namespace nucleus
        {
            //__________________________________________________________________
            //
            //
            //! a cluster is a set of cpus
            //
            //__________________________________________________________________
            class cluster : public object, public inode<cluster>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef core::list_of_cpp<cluster> list; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~cluster() throw(); //!< cleanup

                //! setup with checking
                static cluster *create(const size_t start_,
                                       const size_t width_,
                                       const size_t every_);

                //! parse with checking: width | start:width | start:width:every
                static cluster *create(const string &description);

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                size_t core_of(const size_t rank) const; //!< start + rank*every
                friend std::ostream & operator<<(std::ostream &, const cluster &); //!< display


                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t start; //!< start index
                const size_t width; //!< width <= nprocs-start
                const size_t every; //!< clamp(1,every,width)
                const size_t count; //!< width/every >= 1

            private:
                Y_DISABLE_COPY_AND_ASSIGN(cluster);
                //! setup
                explicit cluster(const size_t start_,
                                 const size_t width_,
                                 size_t       every_) throw();
            };


            

        }


    }

}

#endif

