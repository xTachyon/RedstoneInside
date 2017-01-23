cmake_minimum_required(VERSION 3.2)
project(redi)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
        src/nbt/compiletimemagic.hpp
        src/nbt/compound.cxx
        src/nbt/compound.hpp
        src/nbt/creator.cxx
        src/nbt/creator.hpp
        src/nbt/list.cxx
        src/nbt/list.hpp
        src/nbt/nbt.hpp
        src/nbt/proxyclass.hpp
        src/nbt/scalar.hpp
        src/nbt/stdspecializations.hpp
        src/nbt/string.cxx
        src/nbt/string.hpp
        src/nbt/tag.cxx
        src/nbt/tag.hpp
        src/nbt/type.cxx
        src/nbt/type.hpp
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
        src/nbt/vectorial.hpp
        src/nbt/prettyprint.cxx
        src/nbt/prettyprint.hpp
        src/nbt/roottag.hpp
        src/nbt/deserializer.cxx
        src/nbt/deserializer.hpp
        src/nbt/exceptions.hpp
        src/nbt/serializer.cxx
        src/nbt/serializer.hpp
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
        src/protocol/protocol.hpp
        src/protocol/protocol1_11.cxx
        src/protocol/protocol1_11.hpp
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
        src/events/functor.hpp
        src/events/eventpriority.hpp
        src/serverconfig.cxx src/bytebuffer.cxx)

add_executable(redi ${SOURCE_FILES})

if("${JSON_REPO_DIR} " STREQUAL " ")
    message(FATAL_ERROR "JSON NOT found. Set JSON_REPO_DIR to the repository folder.")
else()
    message(STATUS "JSON found : ${JSON_REPO_DIR}/src")
    include_directories(${JSON_REPO_DIR}/src)
endif()

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS filesystem iostreams program_options system thread)
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

if(MSVC)
    # http://stackoverflow.com/a/3818084/4091452
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wdisabled-optimization -Wno-unused-local-typedefs -Wno-overflow -Wno-deprecated-declarations")
endif()

if(WIN32)
    target_link_libraries(redi wsock32)
endif()