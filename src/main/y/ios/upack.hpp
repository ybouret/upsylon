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
            virtual ~upack_() throw(); //!< cleanup
            const size_t size;         //!< current size

        protected:
            explicit upack_() throw(); //!< setup
            void clear(void *p,const size_t n) throw(); //!< clear all

            void encode(void *addr, const void *u, int2type<1> ) throw(); //!< uint8_t
            void encode(void *addr, const void *u, int2type<2> ) throw(); //!< uint16_t
            void encode(void *addr, const void *u, int2type<4> ) throw(); //!< uint32_t
            void encode(void *addr, const void *u, int2type<8> ) throw(); //!< uint64_t

        private:
            Y_DISABLE_COPY_AND_ASSIGN(upack_);
        };

        //! embedding upack
        template <typename T>
        class upack : public upack_
        {
        public:
            Y_DECL_ARGS(T,type);                                     //!< aliases
            static const size_t type_size = sizeof(type);            //!< raw bytes
            static const size_t requested = type_size + 1;           //!< max encoded bytes
            static const size_t workspace = Y_MEMALIGN(requested);   //!< aligned bytes

            inline virtual ~upack() throw()             { reset(); } //!< cleanup
            inline explicit upack() throw() :  data()   { reset(); } //!< setup

            //! byte access
            inline const char &operator[](const size_t i) const throw()
            {
                assert(i<size);
                return data[i];
            }

            //! field access
            inline const char * operator*() const throw()
            {
                return data;
            }

            //! encore a type
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

