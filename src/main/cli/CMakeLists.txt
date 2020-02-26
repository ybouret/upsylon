PROJECT(cli)

MESSAGE( STATUS "")
MESSAGE( STATUS "<Command Line Interface Tools>" )

FUNCTION(CLI TOOL)
	ADD_EXECUTABLE(${TOOL} "${TOOL}.cpp")
	TARGET_LINK_Y(${TOOL})
	INSTALL( TARGETS ${TOOL} RUNTIME DESTINATION bin )
ENDFUNCTION(CLI)


IF(FALSE)
  CLI(nwsrt)
ENDIF()

CLI(y_uniq)
CLI(y_bin2dat)
CLI(y_hexdump)
CLI(y_grep)

IF(NOT MSVC)
# generate rsa keys with msys-openssl
CLI(y_rsa_keys)
ENDIF()

IF(Y_DARWIN)
CLI(y_port_required)
#CLI(get-kde-wallpapers)
ENDIF()

CLI(y_mpi_xcopy)

MESSAGE( STATUS "<Command Line Interface Tools/>" )
MESSAGE( STATUS "")

