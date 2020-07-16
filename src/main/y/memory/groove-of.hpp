//! \file


#ifndef Y_MEMORY_GROOVE_OF_INCLUDED
#define Y_MEMORY_GROOVE_OF_INCLUDED 1

#include "y/memory/groove.hpp"
#include "y/sequence/addressable.hpp"

namespace upsylon {

    namespace memory {

        //______________________________________________________________________
        //
        //! lightweight facade for a groove
        //______________________________________________________________________
        template <typename T>
        class groove_of : public addressable<T>
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            Y_DECL_ARGS(T,type); //!< aliases

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup from a groove that shouldn't change
            inline explicit groove_of(groove &target) throw() :
            host( target ),
            shift( get_shift_from(host) )
            {
                assert( target.label && typeid(type)==(*target.label) );
            }

            //! setup by redefining the groove
            inline explicit groove_of(groove                      &target,
                                      const memory::storage::model model,
                                      const size_t                 count) :
            host(target.vmake<type>(model,count)),
            shift( get_shift_from(host) )
            {

            }

            //! cleanup
            inline virtual ~groove_of() throw() {}

            //__________________________________________________________________
            //
            // addressable interface
            //__________________________________________________________________

            //! number of current items
            inline virtual size_t size() const throw()
            {
                return host.count;
            }

            //! access
            inline virtual type &operator[](const size_t indx) throw()
            {
                assert(indx>=1);
                assert(indx<=size());
                return shift[indx];
            }

            //! access, const
            inline virtual const_type &operator[](const size_t indx) const throw()
            {
                assert(indx>=1);
                assert(indx<=size());
                return shift[indx];
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(groove_of);
            groove       &host;
            mutable_type *shift;
            static inline mutable_type *get_shift_from(groove &target) throw()
            {
                return ( (mutable_type *)(target.entry) ) -1;
            }
        };

    }

}

#endif
