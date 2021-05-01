########################################################################
#
#
# setup from R and system to compile modules
#
#
########################################################################

SET(R_FOUND OFF)

FIND_PROGRAM(R R DOC "R Executable")
IF( ${R} STREQUAL R-NOTFOUND )
	####################################################################
	#
	# no R on the system...
	#
	####################################################################
	MESSAGE( STATUS "Cannot find R" )
ELSE()
	####################################################################
	#
	# R is installed
	#
	####################################################################
	MESSAGE( STATUS "Found R=<${R}>")
	SET(R_FOUND ON)
	
	####################################################################
	#
	# adjust the C++ libraries...
	#
	####################################################################
	SET(R_STDCXX "")

	####################################################################
	# FreeBSD
	####################################################################
	IF(Y_FREEBSD)
		IF(Y_GNU)
		SET(R_STDCXX "/usr/local/lib/${Y_CC}/libstdc++.a")
		ENDIF()
	ENDIF()

	####################################################################
	# OpenBSD6.5 : TODO: find name
	####################################################################
	IF(Y_OPENBSD)
		IF(Y_GNU)
		SET(R_STDCXX "/usr/lib/libstdc++.so.57.0")
		ENDIF()
	ENDIF()
	
	####################################################################
	# Linux
	####################################################################
	IF(Y_LINUX)
		IF(Y_INTEL)
			SET(ILP "")
			IF(Y32)
				SET(ILP "ia32")
			ENDIF()
			IF(Y64)
				SET(ILP "intel64") 
			ENDIF()
			SET(R_STDCXX "/opt/intel/lib/${ILP}/libintlc.so")
		ENDIF()
	ENDIF()

	####################################################################
	# MacOS
	####################################################################
	IF(Y_DARWIN)
		IF(Y_GNU)
			SET(R_STDCXX "/opt/local/lib/libgcc/libstdc++.6.dylib")
		ENDIF()
	ENDIF()

	####################################################################
	#
	# OK....
	#
	####################################################################	
	MESSAGE( STATUS "R_STDCXX=<${R_STDCXX}>" )
	
ENDIF()
	