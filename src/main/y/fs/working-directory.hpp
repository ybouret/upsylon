//! \file
#ifndef YOCTO_WORKING_DIRECTORY_INCLUDED
#define YOCTO_WORKING_DIRECTORY_INCLUDED 1

#include "y/string.hpp"
#include "y/concurrent/singleton.hpp"

namespace upsylon
{
    //! changing working directory
    class working_directory : public singleton<working_directory>
    {
    public:
        string get() const; //!< get current directory
        void   set( const string & ) const; //!< set current directory

        //! to change and restore working directory
        class change_lock
        {
        public:
            //! set current directory to target, saving CWD
            change_lock( const string &target, bool *status );
            //! set current directory to target, saving CWD
            change_lock( const char   *target, bool *status );
            ~change_lock() throw();
            
        private:
            const string source_;
            bool        *status_;
            Y_DISABLE_COPY_AND_ASSIGN(change_lock);
        };
        
    private:
        explicit working_directory() throw();
        virtual ~working_directory() throw();
        
        Y_DISABLE_COPY_AND_ASSIGN(working_directory);
        friend class singleton<working_directory>;
        
        static const at_exit::longevity life_time = object::life_time - 9; // TODO: check this value
    };

    //! avoid code deleting...
    typedef volatile working_directory::change_lock chdir_lock;
    
}

#endif
