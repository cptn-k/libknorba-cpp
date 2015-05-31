cmake_minimum_required (VERSION 2.6)

project(libKnoRBA)
set(VERSION 0.6)

set(CMAKE_CXX_FLAGS "-fvisibility=hidden")
include_directories(SYSTEM /usr/local/include)
include_directories(SYSTEM src)

#//\/ libKnoRBA /\/////////////////////////////////////////////////////////////

add_library (knorba STATIC
  src/knorba/Agent.cpp
  src/knorba/Group.cpp
  src/knorba/Message.cpp
  src/knorba/MessageSet.cpp
  src/knorba/AgentLoader.cpp
  src/knorba/Protocol.cpp
  src/knorba/type/KType.cpp
  src/knorba/type/KTypeMismatchException.cpp
  src/knorba/type/KValue.cpp
  src/knorba/type/KEnumerationType.cpp
  src/knorba/type/KGridType.cpp
  src/knorba/type/KRecordType.cpp
  src/knorba/type/KTruth.cpp
  src/knorba/type/KInteger.cpp
  src/knorba/type/KLongint.cpp
  src/knorba/type/KReal.cpp
  src/knorba/type/KGlobalUid.cpp
  src/knorba/type/KRaw.cpp
  src/knorba/type/KString.cpp
  src/knorba/type/KEnumeration.cpp
  src/knorba/type/KAny.cpp
  src/knorba/type/KGrid.cpp
  src/knorba/type/KRecord.cpp
  src/knorba/type/KOctet.cpp)

target_link_libraries (knorba
  LINK_PUBLIC kfoundation)

set_target_properties (knorba
  PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    ARCHIVE_OUTPUT_DIRECTORY lib)

install(TARGETS knorba
  ARCHIVE DESTINATION lib)

install(FILES
  src/knorba/Agent.h
  src/knorba/Group.h
  src/knorba/Message.h
  src/knorba/MessageSet.h
  src/knorba/Runtime.h
  src/knorba/AgentLoader.h
  src/knorba/Protocol.h
  DESTINATION include/knorba)


install(FILES
  src/knorba/type/all.h
  src/knorba/type/definitions.h
  src/knorba/type/KValue.h
  src/knorba/type/KType.h
  src/knorba/type/KTypeMismatchException.h
  src/knorba/type/KEnumerationType.h
  src/knorba/type/KGridType.h
  src/knorba/type/KRecordType.h
  src/knorba/type/KTruth.h
  src/knorba/type/KInteger.h
  src/knorba/type/KLongint.h
  src/knorba/type/KReal.h
  src/knorba/type/KGlobalUid.h
  src/knorba/type/KRaw.h
  src/knorba/type/KString.h
  src/knorba/type/KEnumeration.h
  src/knorba/type/KDynamicValue.h
  src/knorba/type/KAny.h
  src/knorba/type/KGrid.h
  src/knorba/type/KRecord.h
  src/knorba/type/KOctet.h
  DESTINATION include/knorba/type)


# //\/ StandardProtocols /\////////////////////////////////////////////////////

add_library (KnorbaStandardProtocols
  src/knorba/protocol/UnixSocketClient.cpp
  src/knorba/protocol/TunnelingProtocol.cpp
  src/knorba/protocol/TunnelingClient.cpp
  src/knorba/protocol/TunnelingServer.cpp
  src/knorba/protocol/DisplayInfoProtocol.cpp
  src/knorba/protocol/ConsoleProtocolClient.cpp
  src/knorba/protocol/GroupingProtocol.cpp
  src/knorba/protocol/PhaserProtocol.cpp
  src/knorba/protocol/DisplayInfoProtocol.cpp)

target_link_libraries (KnorbaStandardProtocols
  knorba)

set_target_properties (KnorbaStandardProtocols
  PROPERTIES
  POSITION_INDEPENDENT_CODE ON
    ARCHIVE_OUTPUT_DIRECTORY lib)


install(TARGETS KnorbaStandardProtocols
  ARCHIVE DESTINATION lib)

install(FILES
  src/knorba/protocol/UnixSocketClient.h
  src/knorba/protocol/TunnelingProtocol.h
  src/knorba/protocol/TunnelingClient.h
  src/knorba/protocol/TunnelingServer.h
  src/knorba/protocol/DisplayInfoProtocol.h
  src/knorba/protocol/ConsoleProtocolClient.h
  src/knorba/protocol/GroupingProtocol.h
  src/knorba/protocol/PhaserProtocol.h
  src/knorba/protocol/DisplayInfoProtocol.h
  DESTINATION include/knorba/protocol)
