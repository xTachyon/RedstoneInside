project(redi CXX)

set(SOURCE_FILES
        bytebuffer.cxx
        bytebuffer.hpp
        connectionlistener.cxx
        connectionlistener.hpp
        enums.cxx
        enums.hpp
        exceptions.hpp
        logger.cxx
        logger.hpp
        main.cxx
        player.cxx
        player.hpp
        playerposition.hpp
        server.cxx
        server.hpp
        serverconfig.cxx
        serverconfig.hpp
        session.cxx
        session.hpp
        sizeliteraloperators.hpp
        threadsafequeue.hpp
        vectorn.hpp

        util/base64.hpp
        util/file.cxx
        util/file.hpp
        util/maths.cxx
        util/maths.hpp
        util/random.hpp
        util/string.hpp
        util/time.cxx
        util/time.hpp
        util/util.hpp

        compressor.hpp
        compressor.cxx

        world.cxx
        world.hpp
        world/anvil.cxx
        world/anvil.hpp
        world/anvilregion.cxx
        world/anvilregion.hpp
        world/biome.hpp
        world/chunk.cxx
        world/chunk.hpp
        world/chunkcolumn.hpp
        world/chunkmanager.cxx
        world/chunkmanager.hpp
        world/memoryregion.cxx
        world/memoryregion.hpp
        world/terraingenerator.cxx
        world/terraingenerator.hpp
        world/block.hpp
        world/chunkdeseriazer.cxx
        world/chunkdeseriazer.hpp
        world/chunkserializer.cxx
        world/chunkserializer.hpp

        protocol/chunkserializer13.hpp
        protocol/packetreader.cxx
        protocol/packetreader.hpp
        protocol/packets/packet.cxx
        protocol/packets/packet.hpp
        protocol/packets/packethandler.cxx
        protocol/packets/packethandler.hpp
        protocol/packets/server/handshake/handshake.cxx
        protocol/packets/server/handshake/handshake.hpp
        protocol/packets/server/login/loginstart.cxx
        protocol/packets/server/login/loginstart.hpp
        protocol/packets/server/login/loginsucces.cxx
        protocol/packets/server/login/loginsucces.hpp
        protocol/packets/server/login/setcompression.cxx
        protocol/packets/server/login/setcompression.hpp
        protocol/packets/server/play/chatmessage.cxx
        protocol/packets/server/play/chatmessage.hpp
        protocol/packets/server/play/chunkdata.cxx
        protocol/packets/server/play/chunkdata.hpp
        protocol/packets/server/play/disconnect.cxx
        protocol/packets/server/play/disconnect.hpp
        protocol/packets/server/play/entitylookandrelativemove.cxx
        protocol/packets/server/play/entitylookandrelativemove.hpp
        protocol/packets/server/play/joingame.cxx
        protocol/packets/server/play/joingame.hpp
        protocol/packets/server/play/keepalive.cxx
        protocol/packets/server/play/keepalive.hpp
        protocol/packets/server/play/playerlistitem.cxx
        protocol/packets/server/play/playerlistitem.hpp
        protocol/packets/server/play/playerlook.cxx
        protocol/packets/server/play/playerlook.hpp
        protocol/packets/server/play/playerposition.cxx
        protocol/packets/server/play/playerposition.hpp
        protocol/packets/server/play/playerpositionandlook.cxx
        protocol/packets/server/play/playerpositionandlook.hpp
        protocol/packets/server/play/spawnplayer.cxx
        protocol/packets/server/play/spawnplayer.hpp
        protocol/packets/server/play/spawnposition.cxx
        protocol/packets/server/play/spawnposition.hpp
        protocol/packets/server/play/timeupdate.hpp
        protocol/packets/server/play/unloadchunk.cxx
        protocol/packets/server/play/unloadchunk.hpp
        protocol/packets/server/status/ping.cxx
        protocol/packets/server/status/ping.hpp
        protocol/packets/server/status/pong.cxx
        protocol/packets/server/status/pong.hpp
        protocol/packets/server/status/request.cxx
        protocol/packets/server/status/request.hpp
        protocol/packets/server/status/response.cxx
        protocol/packets/server/status/response.hpp
        protocol/packetwriter.cxx
        protocol/packetwriter.hpp
        protocol/chunkserializer13.cxx

        messages/event.hpp
        messages/eventmanager.cxx
        messages/eventmanager.hpp
        messages/eventpriority.hpp
        messages/events.hpp
        messages/eventtype.hpp
        messages/playerdisconnected.hpp
        messages/playerjoin.hpp
        messages/playerlook.hpp
        messages/playerposition.hpp
        messages/playerpositionandlook.hpp
        messages/sendkeepalivering.hpp
        messages/sessiondisconnected.hpp
        messages/statusrequest.hpp
        messages/tick.hpp
        messages/chatmessage.hpp
        messages/chunkloaded.hpp

        chat/chatmanager.hpp
        chat/commandmanager.cxx
        chat/commandmanager.hpp
        chat/commandsender2.cxx
        chat/commandsender2.hpp
        chat/redicommands.cxx
        chat/redicommands.hpp
        chat/chatmanager.cxx

        lockfree/queue.hpp

        nbt/array.hpp
        nbt/basic.hpp
        nbt/compound.cxx
        nbt/compound.hpp
        nbt/creator.cxx
        nbt/creator.hpp
        nbt/deserializer.cxx
        nbt/deserializer.hpp
        nbt/forward.hpp
        nbt/list.cxx
        nbt/list.hpp
        nbt/nbt.hpp
        nbt/prettyprinter.cxx
        nbt/prettyprinter.hpp
        nbt/primitive.hpp
        nbt/roottag.cxx
        nbt/roottag.hpp
        nbt/serializer.cxx
        nbt/serializer.hpp
        nbt/string.hpp
        nbt/tag.cxx
        nbt/type.cxx
        nbt/type.hpp
        nbt/value.cxx
        nbt/value.hpp
        nbt/tag.hpp
        datatypes.hpp
        checks.cxx
        util/threadgroup.hpp pch/pchh.hpp buffers.hpp HasServer.hpp chat/commandsender.hpp chat/commandsender.cxx)



