file(GLOB HEADERS "*.h")
file(GLOB SOURCES "*.cpp")
add_library(InnoGLRenderingServer ${HEADERS} ${SOURCES})
set_property(TARGET InnoGLRenderingServer PROPERTY POSITION_INDEPENDENT_CODE ON)
set_target_properties(InnoGLRenderingServer PROPERTIES FOLDER Engine/RenderingServer)
target_link_libraries(InnoGLRenderingServer ${OPENGL_LIBRARIES})
find_library(GLAD glad)
target_link_libraries(InnoGLRenderingServer "${GLAD}")