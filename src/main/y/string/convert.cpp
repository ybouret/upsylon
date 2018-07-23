#include "y/string/convert.hpp"
#include "y/exceptions.hpp"
#include "y/code/utils.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{

    static const char default_name[] = "variable";

    ////////////////////////////////////////////////////////////////////////////
    //
    // parse decimal unsigned
    //
    ////////////////////////////////////////////////////////////////////////////
    static inline size_t __to_size_dec( const string &data, const char *id)
    {
        static const char fn[] = "string_convert::to<size_t>/dec: ";
        assert(id);
        size_t       ans = 0;
        const size_t n   = data.size();
        for(size_t i=0;i<n;++i)
        {
            const char C = data[i];
            switch(C)
            {
                case '0': ans *= 10;         break;
                case '1': ans *= 10; ++ans;  break;
                case '2': ans *= 10; ans+=2; break;
                case '3': ans *= 10; ans+=3; break;
                case '4': ans *= 10; ans+=4; break;
                case '5': ans *= 10; ans+=5; break;
                case '6': ans *= 10; ans+=6; break;
                case '7': ans *= 10; ans+=7; break;
                case '8': ans *= 10; ans+=8; break;
                case '9': ans *= 10; ans+=9; break;

                default:
                    throw exception("%sinvalid decimal '%s' for %s",fn, visible_char[ uint8_t(C) ], id);
            }
        }
        return ans;
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // parse hexadecimal unsigned
    //
    ////////////////////////////////////////////////////////////////////////////
    static inline size_t __to_size_hex( const string &data, const char *id)
    {
        static const char fn[] = "string_convert::to<size_t>/hex: ";
        assert(id);
        size_t       ans = 0;
        const size_t n   = data.size();
        for(size_t i=2;i<n;++i)
        {
            const char C = data[i];
            switch(C)
            {
                case '0': ans <<= 4;         break;
                case '1': ans <<= 4; ++ans;  break;
                case '2': ans <<= 4; ans+=2; break;
                case '3': ans <<= 4; ans+=3; break;
                case '4': ans <<= 4; ans+=4; break;
                case '5': ans <<= 4; ans+=5; break;
                case '6': ans <<= 4; ans+=6; break;
                case '7': ans <<= 4; ans+=7; break;
                case '8': ans <<= 4; ans+=8; break;
                case '9': ans <<= 4; ans+=9; break;
                case 'a':
                case 'A':
                    ans <<= 4; ans += 10; break;
                case 'b':
                case 'B':
                    ans <<= 4; ans += 11; break;
                case 'c':
                case 'C':
                    ans <<= 4; ans += 12; break;
                case 'd':
                case 'D':
                    ans <<= 4; ans += 13; break;
                case 'e':
                case 'E':
                    ans <<= 4; ans += 14; break;
                case 'f':
                case 'F':
                    ans <<= 4; ans += 15; break;
                default:
                    throw exception("%sinvalid hexadecimal '%s' for %s",fn, visible_char[ uint8_t(C) ], id);
            }
        }
        return ans;
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // parse [hexa]decimal unsigned
    //
    ////////////////////////////////////////////////////////////////////////////
    template <>
    size_t string_convert::to<size_t>(const string &data, const char *name)
    {
        static const char fn[] = "string_convert::to<size_t>: ";
        const char       *id   = name ? name : default_name;
        const size_t      n    = data.size();
        if(n<=0)
        {
            throw exception("%sempty %s string",fn,id);
        }

        if(n>2&&data[0]=='0'&&data[1]=='x')
        {
            return __to_size_hex(data,id);
        }
        else
        {
            return __to_size_dec(data,id);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // parse decimal signed
    //
    ////////////////////////////////////////////////////////////////////////////
    template <>
    unit_t string_convert::to<unit_t>(const string &data, const char *name)
    {
        static const char fn[] = "string_convert::to<unit_t>: ";
        const char       *id   = name ? name : default_name;
        const size_t      n    = data.size();
        size_t            i    = 0;
        unit_t            s    = 1;
        if(n<=0)
        {
            throw exception("%sempty %s string",fn,id);
        }
        if(n>1)
        {
            switch(data[0])
            {
                case '-': s=-1;  /* FALLTHRU*/     // change sign
                case '+': ++i; break; // skip
                default: break;
            }
        }
        unit_t ans = 0;
        for(;i<n;++i)
        {
            const char C = data[i];
            switch(C)
            {
                case '0': ans *= 10;         break;
                case '1': ans *= 10; ++ans;  break;
                case '2': ans *= 10; ans+=2; break;
                case '3': ans *= 10; ans+=3; break;
                case '4': ans *= 10; ans+=4; break;
                case '5': ans *= 10; ans+=5; break;
                case '6': ans *= 10; ans+=6; break;
                case '7': ans *= 10; ans+=7; break;
                case '8': ans *= 10; ans+=8; break;
                case '9': ans *= 10; ans+=9; break;

                default:
                    throw exception("%sinvalid decimal '%s' for %s",fn, visible_char[ uint8_t(C) ], id);
            }
        }
        return ans*s;
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // parse floating point
    //
    ////////////////////////////////////////////////////////////////////////////
    template <typename T>
    static inline T __to_real(const string &data, const char *fn, const char *id )
    {
        static const T ten(10);
        static const T tenth(0.1);

        assert(fn);
        assert(id);
        const size_t n = data.size();
        if(n<=0) throw exception("%sempty %s string",fn,id);
        size_t       i=0;

        //______________________________________________________________________
        //
        // detect sign
        //______________________________________________________________________
        T s = 1;
        if(n>1)
        {
            switch(data[0])
            {
                case '-': s=-1;  /* FALLTHRU*/ // change sign
                case '+': ++i; break;          // and/or skip
                default: break;
            }
        }

        //______________________________________________________________________
        //
        // parse integral part
        //______________________________________________________________________
        T      ip       = 0;
        for(;i<n;++i)
        {
            const char C = data[i];
            switch(C)
            {
                case '0': ip *= ten;         break;
                case '1': ip *= ten; ip+=1; break;
                case '2': ip *= ten; ip+=2; break;
                case '3': ip *= ten; ip+=3; break;
                case '4': ip *= ten; ip+=4; break;
                case '5': ip *= ten; ip+=5; break;
                case '6': ip *= ten; ip+=6; break;
                case '7': ip *= ten; ip+=7; break;
                case '8': ip *= ten; ip+=8; break;
                case '9': ip *= ten; ip+=9; break;

                case '.':
                case ',':
                    ++i;
                    goto PARSE_FP;

                case 'e':
                case 'E':
                    ++i; // skip [eE]
                    goto PARSE_EXP;
                default:
                    throw exception("%sinvalid decimal '%s' for %s integer part",fn, visible_char[ uint8_t(C) ], id);
            }
        }
        return s*ip;
    PARSE_FP:
        {
            T fp = tenth;
            for(;i<n;++i)
            {
                const char C = data[i];
                switch(C)
                {
                    case '0': fp *= tenth;  break;
                    case '1': ip += fp;    fp *= tenth;   break;
                    case '2': ip += fp+fp; fp *= tenth;   break;
                    case '3': ip += 3*fp;  fp *= tenth;   break;
                    case '4': ip += 4*fp;  fp *= tenth;   break;
                    case '5': ip += 5*fp;  fp *= tenth;   break;
                    case '6': ip += 6*fp;  fp *= tenth;   break;
                    case '7': ip += 7*fp;  fp *= tenth;   break;
                    case '8': ip += 8*fp;  fp *= tenth;   break;
                    case '9': ip += 9*fp;  fp *= tenth;   break;

                    case 'e':
                    case 'E':
                        ++i; // skip [eE]
                        goto PARSE_EXP;
                        
                    default:
                        throw exception("%sinvalid decimal '%s' for %s fractional part",fn, visible_char[ uint8_t(C) ], id);
                }
            }
        }
        return s*ip;
    PARSE_EXP:
        {
            if(i>=n) throw exception("%smissing exponent value for %s",fn,id);
            // detect sign
            int exp_s = 1;
            switch(data[i])
            {
                case '-': exp_s=-1;  /* FALLTHRU*/ // change sign
                case '+': ++i;
                    if(i>=n) throw exception("%smissing exponent value after sign for %s",fn,id);
                    break;              // and/or skip
                default: break;
            }
            size_t exp_v = 0;
            for(;i<n;++i)
            {
                const char C = data[i];
                switch(C)
                {
                    case '0': exp_v *= 10; break;
                    case '1': exp_v *= 10; exp_v += 1; break;
                    case '2': exp_v *= 10; exp_v += 2; break;
                    case '3': exp_v *= 10; exp_v += 3; break;
                    case '4': exp_v *= 10; exp_v += 4; break;
                    case '5': exp_v *= 10; exp_v += 5; break;
                    case '6': exp_v *= 10; exp_v += 6; break;
                    case '7': exp_v *= 10; exp_v += 7; break;
                    case '8': exp_v *= 10; exp_v += 8; break;
                    case '9': exp_v *= 10; exp_v += 9; break;
                    default:
                        throw exception("%sinvalid decimal '%s' for %s exponent",fn, visible_char[ uint8_t(C) ], id);
                }
            }
            if(exp_s>0)
            {
                ip *= ipower(ten,exp_v);
            }
            else
            {
                ip *= ipower(tenth,exp_v);
            }
        }
        return s*ip;
    }

    template <>
    double string_convert::to<double>(const string &data, const char *name)
    {
        return __to_real<double>(data,"string_convert::to<double>: ",name?name:default_name);
    }

    template <>
    float string_convert::to<float>(const string &data, const char *name)
    {
        return __to_real<double>(data,"string_convert::to<float>: ",name?name:default_name);
    }

}
