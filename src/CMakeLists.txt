set(SOURCE_LIST
    Simulator.cpp
    Results.cpp
)

set(HEADER_LIST
    SimulatorImpl.h
)

# add subdirectories
add_subdirectory(nodalAnalysis)
add_subdirectory(architecture)
add_subdirectory(simulation)

target_sources(${TARGET_NAME} PRIVATE ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE nlohmann_json::nlohmann_json)