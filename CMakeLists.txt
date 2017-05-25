cmake_minimum_required(VERSION 2.8.4)
project(engine)

add_definitions(-DSHINY_IS_COMPILED=FALSE)

include_directories(../libs/sql/include)

set(SOURCE_FILES Config.h
                 FlowControl.h
                 FlowControl.cpp
                 FrameInfo.h
                 GameBase.h
                 GameBase.cpp
                 GameState.h
                 GameStateManager.h
                 GameStateManager.cpp
                 ObjectPool.h
                 Random.h
                 Random.cpp
				 TimedInput.h
				 TimedInput.cpp)

set(GO_SOURCE_FILES	go/Go.h
                    go/Go.cpp
                    go/GoAlloc.h
                    go/GoComponent.h
                    go/GoID.h)

list(APPEND ALL_SOURCE_FILES ${SOURCE_FILES})
set_source_files_properties(${SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("src" FILES ${SOURCE_FILES})

list(APPEND ALL_SOURCE_FILES ${GO_SOURCE_FILES})
set_source_files_properties(${GO_SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("src\\go" FILES ${GO_SOURCE_FILES})

list(APPEND ALL_SOURCE_FILES _engine.cpp)
source_group("src" FILES _engine.cpp)

add_library(engine STATIC ${ALL_SOURCE_FILES})
target_link_libraries(engine Shiny sqlite ai net ren sys ui)

add_subdirectory(tests)