cmake_minimum_required(VERSION 3.26)

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing executable name in this file
# for now, the project name is used as the executable name
set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")


project(${MAIN_PROJECT_NAME})
# set(CMAKE_PROJECT_VERSION_MAJOR 0)
# set(CMAKE_PROJECT_VERSION_MINOR 0)
# set(CMAKE_PROJECT_VERSION_PATCH 1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

###############################################################################

# external dependencies with FetchContent

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

# din cauza SFML
set(WARNINGS_AS_ERRORS OFF)

# NOTE: Also update SFML_VERSION env var in .github/workflows/cmake.yml:33
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.0
    # GIT_TAG        7f1162dfea4969bc17417563ac55d93b72e84c1e # 3.0.0
    # GIT_TAG        origin/master
    GIT_SHALLOW    1  # works only with branches or tags, not with arbitrary commit hashes
)

FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.12.0
)
FetchContent_MakeAvailable(json)

###############################################################################

# external dependencies with find_package

find_package(Threads REQUIRED)
if(APPLE)
elseif(UNIX)
    find_package(X11)
endif()

###############################################################################

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing name here
add_executable(${MAIN_EXECUTABLE_NAME}
    main.cpp
    generated/src/Helper.cpp
    Source/Player.cpp
    Headers/Player.h
    Source/BulletType.cpp
    Headers/BulletType.h
    Source/Bullet.cpp
    Headers/Bullet.h
        Headers/BattleBox.h
        Source/BattleBox.cpp
        Headers/Button.h
        Source/Button.cpp
        Headers/BattleText.h
        Source/BattleText.cpp
        Headers/DrawableEntity.h
        Headers/SpriteEntity.h
        Headers/ShapeEntity.h
        Source/SpriteEntity.cpp
        Source/ShapeEntity.cpp
        Headers/StatefulSprite.h
        Source/StatefulSprite.cpp
        Headers/AnimatedEntity.h
        Source/AnimatedEntity.cpp
        Headers/GameState.h
        Headers/SplashState.h
        Headers/PlayState.h
        Headers/GameManager.h
        Source/GameManager.cpp
        Source/SplashState.cpp
        Source/PlayState.cpp
        Source/GameState.cpp
        Headers/Singleton.h
        Headers/ResourceManager.h
        Headers/InstructionsState.h
        Source/InstructionsState.cpp
        Headers/Exceptions.h
        Source/Exceptions.cpp
        Headers/BitmapFont.h
        Source/BitmapFont.cpp
        Headers/Froggit.h
        Source/Froggit.cpp
        Headers/Hp.h
        Source/Hp.cpp
        Headers/GameOverState.h
        Source/GameOverState.cpp
        Headers/ShardEntity.h
        Source/ShardEntity.cpp
        Headers/Enemy.h
        Source/Enemy.cpp
        Headers/BattleUISystem.h
        Source/BattleUISystem.cpp
        Headers/PlayerTurnSystem.h
        Source/PlayerTurnSystem.cpp
        Headers/BulletManager.h
        Source/BulletManager.cpp
        Headers/DeathAnimationManager.h
        Source/DeathAnimationManager.cpp
        Headers/EnemyTurnSystem.h
        Source/EnemyTurnSystem.cpp
        Headers/BulletPattern.h
)

# NOTE: Add all defined targets (e.g. executables, libraries, etc. )
# NOTE: RUN_SANITIZERS is optional, if it's not present it will default to true
set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})
# set_compiler_flags(TARGET_NAMES ${MAIN_EXECUTABLE_NAME} ${FOO} ${BAR})
# where ${FOO} and ${BAR} represent additional executables or libraries
# you want to compile with the set compiler flags

###############################################################################

# use SYSTEM so cppcheck and clang-tidy do not report warnings from these directories
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE generated/include)
# target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ext/<SomeHppLib>/include)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ${SFML_SOURCE_DIR}/include)
target_link_directories(${MAIN_EXECUTABLE_NAME} PRIVATE ${SFML_BINARY_DIR}/lib)
target_link_libraries(${MAIN_EXECUTABLE_NAME}
        PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
        Threads::Threads
        nlohmann_json::nlohmann_json
)


if(APPLE)
elseif(UNIX)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
endif()

###############################################################################

# copy binaries to "bin" folder; these are uploaded as artifacts on each release
# DESTINATION_DIR is set as "bin" in cmake/Options.cmake:6
install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()


copy_files(FILES tastatura.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
# copy_files(FILES tastatura.txt config.json DIRECTORY images sounds COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
# copy_files(DIRECTORY images sounds COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
copy_files(DIRECTORY img mus fonts sounds json COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
