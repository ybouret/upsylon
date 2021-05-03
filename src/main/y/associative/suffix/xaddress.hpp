
//! \file

#ifndef Y_ASSOCIATIVE_SUFFIX_XADDRESS_INCLUDED
#define Y_ASSOCIATIVE_SUFFIX_XADDRESS_INCLUDED

#include "y/memory/buffer.hpp"
#include "y/type/args.hpp"
#include "y/code/round.hpp"
#include "y/type/ints.hpp"
#include <cstring>

namespace upsylon
{

    namespace core
    {
        //! common stuff for xaddr
        struct suffix_xaddr {
            static const void *be(const void *) throw();                 //!< big endian address
            static void        be(void *, const void *, size_t) throw(); //!< big endian copy
        };
    }
    //__________________________________________________________________________
    //
    //
    //! using unique addresses+data as optimized keys
    //
    //__________________________________________________________________________
    template <typename T>
    class suffix_xaddress : public memory::ro_buffer
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                                       //!< aliases
        typedef typename unsigned_int<sizeof(void*)>::type word_t; //!< alias

        static const size_t addr_size = sizeof(void *);                     //!< for void *
        static const size_t type_size = sizeof(type);                       //!< for type
        static const size_t data_size = addr_size + type_size;              //!< sum of bytes
        static const size_t num_bytes = Y_ALIGN_FOR_ITEM(word_t,data_size); //!< aligned
        static const size_t num_words = num_bytes/sizeof(word_t);           //!< local words

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~suffix_xaddress() throw() { memset(wksp,0,sizeof(wksp)); }

        //! copy data
        inline suffix_xaddress(const suffix_xaddress &other) throw() : wksp()
        {
            memcpy(wksp,other.wksp,sizeof(wksp));
        }

        //! construct
        template <typename U> inline
        explicit suffix_xaddress(const U &obj, param_type pod) throw() : wksp()
        {
            const void *ptr = core::suffix_xaddr::be(&obj);
            memcpy                (&wksp[0],&ptr,addr_size);
            core::suffix_xaddr::be(&wksp[1],&pod,type_size);
        }
        

        //______________________________________________________________________
        //
        // buffer interface
        //______________________________________________________________________
        inline virtual size_t      length() const throw() { return data_size; }
        inline virtual const void *ro()     const throw() { return wksp;      }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        const void         *addr()   const throw(); //!< retrieve original address

    private:
        Y_DISABLE_ASSIGN(suffix_xaddress);
        word_t wksp[num_words];

    };
}

#endif
