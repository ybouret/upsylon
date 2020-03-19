//! \file

#ifndef Y_JARGON_PATTERN_DICT_INCLUDED
#define Y_JARGON_PATTERN_DICT_INCLUDED 1

#include "y/jargon/pattern.hpp"
#include "y/associative/suffix-tree.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! base class for dictionary
        typedef suffix_tree<Motif> DictionaryType;
        
        //! dictionary of named patterns
        class Dictionary : public DictionaryType
        {
        public:
            explicit Dictionary();          //!< setup
            virtual ~Dictionary() throw();  //!< cleanup
            Dictionary(const Dictionary &); //!< copy
            
            
            bool insert(const string &name, const Pattern *p);     //!< insert a named fixed pattern
            bool insert(const char   *name, const Pattern *p);     //!< insert a named fixed pattern
            const Motif *search(const string &name) const throw(); //!< search a pattern
            const Motif *search(const char   *name) const throw(); //!< seatch a pattern
            
            const Motif & operator[](const string &name) const;    //!< throw if not found
            const Motif & operator[](const char   *name) const;    //!< throw if not found

            
        private:
            Y_DISABLE_ASSIGN(Dictionary);
        };
        
    }
    
}

#endif

