#include "y/alchemy/equilibrium.hpp"
#include "y/alchemy/library.hpp"
#include "y/exception.hpp"
#include <cstdlib>

namespace upsylon {

    namespace Alchemy
    {

#if 0
        static inline bool isInt(const char C)   throw() { return C>='0' && C <= '9'; }

        static inline bool isSpc(const char C)   throw() { return C == ' ' || C == '\t'; }


        void Equilibrium:: parse(const string &info, Library &lib)
        {
            static const char fn[] = ".parse";
            const size_t n = info.size();
            if(n<=0) throw exception("%s%s empty info",*name,fn);

            string coef;
            string data;

            // get num
            size_t i=0;
            if(info[0]=='-')
            {
                ++i;
                coef << '-';
            }
            while(i<n&&isInt(info[i])) coef << info[i++];

            
            // skip blanks
            while(i<n&&isSpc(info[i])) ++i;

            // get data
            while(i<n) data << info[i++];

            if(data.size()<=0) throw exception("%s%s no species in '%s'",*name,fn,*info);
            
            // parse num
            long   nu    = 0;
            switch(coef.size())
            {
                case 0: nu=1; break;
                case 1:
                    switch(coef[0])
                    {
                        case '-': nu=-1; break;
                        default :
                            assert( isInt(coef[0]) );
                            nu = long(coef[0])-long('0');
                            break;
                    }
                    break;
                    
                default:
                    nu = atol( *coef );
                    break;
            }
            
            if(0==nu) throw exception("%s%s nu=0 for '%s'",*name,fn,*data);
            
            // create actor
            Equilibrium &self = *this;
            self(nu,lib.get(data));
        }
#endif
        
    }
}
