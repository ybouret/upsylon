
ADD_EXECUTABLE(y_rc y_rc.cpp)
TARGET_LINK_Y(y_rc)
INSTALL( TARGETS y_rc RUNTIME DESTINATION bin)

IF(FALSE)
  ADD_EXECUTABLE(nwsrt nwsrt.cpp)
  TARGET_LINK_Y(nwsrt)
ENDIF()

ADD_EXECUTABLE(mprimes mprimes.cpp)
TARGET_LINK_Y(mprimes)

ADD_EXECUTABLE(y_uniq y_uniq.cpp)
TARGET_LINK_Y(y_uniq)
INSTALL( TARGETS y_uniq RUNTIME DESTINATION bin)

IF(Y_DARWIN)

ADD_EXECUTABLE(y_port_missing y-port-missing.cpp)
TARGET_LINK_Y(y_port_missing)

INSTALL( TARGETS y_port_missing RUNTIME DESTINATION bin)

ENDIF()
