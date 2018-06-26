//! \file exceptions.hpp

#ifndef Y_EXCEPTIONS_INCLUDED
#define Y_EXCEPTIONS_INCLUDED 1

#include "y/exception.hpp"

namespace upsylon
{

	namespace libc
    {

        //! wrapper for libc errors
		class exception : public upsylon::exception
		{
		public:
			explicit exception( int err, const char *fmt, ... ) throw() Y_PRINTF_CHECK(3,4);
			virtual ~exception() throw();
			exception( const exception & ) throw();

			virtual const char *what() const throw();
			int                 code() const throw();

		private:
			Y_DISABLE_ASSIGN(exception);
			const int code_;
			char      what_[128];
		};

	}

	namespace win32 {

		//! wrapper for win32 errors
		class exception : public upsylon::exception
		{
		public:
			explicit exception( uint32_t err, const char *fmt, ... ) throw() Y_PRINTF_CHECK(3,4);
			virtual ~exception() throw();
			exception( const exception & ) throw();

			virtual const char *what() const throw();
			uint32_t            code() const throw();

		private:
			Y_DISABLE_ASSIGN(exception);
			const uint32_t code_;
			char           what_[128];
		};
	}

#if defined(Y_WIN)
	typedef win32::exception os_exception;
#endif
	
#if defined(Y_BSD)
	typedef libc::exception    os_exception;
#endif
	
	namespace imported {

		//! base class for third party libraries error
		class exception : public upsylon::exception
		{
		public:
			virtual ~exception() throw();
			explicit exception( const char *reason /*->what*/, const char *fmt /*->when*/,...) throw() Y_PRINTF_CHECK(3,4);
			exception( const exception &other ) throw();
			exception & operator=( const upsylon::exception &other ) throw();
			exception & operator=( const exception &other ) throw();
			
			virtual const char *what() const throw();
			
		private:
			char      what_[128];
		};

	}
}

#endif
