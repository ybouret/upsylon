
#include "y/yap/prime/ratio.hpp"
#include "y/type/aliasing.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
#include <iostream>

namespace upsylon {
    
    namespace yap {
        
        const char prime_ratio:: CLASS_NAME[] = "yapQ";
        
        const char * prime_ratio:: className() const throw()
        {
            return CLASS_NAME;
        }
        
        size_t prime_ratio:: serialize(ios::ostream &fp) const
        {
            const size_t tmp = num.serialize(fp);
            return       tmp + den.serialize(fp);
        }
        
        void prime_ratio:: check() const
        {
            if( den.is_zero() )
                throw libc::exception(EDOM,"%s division by zero", className());
        }
        
        prime_ratio:: prime_ratio() :
        num(),
        den(1)
        {
        }
        
        prime_ratio:: ~prime_ratio() throw()
        {
        }
        
        prime_ratio:: prime_ratio(const prime_ratio &other) :
        number(),
        num(other.num),
        den(other.den)
        {
            
        }
        
        prime_ratio & prime_ratio:: operator=( const prime_ratio &other )
        {
            
            prime_ratio tmp(other);
            xch(tmp);
            return *this;
        }
        
        void prime_ratio:: xch(prime_ratio &q) throw()
        {
            aliasing::_(num).xch( aliasing::_(q.num) );
            aliasing::_(den).xch( aliasing::_(q.den) );
            
        }
        
        std::ostream & operator<<(std::ostream &os, const prime_ratio &q)
        {
            assert(!q.den.is_zero());
            if(q.den.is_one())
            {
                os << q.num;
            }
            else
            {
                os << '(' << q.num << '/' << q.den << ')';
            }
            return os;
        }
        
        void prime_ratio:: update()
        {
            assert(!den.is_zero());
            
            if(num.is_zero())
            {
                while(den.factors.size>1) delete aliasing::_(den.factors).pop_front();
                (void) aliasing::_(den.factors.head)->make_one();
                assert(den.is_one());
            }
            else
            {
                // num > 0
                if( num.is_one() || den.is_one() )
                {
                    return; // already reduced
                }
                else
                {
                    
                    pf_list &src_num = aliasing::_(num.factors);
                    pf_list &src_den = aliasing::_(den.factors);
                    pf_list  garbage;
                    {
                        pf_list tgt_num, tgt_den;
                        while(src_num.size>0&&src_den.size>0)
                        {
                            switch(natural::scmp(src_num.head->p, src_den.head->p))
                            {
                                case __negative: tgt_num.push_back(src_num.pop_front()); break;
                                case __positive: tgt_den.push_back(src_den.pop_front()); break;
                                case __zero: {
                                    const size_t  num_power = src_num.head->n;
                                    const size_t  den_power = src_den.head->n;
                                    prime_factor *keep = src_num.pop_front();
                                    {
                                        prime_factor *kill = src_den.pop_front();
                                        if(keep>kill)
                                        {
                                            cswap(keep,kill);
                                        }
                                        garbage.push_back(kill);
                                    }
                                    if(num_power>den_power)
                                    {
                                        aliasing::_(keep->n) = num_power-den_power;
                                        tgt_num.push_back(keep);
                                    }
                                    else if(den_power>num_power)
                                    {
                                        aliasing::_(keep->n) = den_power-num_power;
                                        tgt_den.push_back(keep);
                                    }
                                    else
                                    {
                                        garbage.push_back(keep);
                                    }
                                } break;
                            }
                        }
                        tgt_num.merge_back(src_num); assert(0==src_num.size);
                        tgt_den.merge_back(src_den); assert(0==src_den.size);
                        src_num.swap_with(tgt_num);
                        src_den.swap_with(tgt_den);
                    }

                    if(src_den.size<=0)
                    {
                        assert(garbage.size>=1);
                        src_den.push_back(garbage.pop_front()->make_one());
                        assert(den.is_one());
                    }
                    
                    if(src_num.size<=0)
                    {
                        assert(garbage.size>=1);
                        src_num.push_back(garbage.pop_front()->make_one());
                        assert(num.is_one());
                    }
                    
                }
            }
            
            
        }
        
        
    }
    
}

