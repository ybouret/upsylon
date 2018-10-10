//! \file
#ifndef Y_DYNAMO_HASH31_INCLUDED
#define Y_DYNAMO_HASH32_INCLUDED 1

#include "y/hashing/function.hpp"
#include "y/lang/types.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/set.hpp"

namespace  upsylon
{

    namespace Lang
    {

        class DynamoHasher
        {
        public:
            const string id;

            virtual ~DynamoHasher() throw(); //!< destructor
            const string & key() const throw(); //!< id

            //! get a signed value on 31 bits
            inline int32_t  operator()(const string &s) throw()
            {
                const uint32_t u = hfn().key<uint32_t>(s);
                const int32_t  h = int32_t(u&0x7fffffff); assert(h>=0);
                return h;
            }

        protected:
            explicit DynamoHasher(const char *);

        private:
            virtual hashing::function & hfn() throw() = 0;
        };

        //! hasher for integer values
        template <typename HASH_FUNCTION>
        class DynamoHasherWith : public HASH_FUNCTION, public DynamoHasher
        {
        public:

            inline explicit DynamoHasherWith() : HASH_FUNCTION(), DynamoHasher( this->name() ) {}
            inline virtual ~DynamoHasherWith() throw()  {}      //!< destructor
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoHasherWith);
            inline virtual hashing::function & hfn() throw() { return *this; }
        };


    }
}

#endif
