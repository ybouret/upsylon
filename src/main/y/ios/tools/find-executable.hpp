#ifndef Y_FIND_EXE_INCLUDED
#define Y_FIND_EXE_INCLUDED 1

#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! handling exe paths for program look-up
    //
    //__________________________________________________________________________
    class exe_paths : public accessible<const string>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef accessible<const string>  ro_strings; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        explicit exe_paths() throw(); //!< setup
        virtual ~exe_paths() throw(); //!< cleanup
        
        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual size_t        size()                   const throw(); //!< dirs.size()
        virtual const string &operator[](const size_t) const throw(); //!< dirs[]
     
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        bool   add(const string &d);             //!< add: check exists, no multiple
        size_t load(const char *path_env_name);  //!< load paths from env variable
        
        //! detect exe name in registered dirs
        template <typename SEQUENCE, typename FILENAME> inline
        size_t operator()(SEQUENCE &which, const FILENAME &xname) const
        {
            size_t       xtra = 0;
            const size_t ndir = dirs.size();
            for(size_t i=1;i<=ndir;++i)
            {
                const string fn = dirs[i] + xname;
                if(ok(fn))
                {
                    which << fn;
                    ++xtra;
                }
            }
            return xtra;
        }
        
        
        
    private:
        typedef vector<const string,memory::pooled> strings;
        strings dirs;
        static bool ok(const string &);
    };
    
    
}

#endif

