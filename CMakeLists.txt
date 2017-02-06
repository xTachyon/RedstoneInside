cmake_minimum_required(VERSION 3.2)
project(redi CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(warnings "-Wall -Wextra -Wpedantic -Wreorder -Wno-unused-local-typedefs -Wno-deprecated-declarations -Wno-overflow")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(warnings "/W4 /EHsc -D_WIN32_WINNT=0x0501 -D_SCL_SECURE_NO_WARNINGS")
endif()

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

set(SOURCE_FILES
        src/util/util.hpp
        src/bytebuffer.hpp
        src/world/block.hpp
        src/compressor.cxx
        src/compressor.hpp
        src/sizeliteraloperators.hpp
        src/logger.cxx
        src/logger.hpp
        src/main.cxx
        src/vectorn.hpp
        src/session.cxx
        src/session.hpp
        src/stdafx.cxx
        src/stdafx.hpp
        src/world/region.cxx
        src/world/region.hpp
        src/world/chunk.cxx
        src/world/chunk.hpp
        src/world.cxx
        src/world.hpp
        src/threadsafequeue.hpp
        src/connectionlistener.cxx
        src/connectionlistener.hpp
        src/server.cxx
        src/server.hpp
        src/player.cxx
        src/player.hpp
        src/protocol/packetreader.cxx
        src/protocol/packetreader.hpp
        src/protocol/packetwriter.cxx
        src/protocol/packetwriter.hpp
        src/util/base64.hpp
        src/util/time.hpp
        src/util/time.cxx
        src/util/string.hpp
        src/serverconfig.hpp
        src/events/event.hpp
        src/world/biome.hpp
        src/world/chunkcolumn.hpp
        src/world/terraingenerator.cxx
        src/world/terraingenerator.hpp
        src/world/chunkmanager.cxx
        src/world/chunkmanager.hpp
        src/protocol/chunkserializer13.cxx
        src/protocol/chunkserializer13.hpp
        src/events/eventtype.hpp
        src/events/events.hpp
        src/events/playerdisconnected.hpp
        src/events/sessiondisconnected.hpp
        src/events/sendkeepalivering.hpp
        src/events/eventmanager.cxx
        src/events/eventmanager.hpp
        src/events/eventpriority.hpp
        src/serverconfig.cxx src/bytebuffer.cxx
        src/chat/chatmanager.cxx
        src/chat/chatmanager.hpp
        src/events/chatmessage.hpp
        src/exceptions.hpp
        src/events/statusrequest.hpp
        src/enums.hpp
        src/enums.cxx
        src/events/playerjoin.hpp
        src/events/playerposition.hpp
        src/events/playerlook.hpp
        src/events/playerpositionandlook.hpp
        src/playerposition.hpp
        src/util/file.hpp
        src/util/file.cxx
        src/chat/commandsender.hpp
        src/chat/commandmanager.cxx
        src/chat/commandmanager.hpp
        src/chat/redicommands.cxx
        src/chat/redicommands.hpp
        src/chat/commandsender.cxx
        src/protocol/packetwriternocopy.hpp
        src/protocol/packetwriternocopy.cxx
        src/protocol/packets/packet.hpp
        src/protocol/packets/server/handshake/handshake.cxx
        src/protocol/packets/server/handshake/handshake.hpp
        src/protocol/packets/packet.cxx src/protocol/packets/server/login/loginstart.cxx src/protocol/packets/server/login/loginstart.hpp src/protocol/packets/server/login/loginsucces.cxx src/protocol/packets/server/login/loginsucces.hpp src/protocol/packets/server/login/setcompression.cxx src/protocol/packets/server/login/setcompression.hpp src/protocol/packets/server/play/spawnposition.cxx src/protocol/packets/server/play/spawnposition.hpp src/protocol/packets/server/play/joingame.cxx src/protocol/packets/server/play/joingame.hpp src/protocol/packets/server/play/playerpositionandlook.cxx src/protocol/packets/server/play/playerpositionandlook.hpp src/protocol/packets/packethandler.cxx src/protocol/packets/packethandler.hpp src/protocol/packets/server/status/request.cxx src/protocol/packets/server/status/request.hpp src/protocol/packets/server/status/response.cxx src/protocol/packets/server/status/response.hpp src/protocol/packets/server/status/ping.cxx src/protocol/packets/server/status/ping.hpp src/protocol/packets/server/status/pong.cxx src/protocol/packets/server/status/pong.hpp src/protocol/packets/server/play/chunkdata.cxx src/protocol/packets/server/play/chunkdata.hpp src/protocol/packets/server/play/keepalive.cxx src/protocol/packets/server/play/keepalive.hpp src/protocol/packets/server/play/chatmessage.cxx src/protocol/packets/server/play/chatmessage.hpp)

add_definitions(-DBOOST_SYSTEM_NO_DEPRECATED)

add_executable(redi ${SOURCE_FILES})

if("${JSON_REPO_DIR} " STREQUAL " ")
  message(FATAL_ERROR "JSON NOT found. Set JSON_REPO_DIR to the repository folder.")
else()
  message(STATUS "JSON found : ${JSON_REPO_DIR}/src")
  include_directories(${JSON_REPO_DIR}/src)
endif()

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS filesystem iostreams program_options system thread)
if(MSVC)
	find_package(Boost REQUIRED COMPONENTS filesystem iostreams program_options system thread zlib)
endif()
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(redi ${Boost_LIBRARIES})
endif()

#find_package(OpenSSL)
#if(OPENSSL_FOUND)
#    message(STATUS "OpenSSL version: ${OPENSSL_VERSION}")
#    include_directories(${OPENSSL_INCLUDE_DIRS})
#    target_link_libraries(redi ${OPENSSL_LIBRARIES})
#else()
#    message(FATAL_ERROR "Openssl not found")
#endif()

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries(redi ${ZLIB_LIBRARIES})
endif()

if(MINGW)
  target_link_libraries(redi ws2_32)
endif()

if (UNIX)
  target_link_libraries(redi pthread)
endif()

if(WIN32)
  target_link_libraries(redi wsock32)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DREDI_DEBUG)
endif()

if(UNIX OR MINGW)
  add_definitions(-DREDI_FUNCTION=__PRETTY_FUNCTION__)
elseif(MSVC)
  add_definitions(-DREDI_FUNCTION=__FUNCTION__)
else()
  add_definitions(-DREDI_FUNCTION="")
endif()