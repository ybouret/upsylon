//! file

#ifndef Y_CODE_DECOMP_INCLUDED
#define Y_CODE_DECOMP_INCLUDED 1

#include "y/type/args.hpp"
#include "y/object.hpp"
#include "y/sort/merge.hpp"
#include "y/comparison.hpp"

namespace upsylon {

    //--------------------------------------------------------------------------
    //
    //! decomposition of one 'integral' type into a product
    //
    //--------------------------------------------------------------------------
    template <typename T>
    struct decompose {
        //______________________________________________________________________
        //
        //! aliases
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);

        //______________________________________________________________________
        //
        //! node of points
        //______________________________________________________________________
        class pnode : public object
        {
        public:

            //! setup
            inline explicit pnode(param_type X, param_type Y) :
			next(0), prev(0),
            x(X), y(Y), d(x<=y?y-x:x-y)
            {}

            //! cleanup
            inline virtual ~pnode() throw() {}

			pnode     *next; //!< for list
			pnode     *prev; //!< for list
            const_type x;    //!< x<=y
            const_type y;    //!< x<=y
            const_type d;    //!< |x-y|

            //! increasing indicator
            static inline int compare(const pnode *lhs, const pnode *rhs, void *)
            {
                return comparison::increasing(lhs->d,rhs->d);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pnode);
        };

        //______________________________________________________________________
        //
        //! list of values
        //______________________________________________________________________
        typedef core::list_of_cpp<pnode> plist;

        //______________________________________________________________________
        //
        //! build all x<=y, x*y = z
        /**
         ranked in "less square" order
         */
        //______________________________________________________________________
        static inline void build(plist &l, param_type z)
        {
            l.release();
            for(mutable_type i=1;i<=z;++i)
            {
                for(mutable_type j=i;j<=z;++j)
                {
                    if(i*j==z) l.push_back( new pnode(i,j) );
                }
            }
            merging<pnode>::sort(l,pnode::compare,NULL);
        }

        //______________________________________________________________________
        //
        //! keep only most square(d) solution after build
        //______________________________________________________________________
        static inline void clear(plist &l) throw()
        {
            while(l.size>1)
            {
                if(l.tail->prev->d<=l.tail->d) delete l.pop_back();
            }
        }

        //______________________________________________________________________
        //
        //! all in one function keep most squared decomposition
        //______________________________________________________________________
        static inline
        void square(mutable_type &x, mutable_type &y, param_type z)
        {
            plist l;
            build(l,z); assert(l.size>=1 || die("bad build"));
            clear(l);   assert(l.size>=1 || die("bad clear"));
            x=l.head->x;
            y=l.head->y;
        }


    };

}

#endif
