
#include "y/memory/tight/quarry.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/code/base2.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/self-destruct.hpp"
#include "y/exceptions.hpp"

#include <cstring>
#include <iostream>
#include <cerrno>

namespace upsylon {

    namespace memory {

        namespace tight {

            const unsigned quarry::widths[64] =
            {
                0,0,0,0,2,2,2,3,3,3,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,9,9,9,10,10,10,10,11,11,11,12,12,12,13,13,13,13,14,14,14,15,15,15,16,16,16,16,17,17,17,18,18,18,19,19,19,19
            };

            quarry:: ~quarry() throw()
            {
                if(undersized)
                {
                    std::cerr << "[small::quarry] leak #undersized=" << undersized << std::endl;
                }
                aliasing::_(undersized) = 0;

                for(size_t i=vein::max_exp2;i>=vein::min_exp2;--i)
                {
                    self_destruct(ore[i]);
                }
                ore=0;
                memset(wksp,0,sizeof(wksp));
            }

            quarry:: quarry() throw() :
            ore(0),
            undersized(0),
            wksp()
            {
                memset(wksp,0,sizeof(wksp));
                ore = aliasing::as<vein>(wksp);
                for(size_t i=vein::min_exp2;i<=vein::max_exp2;++i)
                {
                    new (ore+i) vein(i);
                }
            }

            void  * quarry:: pull(const size_t shift)
            {
                if(shift<vein::min_exp2)
                {
                    static global &mgr = global::instance();
                    const  size_t  num = vein::one<<shift;
                    aliasing::_(undersized) += num;
                    return mgr.__calloc(1,num);
                }
                else
                {
                    if(shift>vein::max_exp2) throw libc::exception(ERANGE,"quarry::pull(shift>%lu)", (unsigned long) shift );
                    return ore[shift].acquire();
                }

            }

            void  quarry:: push(void *addr, const size_t shift) throw()
            {
                assert(addr);
                assert(shift<=vein::max_exp2);
                if(shift<vein::min_exp2)
                {
                    static global &mgr = global::location();
                    const  size_t  num = vein::one<<shift;
                    assert(num<=undersized);
                    aliasing::_(undersized) -= num;
                    return mgr.__free(addr,num);
                }
                else
                {
                    return ore[shift].release(addr);
                }
            }

            vein & quarry:: operator[](const size_t shift) throw()
            {
                assert(shift<=vein::max_exp2);
                assert(shift>=vein::min_exp2);
                return ore[shift];
            }

            vein & quarry:: operator()(const size_t chunk_size) throw()
            {
                return ore[ exp2_of(chunk_size) ];
            }


            std::ostream & operator<<(std::ostream &os, const quarry &q)
            {
                os << "<quarry>" << std::endl;
                os << "\t<undersized bytes=" << q.undersized << ">" << std::endl;


                for(size_t shift=vein::min_exp2;shift<=vein::max_exp2;++shift)
                {
                    const vein &s = q.ore[shift];
                    if(s.count)
                    {
                        vein::width = quarry::widths[shift];
                    }
                }

                for(size_t shift=vein::min_exp2;shift<=vein::max_exp2;++shift)
                {
                    const vein &s = q.ore[shift];
                    if(s.count)
                    {
                        os << "\t" << s << std::endl;
                    }
                }

                os << "<quarry/>";
                return os;
            }

            size_t quarry::exp2_of(const size_t chunk_size) throw()
            {
                assert( is_a_power_of_two(chunk_size) );
                assert( chunk_size>=vein::min_size );
                assert( chunk_size<=vein::max_size );
                const size_t chunk_exp2 = integer_log2(chunk_size);
                assert(chunk_exp2>=vein::min_exp2);
                assert(chunk_exp2<=vein::max_exp2);
                return chunk_exp2;
            }

            void quarry:: optimize() throw()
            {
                for(size_t shift=vein::min_exp2;shift<=vein::max_exp2;++shift)
                {
                    vein &v = ore[shift];
                    v.optimize();
                }
            }
        }

    }

}
