//! file

#ifndef Y_CODE_DECOMP_INCLUDED
#define Y_CODE_DECOMP_INCLUDED 1

#include "y/core/inode.hpp"
#include "y/core/list.hpp"
#include "y/type/args.hpp"
#include "y/object.hpp"

namespace upsylon {

    template <typename T>
    struct decompose {

        Y_DECL_ARGS(T,type);

        class pnode : public object, public inode< pnode >
        {
        public:
            typedef core::list_of_cpp<pnode> list;
            
            //! setup
            inline explicit pnode(param_type X, param_type Y) :
            x(X), y(Y) { assert(x<=y); }

            //! cleanup
            inline virtual ~pnode() throw() {}

            //! x/y value
            const_type x;
            const_type y;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pnode);
        };

        typedef core::list_of_cpp<pnode> plist;

        //! all x<=y, x*y = z
        static inline void build( plist &l, param_type z )
        {
            for(mutable_type i=1;i<=z;++i)
            {
                for(mutable_type j=i+1;j<=z;++j)
                {
                    if(i*j==z) l.push_back( new pnode(i,j) );
                }
            }
        }


    };

}

#endif