add_executable(redi ${SOURCE_FILES})

set(Boost_USE_STATIC_LIBS ON)
add_definitions(-DBOOST_SYSTEM_NO_DEPRECATED)

if(MSVC)
  find_package(Boost REQUIRED COMPONENTS filesystem iostreams program_options system thread zlib)
else()
  find_package(Boost REQUIRED COMPONENTS filesystem iostreams program_options system)
endif()
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(redi ${Boost_LIBRARIES})
endif()

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries(redi ${ZLIB_LIBRARIES})
endif()

if(MINGW)
  target_link_libraries(redi ws2_32)
endif()

#find_package(Threads REQUIRED)
#target_link_libraries(redi "${CMAKE_THREAD_LIBS_INIT}")

if(UNIX)
  target_link_libraries(redi pthread)
endif()

if(WIN32)
  target_link_libraries(redi wsock32)
endif()

if("${JSON_REPO_DIR} " STREQUAL " ")
  message(FATAL_ERROR "JSON NOT found. Set JSON_REPO_DIR to the repository folder.")
else()
  message(STATUS "JSON found : ${JSON_REPO_DIR}/src")
  include_directories(${JSON_REPO_DIR}/src)
endif()

if(UNIX OR MINGW)
  add_definitions(-DREDI_FUNCTION=__PRETTY_FUNCTION__)
elseif(MSVC)
  add_definitions(-DREDI_FUNCTION=__FUNCTION__)
else()
  add_definitions(-DREDI_FUNCTION="")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DREDI_DEBUG)
endif()

include(../CMake/cotire.cmake)
cotire(redi)