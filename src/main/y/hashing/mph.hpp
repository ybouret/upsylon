//! \file
#ifndef Y_HASHING_MPH_INCLUDED
#define Y_HASHING_MPH_INCLUDED 1

#include "y/associative/affix/tree.hpp"

namespace upsylon
{

    namespace hashing
    {
        //! base class
        typedef affix_tree<unsigned> minimal_perfect_tree;

        //______________________________________________________________________
        //
        //
        //! minimal perfect hashing
        /**
         based on an affix_tree
         */
        //
        //______________________________________________________________________
        class minimal_perfect : public minimal_perfect_tree
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const_type invalid = limit_of<const_type>::maximum; //!< unallowed key

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit minimal_perfect();                                //!< setup empty
            virtual ~minimal_perfect() throw();                        //!< cleanup
            minimal_perfect(const minimal_perfect &);                  //!< copy
            minimal_perfect(const char **words, const unsigned count); //!< build from C-strings


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! a new key/hash
            template <typename ID> inline
            void operator()(const ID &key, const_type h)
            {
                check(h);
                tree_mark mark=0;
                if(!insert_by(key,h,&mark))
                {
                    found(mark);
                }
            }

            //! get hash, invalid if not found
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

            //! emit defines
            void emit( ios::ostream &fp ) const;


        private:
            Y_DISABLE_ASSIGN(minimal_perfect);
            void check(const_type) const;
            void found(const tree_node *node) const;
        };

        //! macro for static arrays of strings
#define  Y_MPH_WORDS(DATA) DATA, sizeof(DATA)/sizeof(DATA[0])


    }


}

#endif
