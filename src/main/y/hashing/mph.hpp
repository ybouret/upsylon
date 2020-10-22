//! \file
#ifndef Y_HASHING_MPH_INCLUDED
#define Y_HASHING_MPH_INCLUDED 1

#include "y/associative/affix/tree.hpp"

namespace upsylon
{

    namespace hashing
    {
        typedef affix_tree<unsigned> minimal_perfect_tree;

        //! minimal perfect hashing
        class minimal_perfect : public minimal_perfect_tree
        {
        public:
            static const_type invalid = limit_of<const_type>::maximum;
            explicit minimal_perfect();
            virtual ~minimal_perfect() throw();
            minimal_perfect(const minimal_perfect &);
            minimal_perfect(const char **words, const unsigned count);

            template <typename ID> inline
            bool add(const ID &key, const_type h)
            {
                check(h);
                return insert_by(key,h);
            }

            template <typename ID> inline
            type operator()(const ID &key)
            {
                const_type *ptr = search_by(key);
                if(ptr)
                {
                    assert(invalid!=*ptr);
                    return *ptr;
                }
                else
                {
                    return invalid;
                }
            }



        private:
            Y_DISABLE_ASSIGN(minimal_perfect);
            void check(const_type) const;
        };

#define  Y_MPH_WORDS(DATA) DATA, sizeof(DATA)/sizeof(DATA[0])


    }


}

#endif
