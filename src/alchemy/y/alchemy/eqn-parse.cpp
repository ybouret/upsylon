#include "y/alchemy/equilibrium.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Alchemy
    {

        static inline bool isInt(const char C)   throw() { return C>='0' && C <= '9'; }

        static inline bool isSpc(const char C)   throw() { return C == ' ' || C == '\t'; }


        void Equilibrium:: parse(const string &info, Library &lib)
        {
            static const char fn[] = ".parse ";
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

            std::cerr << "coef=(" << coef << ")" << std::endl;

            while(i<n&&isSpc(info[i])) ++i;

            while(i<n) data << info[i++];

            std::cerr << "data=(" << data << ")" << std::endl;

            // parse num
            long   nu    = 0;
            {
                bool        ng = false;
                const char *ch = *coef;
                size_t      nc = coef.size();
                
            }

        }

    }
}
