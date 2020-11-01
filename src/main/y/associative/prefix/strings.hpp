
//! file
#ifndef Y_PREFIX_STRINGS_INCLUDED
#define Y_PREFIX_STRINGS_INCLUDED 1

#include "y/associative/prefix/depot.hpp"

namespace upsylon {
    
    //__________________________________________________________________________
    //
    //
    //! depot to store strings as keys, no data
    //
    //__________________________________________________________________________
    class prefix_strings : public prefix_depot<char>
    {
    public:
        typedef prefix_depot<char> depot_type;
        
        explicit prefix_strings();
        virtual ~prefix_strings() throw();
        prefix_strings(const prefix_strings &);
        prefix_strings & operator=(const prefix_strings &);
        
        bool insert(const char   *);
        bool insert(const string &);
        bool has(const char   *) const throw();
        bool has(const string &) const throw();
    };
    
}

#endif
