include_directories(${HSPlasma_INCLUDE_DIRS})
include_directories(${3dsm_INCLUDE_DIR})

set(IMPORTER_SOURCES
    DllEntry.cpp
    PlasmaImport.cpp
)

set(IMPORTER_HEADERS
    PlasmaImport.h
    PlasmaImport.def
    veroverrides.h
)

set(IMPORTER_RESOURCES
    PlasmaImport.rc
    resource.h
)

add_library(PlasmaImport SHARED ${IMPORTER_SOURCES} ${IMPORTER_HEADERS} ${IMPORTER_RESOURCES})
target_link_libraries(PlasmaImport HSPlasma)
target_link_libraries(PlasmaImport ${3dsm_LIBRARIES})

set_target_properties(PlasmaImport PROPERTIES SUFFIX ".dli")

source_group("Source Files" FILES ${IMPORTER_SOURCES})
source_group("Header Files" FILES ${IMPORTER_HEADERS})
source_group("Resource Files" FILES ${IMPORTER_RESOURCES})