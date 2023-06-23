set(SOURCE_LIST
    Channel.cpp
    ChannelPosition.cpp
    Chip.cpp
    Edge.cpp
    FlowRatePump.cpp
    Node.cpp
    PressurePump.cpp
)
    
set(HEADER_LIST
    Channel.h
    ChannelPosition.h
    Chip.h
    Edge.h
    FlowRatePump.h
    Node.h
    PressurePump.h
)

target_sources(${TARGET_NAME} PRIVATE ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})