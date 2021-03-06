//! \file
#ifndef Y_LOCAL_FS_INCLUDED
#define Y_LOCAL_FS_INCLUDED 1

#include "y/fs/vfs.hpp"
#include "y/concurrent/singleton.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! local file system
    //
    //__________________________________________________________________________
    class local_fs : public singleton<local_fs>, public vfs
    {
    public:
        //______________________________________________________________________
        //
        // vfs interface
        //______________________________________________________________________
        virtual entry::attribute query_attribute(const string &path, bool &is_link) const throw();
        virtual void             create_dir(const string &dirname, const bool allow_already_exists);
        virtual void             remove_dir(const string &dirname);
        virtual void             remove_file(const string &filename);
        virtual scanner *        new_scanner(const string &dirname) const;
        virtual uint64_t         get_file_size(const string &path) const;
        virtual void             rename_path(const string&,const string &) const;

        //______________________________________________________________________
        //
        // using PATH
        //______________________________________________________________________
        const accessible<const string> &xpaths() const;                       //!< from impl
        bool                            add_xpath(const string &);            //!< check existing, non multiple
        size_t                          to_xpaths(const char *path_env_name); //!< parse the variable name

        //! detect exe name in registered dirs
        template <typename SEQUENCE, typename FILENAME> inline
        size_t which(SEQUENCE &target, const FILENAME &xname) const
        {
            const accessible<const string> &dirs = xpaths();
            size_t       xtra = 0;
            const size_t ndir = dirs.size();
            for(size_t i=1;i<=ndir;++i)
            {
                string fn = dirs[i] + xname;
                if(check_xpath(fn))
                {
                    target << fn;
                    ++xtra;
                }
            }
            return xtra;
        }


        //! default path, with caching in impl
        const string * query_path_handle(const string &xname) const;

        //! default path, wrapper
        const string * query_path_handle(const char *xname) const;


    private:
        explicit local_fs();
        virtual ~local_fs() throw();
        Y_DISABLE_COPY_AND_ASSIGN(local_fs);
        Y_SINGLETON_DECL(local_fs);
        void *impl;
        bool  check_xpath(string &fn) const;
        

    };

    
}
#endif
