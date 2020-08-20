#include "y/yap/number.hpp"
#include "y/code/round.hpp"
#include "y/os/error.hpp"
namespace upsylon {

    namespace yap {

        number:: ~number() throw()
        {
        }

        number:: number() throw()
        {
        }

        sign_type number:: sign_of(const itype i) throw()
        {
            if(i<0)
            {
                return __negative;
            }
            else if(0<i)
            {
                return __positive;
            }
            else
                return __zero;
        }

        number::utype number::  iabs_of(const itype i) throw()
        {
            return (i<0) ? utype(-i) : utype(i);
        }

        sign_type number:: opposite(const sign_type s) throw()
        {
            switch(s)
            {
                case __negative: return __positive;
                case __zero:     break;
                case __positive: return __negative;
            }
            return __zero;
        }

        sign_type number:: product(const sign_type a, const sign_type b) throw()
        {
            switch(a)
            {
                case __negative:
                    switch(b)
                    {
                        case __negative: return __positive;
                        case __zero:     break;
                        case __positive: return __negative;
                    }
                    break;

                case __zero: break;

                case __positive:
                    switch(b)
                    {
                        case __negative: return __negative;
                        case __zero:     break;
                        case __positive: return __positive;
                    }
                    break;
            }
            return __zero;
        }


    }

}

