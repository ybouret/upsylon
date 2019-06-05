#if defined(Y_BSD)
int                 flags      = 0;
static const mode_t unix_flags = (S_IWUSR | S_IRUSR) | (S_IRGRP) | (S_IROTH);
#endif

#if defined(Y_WIN)
DWORD dwDesiredAccess        = 0;
DWORD dwShareMode            = FILE_SHARE_READ | FILE_SHARE_WRITE;
DWORD dwCreationDisposition  = 0;
DWORD dwFlagsAndAttributes   = FILE_ATTRIBUTE_NORMAL;
#endif

/* Setup parameters */
switch( mode )
{

    case readable:
#if defined(Y_BSD)
        flags |= O_RDONLY;
#endif

#if defined(Y_WIN)
        dwDesiredAccess        = GENERIC_READ;
        dwCreationDisposition  = OPEN_EXISTING;
        dwFlagsAndAttributes  |= FILE_FLAG_SEQUENTIAL_SCAN;
#endif

        break;

    case writable:
#if defined(Y_BSD)
        flags |= O_WRONLY | O_CREAT ;
#endif

#if defined(Y_WIN)
        dwDesiredAccess       = GENERIC_WRITE;
        dwCreationDisposition = OPEN_ALWAYS;
        /* dwFlagsAndAttributes |=0; */
#endif

        break;

    case  readable | writable:
#if defined(Y_BSD)
        flags |= O_RDWR | O_CREAT;
#endif

#if defined(Y_WIN)
        dwDesiredAccess       = GENERIC_READ | GENERIC_WRITE;
        dwCreationDisposition = OPEN_ALWAYS;
        dwFlagsAndAttributes  = FILE_FLAG_RANDOM_ACCESS;
#endif

        break;

    case  writable | truncate:
#if defined(Y_BSD)
        flags |= O_WRONLY | O_CREAT | O_TRUNC;
#endif

#if defined(Y_WIN)
        dwDesiredAccess       = GENERIC_WRITE;
        dwCreationDisposition = CREATE_ALWAYS;
        /*dwFlagsAndAttributes |=0;*/
#endif

        break;


    case readable | writable | truncate:
#if defined(Y_BSD)
        flags |= O_RDWR | O_CREAT | O_TRUNC;
#endif

#if defined(Y_WIN)
        dwDesiredAccess       = GENERIC_READ | GENERIC_WRITE;
        dwCreationDisposition = OPEN_ALWAYS  | TRUNCATE_EXISTING;
        dwFlagsAndAttributes  = FILE_FLAG_RANDOM_ACCESS;
#endif

        break;

    default:
#if defined(Y_BSD)
        throw libc::exception( EINVAL, "%s('%s',%u)", fn,*filename,unsigned(mode));
#endif

#if defined(Y_WIN)
        throw win32::exception( ERROR_INVALID_PARAMETER, "%s('%s',%u)", fn,*filename,unsigned(mode) );
#endif


}

/* system calls */
#if defined(Y_BSD)
Y_GIANT_LOCK();
while( (handle = open( *filename, flags, unix_flags) ) == descriptor::invalid() )
{
    const int err = errno;
    switch( err )
    {
        case EINTR:
            break;

        default:
            throw libc::exception( err, "open('%s')", *filename );
    }
}
#endif

#if defined(Y_WIN)
Y_GIANT_LOCK();
handle = CreateFile( TEXT(*filename),
                    dwDesiredAccess,
                    dwShareMode,
                    NULL,
                    dwCreationDisposition,
                    dwFlagsAndAttributes,
                    NULL);
if(  descriptor::invalid() == handle )
{
    throw win32:: exception( ::GetLastError(), "::CreateFile('%s')", *filename );
}
#endif

