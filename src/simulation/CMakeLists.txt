set(SOURCE_LIST
    Droplet.cpp
    DropletBoundary.cpp
    Fluid.cpp
    Injection.cpp
    ResistanceModels.cpp
    Simulation.cpp
)

set(HEADER_LIST
    Droplet.h
    DropletBoundary.h
    Fluid.h
    Injection.h
    IResistanceModel.h
    ResistanceModels.h
    Simulation.h
)

target_sources(${TARGET_NAME} PRIVATE ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(events)
