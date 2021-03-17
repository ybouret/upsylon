
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


            //! get min of shacks
            template <typename T> inline
            T get_min() const throw()
            {
                const size_t      num  = size();
                const slots_type &self = *this;
                assert( self[0]->is<T>() );
                T ans = self[0]->as<T>();
                for(size_t i=1;i<num;++i)
                {
                    ans = min_of(ans,self[i]->as<T>());
                }
                return ans;
            }

            //! minmax
            template <typename T> inline
            const T * minmax() throw()
            {
                const size_t          num  = size();
                slots_type           &self = *this;
                lightweight_array<T> &here = self[0]->__<T>();
                {
                    T vmin = here[1];
                    T vmax = here[2];
                    for(size_t i=1;i<num;++i)
                    {
                        const shack &s = *self[i];
                        vmin = min_of(vmin,s.get<T>(1));
                        vmax = max_of(vmax,s.get<T>(2));
                    }

                    here[1] = vmin;
                    here[2] = vmax;
                }

                return &here[1];
            }

            //! set same value
            template <typename T> inline
            void set( const T &args )
            {
                const size_t num  = size();
                slots_type  &self = *this;
                for(size_t i=0;i<num;++i)
                {
                    self[i]->as<T>() = args;
                    //(*self[i]) = args;
                }
            }

            //! sum (integral) values
            template <typename T> inline
            T sum() const throw()
            {

                const size_t      num  = size();
                const slots_type &self = *this;
                T ans = 0;
                for(size_t i=0;i<num;++i)
                {
                    ans += self[i]->as<T>();
                }
                return ans;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(shacks);
        };



    }

}


#endif

