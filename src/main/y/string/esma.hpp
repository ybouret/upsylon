//!\file
#ifndef Y_STRING_ESMA_INCLUDED
#define Y_STRING_ESMA_INCLUDED 1

#include "y/string/basic.hpp"
#include "y/type/releasable.hpp"

namespace upsylon {

    namespace core {

        class esma : public releasable
        {
        public:
            explicit esma() throw();
            virtual ~esma() throw();

            virtual void release() throw();
            void         prepare(const size_t numChars,
                                 const size_t charSize); //!< acquire some bytes

        protected:
            void      *wksp;  //!< for    string
            ptrdiff_t *k;     //!< search indices
            ptrdiff_t  m;     //!< string length

        private:
            Y_DISABLE_COPY_AND_ASSIGN(esma);
            size_t size;
            void bye() throw();
            void hook(const size_t numChars,
                      const size_t kOffset) throw();
        };

    }

    template <typename T>
    class esma : public core::esma
    {
    public:
        inline explicit esma() throw() : core::esma() {}
        inline virtual ~esma() throw() {}

        inline void set(const T      *motif,
                        const size_t  length)
        {
            assert(!(NULL==motif&&length>0));
            prepare(length,sizeof(T));
            memcpy(wksp,motif,length*sizeof(T));
        }

        inline void set( const core::string<T> &s )
        {
            set( *s, s.size() );
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(esma);
    };

}

#endif

