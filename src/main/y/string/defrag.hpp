//! \file
#ifndef Y_STRING_DEFRAG_INCLUDED
#define Y_STRING_DEFRAG_INCLUDED 1

#include "y/string.hpp"
#include "y/core/addr-list.hpp"

namespace upsylon
{

    class string_defrag : public releasable
    {
    public:
        typedef core::addr_node<string>   s_node;
        typedef core::addr_list<string>   s_list;
        typedef core::pool_of_cpp<s_node> s_pool;

        explicit string_defrag() throw();
        virtual ~string_defrag() throw();

        void         record( string &s );
        void         remove( string &s ) throw();
        void         free() throw();
        virtual void release() throw();

        size_t       one_cycle() throw(); //!< call a direct cycle upon the recorded strings
        size_t       cycle() throw();     //!< order string, call one_cycle repeatedly

        template <typename ITERATOR>
        size_t cycle_on( ITERATOR it, size_t n )
        {
            free();
            while(n-->0) record( *(it++) );
            return cycle();
        }

        template <typename SEQUENCE>
        size_t cycle_on( SEQUENCE &seq )
        {
            return cycle_on( seq.begin(), seq.size() );
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(string_defrag);
        s_list _list;
        s_pool _pool;

    };
}


#endif

