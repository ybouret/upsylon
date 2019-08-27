//! \file
#ifndef Y_IOS_UPACK_INCLUDED
#define Y_IOS_UPACK_INCLUDED 1

#include "y/code/round.hpp"
#include "y/type/args.hpp"

namespace upsylon
{

    namespace ios
    {

        //! base class for unsigned type pack/unpack
        class upack_
        {
        public:
            virtual ~upack_() throw();
            const size_t size; //!< current size

        protected:
            explicit upack_() throw();
            void clear(void *p,const size_t n) throw();

            void encode(void *addr, const void *u, int2type<1> ) throw();
            void encode(void *addr, const void *u, int2type<2> ) throw();
            void encode(void *addr, const void *u, int2type<4> ) throw();
            void encode(void *addr, const void *u, int2type<8> ) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(upack_);
        };

        template <typename T>
        class upack : public upack_
        {
        public:
            Y_DECL_ARGS(T,type);
            static const size_t type_size = sizeof(type);
            static const size_t requested = type_size + 1;
            static const size_t workspace = Y_MEMALIGN(requested);

            inline virtual ~upack() throw()             { reset(); }
            inline explicit upack() throw() :  data()   { reset(); }

            inline const char &operator[](const size_t i) const throw()
            {
                assert(i<size);
                return data[i];
            }

            inline const char * operator*() const throw()
            {
                return data;
            }

            void operator()( const_type u ) throw()
            {
                encode(data, &u, int2type<type_size>() );
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(upack);
            char data[ workspace ];
            inline void reset() throw() { clear(data,sizeof(data)); }
        };

    }

}

#endif

