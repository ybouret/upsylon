#include "y/hashing/mph.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{

    namespace hashing
    {
        minimal_perfect:: ~minimal_perfect() throw()
        {
        }

        minimal_perfect:: minimal_perfect() : minimal_perfect_tree()
        {
        }

        void minimal_perfect::check(const_type h) const
        {
            if(invalid==h) throw exception("minimal perfect hashing: invalid key");
        }
        
        minimal_perfect:: minimal_perfect(const minimal_perfect &other) : minimal_perfect_tree(other)
        {
        }

        minimal_perfect:: minimal_perfect(const char **words, const unsigned count) :
        minimal_perfect_tree()
        {
            assert(words);
            for(unsigned i=0;i<count;++i)
            {
                const char *word = words[i];
                add(word,i);
            }
        }



        


    }

}
