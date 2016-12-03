cmake_minimum_required(VERSION 3.2)
project(redi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

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
        src/protocol/packet.cxx
        src/protocol/packet.hpp
        src/util/util.cxx
        src/util/util.hpp
        src/binarydata.hpp
        src/block.hpp
        src/compressor.cxx
        src/compressor.hpp
        src/sizeliteraloperators.hpp
        src/logger.cxx
        src/logger.hpp
        src/main.cxx
        src/position.hpp
        src/session.cxx
        src/session.hpp
        src/stdafx.cxx
        src/stdafx.hpp
        src/mingwworkaround.hpp
        src/nbt/vectorial.hpp
        src/nbt/printer.cxx
        src/nbt/printer.hpp
        src/nbt/roottag.hpp
        src/nbt/bytesreader.cxx
        src/nbt/bytesreader.hpp
        src/nbt/exceptions.hpp
        src/nbt/byteswriter.cxx
        src/nbt/byteswriter.hpp
        src/region.cxx
        src/region.hpp)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS filesystem iostreams system thread)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(redi ${SOURCE_FILES})
    target_link_libraries(redi ${Boost_LIBRARIES})
endif()

find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries(redi ${ZLIB_LIBRARIES})
endif()

if (MINGW)
    target_link_libraries(redi ws2_32)
endif()

if (CMAKE_COMPILER_IS_GNUCC)
    target_link_libraries(redi pthread)
endif()

if (MSVC)
    # http://stackoverflow.com/a/3818084/4091452
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wdisabled-optimization")
endif()