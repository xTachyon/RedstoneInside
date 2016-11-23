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
        src/literaloperators.hpp
        src/logger.cxx
        src/logger.hpp
        src/main.cxx
        src/position.hpp
        src/session.cxx
        src/session.hpp
        src/stdafx.cxx
        src/stdafx.hpp
        src/mingwworkaround.hpp
        src/nbt/vectorial.hpp)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS filesystem iostreams system thread)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(redi ${SOURCE_FILES})
    target_link_libraries(redi ${Boost_LIBRARIES})
endif()

if(MINGW)
    target_link_libraries(redi ws2_32)
endif()

if (CMAKE_COMPILER_IS_GNUCC)
    target_link_libraries(redi pthread)
endif()