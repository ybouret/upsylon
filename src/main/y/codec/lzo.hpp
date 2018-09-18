//! \file
#ifndef Y_LZO_INCLUDED
#define Y_LZO_INCLUDED 1

#include "y/string.hpp"

namespace upsylon
{
    class miniLZO : public singleton<miniLZO>
    {
    public:
        string Compress( const string &source ) const;
        const char *VersionString() const throw();
        const char *VersionDate() const throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(miniLZO);
        friend class singleton<miniLZO>;
        explicit miniLZO();         //!< initialize the library
        virtual ~miniLZO() throw(); //!< destructor
    public:
        static const at_exit::longevity life_time = memory::pooled::life_time - 8;
    };
}

#endif

