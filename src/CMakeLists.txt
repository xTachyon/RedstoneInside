project(redi CXX)

set(SOURCE_FILES
      bytebuffer.cpp
      bytebuffer.hpp
      enums.cpp
      enums.hpp
      logger.cpp
      logger.hpp
      main.cpp
      player.cpp
      player.hpp
      playerposition.hpp
      server.cpp
      server.hpp
      serverconfig.cpp
      serverconfig.hpp
      session.cpp
      session.hpp
      sizeliteraloperators.hpp
      threadsafequeue.hpp
      vectorn.hpp

      util/base64.hpp
      util/file.cpp
      util/file.hpp
      util/maths.cpp
      util/maths.hpp
      util/random.hpp
      util/string.hpp
      util/time.cpp
      util/time.hpp
      util/util.hpp

      world.cpp
      world.hpp
      world/anvil.cpp
      world/anvil.hpp
      world/anvilregion.cpp
      world/anvilregion.hpp
      world/biome.hpp
      world/chunk.cpp
      world/chunk.hpp
      world/chunkcolumn.hpp
      world/chunkmanager.cpp
      world/chunkmanager.hpp
      world/memoryregion.cpp
      world/memoryregion.hpp
      world/terraingenerator.cpp
      world/terraingenerator.hpp
      world/block.hpp
      world/chunkdeseriazer.cpp
      world/chunkdeseriazer.hpp
      world/chunkserializer.cpp
      world/chunkserializer.hpp

      protocol/chunkserializer13.hpp
      protocol/packetreader.cpp
      protocol/packetreader.hpp
      protocol/packets/packet.cpp
      protocol/packets/packet.hpp
      protocol/packets/packethandler.cpp
      protocol/packets/packethandler.hpp
      protocol/packets/server/handshakepackets.cpp
      protocol/packets/server/handshakepackets.hpp
      protocol/packetwriter.cpp
      protocol/packetwriter.hpp
      protocol/chunkserializer13.cpp

      messages/event.hpp
      messages/eventmanager.cpp
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
      commands/redicommands.cpp
      commands/redicommands.hpp
      chat/chatmanager.cpp

      lockfree/queue.hpp

      nbt/array.hpp
      nbt/basic.hpp
      nbt/compound.cpp
      nbt/compound.hpp
      nbt/creator.cpp
      nbt/creator.hpp
      nbt/deserializer.cpp
      nbt/deserializer.hpp
      nbt/forward.hpp
      nbt/list.cpp
      nbt/list.hpp
      nbt/nbt.hpp
      nbt/prettyprinter.cpp
      nbt/prettyprinter.hpp
      nbt/primitive.hpp
      nbt/roottag.cpp
      nbt/roottag.hpp
      nbt/serializer.cpp
      nbt/serializer.hpp
      nbt/string.hpp
      nbt/tag.cpp
      nbt/type.cpp
      nbt/type.hpp
      nbt/value.cpp
      nbt/value.hpp
      nbt/tag.hpp
      nbt/datatypes.h
      nbt/end.hpp
      nbt/visitor.hpp

      datatypes.hpp
      checks.cpp
      util/threadgroup.hpp
      buffers.hpp
      hasserver.hpp
      commands/commandsender.hpp
      commands/commandsender.cpp
      commands/commandmanager.cpp
      commands/commandmanager.hpp
      commands/command.hpp
      commands/command.cpp
      commands/redicommands.cpp
      commands/commanddata.hpp
      chat/chatcomponent.cpp
      chat/chatcomponent.hpp
      protocol/varint.cpp
      protocol/varint.hpp
      util/compressor.hpp
      util/compressor.cpp
      servericon.cpp
      servericon.hpp
      filesystem.hpp
      util/string.cpp
      networking.hpp
      networking_linux.cpp
      networking_asio.cpp
      asiofile.cpp
      protocol/packets/server/statuspackets.hpp
      protocol/packets/server/statuspackets.cpp
      protocol/packets/server/loginpackets.hpp
      protocol/packets/server/loginpackets.cpp
      protocol/packets/server/playpackets.hpp
      protocol/packets/server/playpackets.cpp)

add_executable(redi ${SOURCE_FILES})

set(Boost_USE_STATIC_LIBS ON)
add_definitions(-DBOOST_SYSTEM_NO_DEPRECATED)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};../CMake")
find_package(Boost REQUIRED COMPONENTS filesystem iostreams program_options system)

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
  include_directories(${JSON_REPO_DIR}/include)
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
#cotire(redi)