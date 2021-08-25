

#include "y/alchemy/library.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        inline bool isUpper(const char C) throw() { return C>='A' && C <= 'Z'; }
        inline bool isLower(const char C) throw() { return C>='a' && C <= 'z'; }
        inline bool isDecCh(const char C) throw() { return C>='0' && C <= '9'; }
        inline bool isAlpha(const char C) throw() { return isUpper(C) || isLower(C); }
        inline bool isAlNum(const char C) throw() { return isAlpha(C) || isDecCh(C); }
        inline bool isWord_(const char C) throw() { return '_' == C || isAlNum(C);   }

        const Species &Library:: get(const string &id)
        {
            static const char fn[] = "parse";
            // identifying name
            const size_t n = id.size();
            if(n<=0)            throw exception("%s::%s(nil)",CLID,fn);
            if(!isUpper(id[0])) throw exception("%s::%s(invalid first char '%c')",CLID,fn, id[0]);
            size_t i = 1;
            while( (i<n) && isWord_(id[i])) ++i;


            // identifying charge
            size_t nc = 0;
            char   ch = 0;
            while(i<n)
            {
                const char c = id[i];
                switch(c)
                {
                    case '+':
                        if(nc<=0)
                        {
                            ch = c;
                        }
                        else
                        {
                            if(ch!='+') throw exception("%s::%s(invalid '%c' after '+')",CLID,fn,c);
                        }
                        break;
                    case '-':
                        if(nc<=0)
                        {
                            ch = c;
                        }
                        else
                        {
                            if(ch!='-') throw exception("%s::%s(invalid '%c' after '-')",CLID,fn,c);
                        }
                        break;
                    default:
                        throw exception("%s::%s(invalid '%c' for charge)",CLID,fn,c);
                }
                ++nc;
                ++i;
            }

            int z = 0;
            switch(ch)
            {
                case '+': z = static_cast<int>(nc); break;
                case '-': z = -static_cast<int>(nc); break;
                default: break;
            }

            const Species::Pointer *ps = sdb.search(id);
            if(ps)
            {
                if(z!= (**ps).z ) throw exception("%s::%s charge mismatch for '%s'!",CLID,fn,*id);
                return **ps;
            }
            else
            {
                return (*this)(id,z);
            }


        }


        const Species &Library:: get(const char *id)
        {
            const string _(id);
            return get(_);
        }


    }

}

