//! \file
#ifndef Y_LZO_INCLUDED
#define Y_LZO_INCLUDED 1

#include "y/string.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"

namespace upsylon
{
    //! LZO wrapper
    class miniLZO : public singleton<miniLZO>
    {
    public:
        const char *VersionString() const throw(); //!< lzo_version_string()
        const char *VersionDate() const throw();   //!< lzo_version_date()

        //! compress block, emit_upack and output
        void Compress( ios::ostream &fp, const void *data, const size_t size);

        //! compress block, emit_upack and output
        inline void Compress(ios::ostream &fp, const memory::ro_buffer &buff)
        {
            Compress(fp, buff.ro(), buff.length());
        }

        //! decompress in a buffer of size outlen
        void Decompress(void       *output, const size_t outlen,
                        const void *input,  const size_t inlen) const;
        
        //! decompress from input
        string Decompress( ios::istream &fp ) const;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(miniLZO);
        friend class singleton<miniLZO>;
        explicit miniLZO();         //!< initialize the library
        virtual ~miniLZO() throw(); //!< destructor
    public:
        static const at_exit::longevity life_time = 0; //!< life time for singleton
    };
}

#endif

