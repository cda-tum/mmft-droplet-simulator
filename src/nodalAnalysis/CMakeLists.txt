set(SOURCE_LIST
    NodalAnalysis.cpp
)

set(HEADER_LIST
    INode.h
    IEdge.h
    IResistance.h
    IFlowRatePump.h
    IPressurePump.h
    NodalAnalysis.h
)

target_sources(${TARGET_NAME} PRIVATE ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE eigen)