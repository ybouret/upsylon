//! \file
#ifndef Y_HASHING_IMPH_INCLUDED
#define Y_HASHING_IMPH_INCLUDED 1

#include "y/hashing/mph.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace hashing
    {
        template <typename T>
        class mperf_for
        {
        public:
            Y_DECL_ARGS(T,type);

            const size_t count;

            inline explicit mperf_for( const int offset) throw() : count(offset), mph() {}
            inline virtual ~mperf_for() throw()
            {
                (int &)count = 0;
            }

            //! throw on failure
            mperf_for & operator<<( const_type x )
            {
                mph.insert(&x,sizeof(T),count);
                ++(int&)count;
                return *this;
            }

            //! try to insert
            bool insert( const_type x )
            {
                const int h = mph.find( &x, sizeof(T) );
                if(h<0)
                {
                    (*this) << x;
                    return true;
                }
                else
                {
                    return false;
                }
            }

            //! query any data
            inline int code(const_type x) const throw()
            {
                return mph.find(&x,sizeof(x));
            }

            inline size_t operator[](const_type x) const throw()
            {
                return static_cast<size_t>(mph.hash(&x,sizeof(x)));
            }



        private:
            mperf mph;
        };
    }

}

#endif

