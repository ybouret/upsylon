#ifndef YOCTO_WORKING_DIRECTORY_INCLUDED
#define YOCTO_WORKING_DIRECTORY_INCLUDED 1

#include "y/string.hpp"
#include "y/concurrent/singleton.hpp"

namespace upsylon
{
    class working_directory : public singleton<working_directory>
    {
    public:
        string get() const;
        void   set( const string & ) const;
        
        class change_lock
        {
        public:
            change_lock( const string &target, bool *status );
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
    
    typedef volatile working_directory::change_lock chdir_lock;
    
}

#endif
