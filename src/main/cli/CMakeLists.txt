
ADD_EXECUTABLE(y_rc y_rc.cpp)
TARGET_LINK_Y(y_rc)
INSTALL( TARGETS y_rc RUNTIME DESTINATION bin)

IF(FALSE)
  ADD_EXECUTABLE(nwsrt nwsrt.cpp)
  TARGET_LINK_Y(nwsrt)
ENDIF()

#ADD_EXECUTABLE(mprimes mprimes.cpp)
#TARGET_LINK_Y(mprimes)

ADD_EXECUTABLE(y_uniq y_uniq.cpp)
TARGET_LINK_Y(y_uniq)
INSTALL( TARGETS y_uniq RUNTIME DESTINATION bin)

ADD_EXECUTABLE(y_hexdump y_hexdump.cpp)
TARGET_LINK_Y(y_hexdump)
INSTALL( TARGETS y_hexdump RUNTIME DESTINATION bin)



IF(Y_DARWIN)

ADD_EXECUTABLE(y_port_missing y-port-missing.cpp)
TARGET_LINK_Y(y_port_missing)
INSTALL( TARGETS y_port_missing RUNTIME DESTINATION bin)

ADD_EXECUTABLE(get-kde-wallpapers get-kde-wallpapers.cpp)
TARGET_LINK_Y(get-kde-wallpapers)

ENDIF()
