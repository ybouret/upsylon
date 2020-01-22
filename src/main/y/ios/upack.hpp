//! \file
#ifndef Y_IOS_UPACK_INCLUDED
#define Y_IOS_UPACK_INCLUDED 1

#include "y/memory/buffer.hpp"
#include "y/type/args.hpp"
#include "y/code/round.hpp"

namespace upsylon
{

    namespace ios
    {
        //! encoding prototype arguments
#define Y_IOS_UPACK_ENCODE_PROTO(N) void *p, const size_t n, const void *addr, int2type<N>
        //! decoding prototype arguments
#define Y_IOS_UPACK_DECODE_PROTO(N) const void *p, void *addr, int2type<N>
        //! declaration of the different word sizes
#define Y_IOS_UPACK_DECL(N) void encode_(Y_IOS_UPACK_ENCODE_PROTO(N)) throw(); void decode_(Y_IOS_UPACK_DECODE_PROTO(N)) const

        //! unsigned type pack/unpack base class
        class upack_ : public memory::rw_buffer
        {
        public:
            virtual ~upack_() throw(); //!< cleanup
            const size_t size;         //!< currently encoded bytes

        protected:
            explicit upack_() throw(); //!< setup
            //! fix size and clear space
            void     clear(const size_t sz,void *p,const size_t n) throw();

            Y_IOS_UPACK_DECL(1); //!< encode/decode 1 byte  unsigned
            Y_IOS_UPACK_DECL(2); //!< encode/decode 2 bytes unsigned
            Y_IOS_UPACK_DECL(4); //!< encode/decode 4 bytes unsigned
            Y_IOS_UPACK_DECL(8); //!< encode/decode 8 bytes unsigned

        private:
            Y_DISABLE_COPY_AND_ASSIGN(upack_);

        };

        //! encode/decode for an integral type I/O
        template <typename T>
        class upack  : public upack_
        {
        public:
            Y_DECL_ARGS(T,type);                                  //!< aliases
            static const size_t word_size = sizeof(type);         //!< base size
            static const size_t requested = word_size+1;          //!< maximum encoded size
            static const size_t workspace = Y_ROUND4(requested);  //!< internal workspace

            //! cleanup
            inline virtual ~upack() throw()            { clear(0,        data,requested); }

            //! setup clean, ready data
            inline explicit upack() throw() : upack_() { clear(requested,data,workspace); }

            //! return data
            inline virtual const void *ro()     const throw() { return data; }

            //! return current size
            inline virtual size_t      length() const throw() { return size; }

            //! encode data
            inline void encode(const_type value) throw()
            {
                encode_(data,requested,&value,int2type<word_size>());
            }

            //! decode integral type from current content and size
            template <typename U>
            inline U decode() const
            {
                U ans(0);
                decode_(data,&ans,int2type<sizeof(U)>());
                return ans;
            }

            //! reset clean, ready data, to be filled by I/O (a.k.a MPI...)
            inline void reset() throw() { clear(requested,data,requested); }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(upack);
            char data[ workspace ];
        };

        //! base class to encode portable sizes
        typedef upack<uint64_t> upack_size_type;

        //! encode sizes
        class upack_size : public upack_size_type
        {
        public:
            explicit upack_size() throw(); //!< setup
            virtual ~upack_size() throw(); //!< cleanup

            void   operator()(const size_t count) throw(); //!< count->(bytes,size)
            size_t operator()(void)   const;               //!< (bytes,size)->count

            upack_size & get() throw(); //!< reset and return *this;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(upack_size);
        };
    }

}

#endif

