
#ifndef Y_SUFFIX_DATA_INCLUDED
#define Y_SUFFIX_DATA_INCLUDED 1

#include "y/suffix/node.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{

    template <typename CODE,typename T>
    class suffix_data
    {
    public:
        typedef suffix_node<CODE> node_type;
        
        suffix_data *next;
        suffix_data *prev;
        node_type   *hook;
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_data);
    };
    
    
}


#endif
