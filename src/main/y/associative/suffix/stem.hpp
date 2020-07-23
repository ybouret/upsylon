//! \file
#ifndef Y_FILE_ASSOCIATIVE_SUFFIX_STEM_INCLUDED
#define Y_FILE_ASSOCIATIVE_SUFFIX_STEM_INCLUDED 1

#include "y/ios/serializable.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! base class for suffix-(x)store
    //
    //__________________________________________________________________________
    class suffix_stem : public ios::serializable
    {
    public:
        //______________________________________________________________________
        //
        // virtual interface
        //______________________________________________________________________
        virtual       ~suffix_stem() throw();          //!< cleanup
        virtual void   free()             throw() = 0; //!< reset, keep memory
        virtual void   free_cache()       throw() = 0; //!< remove excess memory
        virtual void   grow_cache(size_t)         = 0; //!< expand excess memory
        virtual size_t cache_size() const throw() = 0; //!< return size of cache


        //______________________________________________________________________
        //
        // non-virtual interface
        //______________________________________________________________________
        void   release()  throw();       //!< free and free_cache
        size_t required() const throw(); //!< nodes+cache.size-1 so far

        //______________________________________________________________________
        //
        // common data
        //______________________________________________________________________
        const   size_t  nodes;   //!< number of nodes into tree, >=1
        mutable size_t  created; //!< cumulative number of created nodes, mostly for debug
        const   size_t  entries; //!< current number of entries

    protected:
        explicit suffix_stem() throw(); //!< setup

        static const char used[];     //!< "used"
        static const char code[];     //!< "code"
        static const char branches[]; //!< "branches"

        //! exception raising for loading
        void throw_missing(const char *field) const;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_stem);
    };

}

#endif
