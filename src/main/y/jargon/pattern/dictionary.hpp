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
            
            bool insert(const string &name, const Pattern *p);
            bool insert(const char   *name, const Pattern *p);
            const Motif *search(const string &name) const throw();
            const Motif *search(const char   *name) const throw();
            
            const Motif & operator[](const string &name) const;
            const Motif & operator[](const char   *name) const;

            
        private:
            Y_DISABLE_ASSIGN(Dictionary);
        };
        
    }
    
}

#endif

