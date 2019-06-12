#include "y/math/signal/curve.hpp"


namespace upsylon
{
    namespace math
    {

        curve:: interface:: ~interface() throw()
        {
        }

        curve:: interface:: interface(const size_t d, const size_t r) throw() :
        dimensions(d),
        real_bytes(r),
        point_size(dimensions*real_bytes),
        computed(false)
        {
            assert(d>0);
            assert(r>0);
        }

        void curve:: interface:: set_computed(bool flag) throw()
        {
            (bool &)computed = flag;
        }

        template <typename T>
        static inline void __print( ios::ostream &fp, const void *p, const size_t d)
        {
            const T *q = static_cast<const T *>(p);
            for(size_t i=0;i<d;++i)
            {
                fp(" %.15g", q[i]);
            }
        }
        
        void curve::interface:: save_point( ios::ostream &fp, const void *p ) const
        {
            assert(p);
            switch (real_bytes)
            {
                case sizeof(float) : __print<float> (fp,p,dimensions); break;
                case sizeof(double): __print<double>(fp,p,dimensions); break;
                default:
                    break;
            }
        }
        
        void curve:: interface:: save( ios::ostream &fp ) const
        {
            const size_t n = size();
            for(size_t i=1;i<=n;++i)
            {
                fp("%u",unsigned(i));
                save_point(fp,addr(i));
                fp << '\n';
            }
        }
        
    }

}

