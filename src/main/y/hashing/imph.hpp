//! \file
#ifndef Y_HASHING_IMPH_INCLUDED
#define Y_HASHING_IMPH_INCLUDED 1

#include "y/hashing/mph.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace hashing
    {
        //! minimal perfect hashing for integral types
        /**
         this is a way to get a fast indexing of scattered integral data
         */
        template <typename T>
        class mperf_for
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases

            //! setup, offset=0 for C, offset=1 for C++ for example
            inline explicit mperf_for( const int offset) throw() : counter(offset), mph() {}

            //! cleanup
            inline virtual ~mperf_for() throw()
            {
                counter = 0;
            }

            //! throw on failure
            mperf_for & operator<<( const_type x )
            {
                mph.insert(&x,sizeof(T),counter);
                ++counter;
                return *this;
            }

            //! try to insert, discard of multiple
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

            //! query any data, no throw
            inline int code(const_type x) const throw()
            {
                return mph.find(&x,sizeof(x));
            }

            //! access the index of a registered integral type
            inline size_t operator[](const_type x) const throw()
            {
                return static_cast<size_t>(mph.hash(&x,sizeof(x)));
            }

            //! number of entries
            inline size_t size() const throw() { return mph.entries; }

            //! render a graphix tree
            void graphViz(const string &fn) const { mph.graphViz(fn); }

            //! render a graphviz tree
            void graphViz(const char   *fn) const { mph.graphViz(fn); }

        private:
            size_t counter;
            mperf  mph;
        };
    }

}

#endif

