
//! \file

#ifndef Y_MEMORY_SHACKS_INCLUDED
#define Y_MEMORY_SHACKS_INCLUDED 1

#include "y/memory/shack.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/sequence/slots.hpp"
#include "y/ptr/ref.hpp"
#include "y/type/utils.hpp"


namespace upsylon {

    namespace memory
    {
        //______________________________________________________________________
        //
        //
        //! shared shacks
        //
        //______________________________________________________________________
        class shacks : public slots< ref_ptr<shack>, memory::dyadic >
        {
        public:
            //__________________________________________________________________
            //
            //types and definition
            //__________________________________________________________________
            typedef slots< ref_ptr<shack>, memory::dyadic > slots_type; //!< alias

            //! construct mode
            enum construct_mode
            {
                construct_vacant, //!< no preallocated
                construct_filled  //!< filled with dynamic shacks
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            virtual ~shacks() throw();

            //! setup
            explicit shacks(const size_t n, const construct_mode = construct_vacant);

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            void fill(); //!< fill remaining with default shack

            //! make one object per slot
            template <typename T> inline
            void make()
            {
                slots_type &self = *this;
                for(size_t i=0;i<size();++i) (void) (*self[i]).make<T>();
            }

            //! make n objects per slot
            template <typename T> inline
            void make(const size_t n)
            {
                slots_type &self = *this;
                for(size_t i=0;i<size();++i) (void) (*self[i]).make<T>(n);
            }

            //! get max of shacks
            template <typename T> inline
            T get_max() const throw()
            {

                const size_t      num  = size();
                const slots_type &self = *this;
                assert( self[0]->is<T>() );
                T ans = self[0]->as<T>();
                for(size_t i=1;i<num;++i)
                {
                    ans = max_of(ans,self[i]->as<T>());
                }
                return ans;
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(shacks);
        };



    }

}


#endif

