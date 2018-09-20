//! \file
#ifndef Y_LZO_INCLUDED
#define Y_LZO_INCLUDED 1

#include "y/string.hpp"

namespace upsylon
{
    //! LZO wrapper
    class miniLZO : public singleton<miniLZO>
    {
    public:
        const char *VersionString() const throw(); //!< lzo_version_string()
        const char *VersionDate() const throw();   //!< lzo_version_date()

        //! compress source
        string Compress( const string &source ) const;

        //! decompress source
        string Decompress( const size_t size, const string &source ) const;
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(miniLZO);
        friend class singleton<miniLZO>;
        explicit miniLZO();         //!< initialize the library
        virtual ~miniLZO() throw(); //!< destructor
    public:
        static const at_exit::longevity life_time = memory::pooled::life_time - 8; //!< life time for singleton
    };
}

#endif

