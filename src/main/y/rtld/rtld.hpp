
//! \file
#ifndef Y_RTLD_INCLUDED
#define Y_RTLD_INCLUDED 1

#include "y/string.hpp"
#include "y/concurrent/singleton.hpp"

namespace upsylon
{
    class rtld : public singleton<rtld>
    {
    public:
        size_t linked() const throw();
        
        void  *link(const string &soname);
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(rtld);
        Y_SINGLETON_DECL_WITH(0,rtld);
        explicit rtld();
        virtual ~rtld() throw();
    };
}

#endif

