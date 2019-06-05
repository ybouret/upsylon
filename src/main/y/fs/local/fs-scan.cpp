#include "y/fs/local/fs.hpp"
#include "y/exceptions.hpp"

#if defined(Y_BSD)
#include <dirent.h>
#include <cerrno>
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#endif


namespace upsylon
{


    namespace {
        
        class local_scanner : public vfs::scanner
        {
        public:
            
            const char *local_dir() const throw()
            {
                return *folder;
            }
            
#if defined(Y_BSD)
            explicit local_scanner( const string &dirname , const vfs &fs) :
            vfs::scanner( dirname, fs),
            handle_( NULL )
            {
                Y_GIANT_LOCK();
                handle_ = opendir( local_dir() );
                if( NULL == handle_ )
                {
                    throw libc::exception( errno, "opendir(%s)", local_dir() );
                }
            }
            
            virtual const vfs::entry *next(void) {
                Y_GIANT_LOCK();
                errno = 0;
                struct dirent *ep = readdir( handle_ );
                if( !ep )
                {
                    const int err = errno;
                    if( err == 0)
                        return NULL;
                    throw libc::exception( errno, "readdir(%s)", local_dir() );
                }
                else
                {
                    return make_entry( ep->d_name );
                }
            }
#endif
            
#if defined(Y_WIN)
            explicit local_scanner( const string &dirname , const vfs &fs) :
            vfs::scanner( dirname, fs),
            hFind_( INVALID_HANDLE_VALUE ),
            hData_(),
            valid_( true )
            {
                
                const string args = folder + '*';
                Y_GIANT_LOCK();
                hFind_ = ::FindFirstFile( &args[0], &hData_ );
                
                if( hFind_ == INVALID_HANDLE_VALUE )
                {
                    throw win32::exception( ::GetLastError(), "::FindFirstFile(%s)", local_dir() );
                }
                assert( NULL != hData_.cFileName );
            }
            
            virtual const vfs::entry *next(void)
            {
                if(valid_ )
                {
                    //std::cerr << "[WIN32] has next entry '" << hData_.cFileName << "'" << std::endl;
                    const vfs::entry *ep = make_entry( hData_.cFileName );
                    Y_GIANT_LOCK();
                    if( ! ::FindNextFile( hFind_, &hData_ ) )
                    {
                        const DWORD err = ::GetLastError();
                        valid_ = false;
                        
                        if( err != ERROR_NO_MORE_FILES )
                        {
                            throw win32::exception( err, "::FindNextFile(%s)",local_dir());
                        }
                    }
                    return ep;
                }
                else
                {
                    //std::cerr << "[WIN32] NO  next entry." << std::endl;
                    return 0;
                }
            }
#endif
            
            virtual ~local_scanner() throw()
            {
#if defined(Y_BSD)
                closedir( handle_ );
#endif
                
#if defined(Y_WIN)
                //std::cerr << "[WIN32] Closing handle to '" << dir_ << "'" << std::endl;
                ::FindClose( hFind_ );
#endif
            }
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(local_scanner);
            
#if defined(Y_BSD)
            DIR *handle_;
#endif
            
#if defined(Y_WIN)
            HANDLE           hFind_;
            WIN32_FIND_DATA  hData_;
            bool             valid_;
#endif
        };
    }
    
    vfs::scanner *local_fs:: new_scanner( const string &dirname ) const
    {
        return new local_scanner( dirname, *this );
    }
    
}
