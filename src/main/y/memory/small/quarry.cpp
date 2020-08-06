
#include "y/memory/small/quarry.hpp"
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

        namespace small {

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

                for(size_t i=stones::max_shift;i>=stones::min_shift;--i)
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
                ore = aliasing::as<stones>(wksp);
                for(size_t i=stones::min_shift;i<=stones::max_shift;++i)
                {
                    new (ore+i) stones(i);
                }
            }

            void  * quarry:: pull(const size_t shift)
            {
                if(shift<stones::min_shift)
                {
                    static global &mgr = global::instance();
                    const  size_t  num = stones::one<<shift;
                    aliasing::_(undersized) += num;
                    return mgr.__calloc(1,num);
                }
                else
                {
                    if(shift>stones::max_shift) throw libc::exception(ERANGE,"quarry::pull(shift>%lu)", (unsigned long) shift );
                    return ore[shift].query();
                }

            }

            void  quarry:: push(void *addr, const size_t shift) throw()
            {
                assert(addr);
                assert(shift<=stones::max_shift);
                if(shift<stones::min_shift)
                {
                    static global &mgr = global::location();
                    const  size_t  num = stones::one<<shift;
                    assert(num<=undersized);
                    aliasing::_(undersized) -= num;
                    return mgr.__free(addr,num);
                }
                else
                {
                    return ore[shift].store(addr);
                }
            }

            stones & quarry:: operator[](const size_t shift) throw()
            {
                assert(shift<=stones::max_shift);
                assert(shift>=stones::min_shift);
                return ore[shift];
            }

            stones & quarry:: operator()(const size_t chunk_size) throw()
            {
                return ore[ exp2_of(chunk_size) ];
            }


            std::ostream & operator<<(std::ostream &os, const quarry &q)
            {
                os << "<quarry>" << std::endl;
                os << "\t<undersized bytes=" << q.undersized << ">" << std::endl;
                for(size_t shift=stones::min_shift;shift<=stones::max_shift;++shift)
                {
                    const stones &s = q.ore[shift];
                    if(s.count)
                    {
                        stones::width = quarry::widths[shift];
                    }
                }

                for(size_t shift=stones::min_shift;shift<=stones::max_shift;++shift)
                {
                    const stones &s = q.ore[shift];
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
                assert( chunk_size>=stones::min_bytes );
                assert( chunk_size<=stones::max_bytes );
                const size_t chunk_exp2 = integer_log2(chunk_size);
                assert(chunk_exp2>=stones::min_shift);
                assert(chunk_exp2<=stones::max_bytes);
                return chunk_exp2;
            }
        }

    }

}
