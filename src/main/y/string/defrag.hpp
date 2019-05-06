//! \file
#ifndef Y_STRING_DEFRAG_INCLUDED
#define Y_STRING_DEFRAG_INCLUDED 1

#include "y/string.hpp"
#include "y/core/addr-list.hpp"
#include "y/core/pool.hpp"

namespace upsylon
{

    //! strings defragmenter
    class string_defrag : public releasable
    {
    public:
        typedef core::addr_node<string>   s_node; //!< alias
        typedef core::addr_list<string>   s_list; //!< alias
        typedef core::pool_of_cpp<s_node> s_pool; //!< alias

        explicit string_defrag() throw(); //!< setup
        virtual ~string_defrag() throw(); //!< cleanu

        void         record( string &s );         //!< record a string to be defragged
        void         remove( string &s ) throw(); //!< remove a string
        void         free() throw();              //!< drop all the work to do
        virtual void release() throw();           //!< free() and release mempry

        size_t       one_cycle() throw(); //!< call a direct cycle upon the recorded strings
        size_t       cycle() throw();     //!< order string, call one_cycle repeatedly

        //! free(), record all n strings, cycle()
        template <typename ITERATOR>
        size_t cycle_on( ITERATOR it, size_t n )
        {
            free();
            while(n-->0) record( *(it++) );
            return cycle();
        }

        //! free(), record all sequence, cycle()
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

