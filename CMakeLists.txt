cmake_minimum_required (VERSION 3.8)

project ("StelForgeEngine")

set(ENGINE_NAME StelForgeEngine)
set(ENGINE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${ENGINE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${ENGINE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${ENGINE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ENGINE_PATH})

set(XCODE FALSE)
if(CMAKE_GENERATOR STREQUAL Xcode)
	set(XCODE TRUE)
endif()

add_subdirectory(lib/glad/)
add_subdirectory(lib/imgui/)

add_subdirectory(src)
add_subdirectory(src/submodules/JoltPhysics/Build)

set_property(TARGET ${ENGINE_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${ENGINE_PATH}/bin)
set_property(TARGET ${ENGINE_NAME} PROPERTY WORKING_DIRECTORY ${ENGINE_PATH}/bin)
set_property(TARGET ${ENGINE_NAME} PROPERTY CXX_STANDARD 23)

if(XCODE)
	set_target_properties(${ENGINE_NAME} PROPERTIES
		XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${ENGINE_PATH}/bin")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set_target_properties(glad imgui Jolt PROPERTIES FOLDER Dependencies)

set_target_properties(${ENGINE_NAME} PROPERTIES
	FOLDER Engine
    DEBUG_POSTFIX "-d"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${ENGINE_PATH}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${ENGINE_PATH}"
)

#set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})