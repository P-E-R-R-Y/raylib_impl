#----------- general -----------

cmake_minimum_required(VERSION 3.24)
project(raylib_impl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#----------- project -----------

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(CMakeUtils REQUIRED)
include(${cmakeutils_SOURCE_DIR}/ToolVar.cmake)

# FindRaylib force BUILD_SHARED_LIBS ET CMAKE_POSITION_INDEPENDENT_CODE :
# on les retire pour que chaque dependance decide, et on les remet apres.
cmake_var_extract(BUILD_SHARED_LIBS)
cmake_var_extract(CMAKE_POSITION_INDEPENDENT_CODE)

find_package(System REQUIRED)
find_package(modulemanager REQUIRED)
find_package(igraphic REQUIRED)
find_package(iaudio REQUIRED)
find_package(Raylib REQUIRED)

cmake_var_restore(BUILD_SHARED_LIBS)
cmake_var_restore(CMAKE_POSITION_INDEPENDENT_CODE)

set(SOURCE_FILES sources/raylib.cpp)

# Deux facons de recuperer ce vendor, deux cibles explicites :
#
#   raylib_impl         SHARED  raylib_impl.dylib   -> a dlopen (ModuleManager)
#   raylib_impl_static  STATIC  libraylib_impl.a    -> a linker en dur
#
# Le type n'est VOLONTAIREMENT pas gouverne par BUILD_SHARED_LIBS. La raison
# d'etre du plugin est d'etre charge a chaud : si le drapeau global le
# transformait en .a, Load() echouerait au runtime avec un message obscur,
# et l'exemple dynamique comme les tests casseraient sans rien dire. Le
# consommateur choisit par le NOM de la cible, pas par un drapeau global.
add_library(${PROJECT_NAME}        SHARED ${SOURCE_FILES})
add_library(${PROJECT_NAME}_static STATIC ${SOURCE_FILES})

#----------- dependencies -----------

foreach(flavour ${PROJECT_NAME} ${PROJECT_NAME}_static)
  target_link_libraries(${flavour} PUBLIC system igraphic iaudio raylib)
  # la version statique peut finir dans une shared en aval : PIC des le depart
  set_target_properties(${flavour} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endforeach()

# plus de dependance a maths : RayPolygon porte son propre decoupage en
# oreilles, et Triangle / Vector2f / epsilond viennent de system.

set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# Ce que raylib exige de la plateforme. Une lib STATIQUE ne linke rien
# elle-meme : c'est l'executable final qui le fera, donc ces dependances
# doivent lui etre transmises en PUBLIC. Sans ca, chaque consommateur
# devrait recopier la liste des frameworks - ce que l'exemple statique
# faisait avant, et qui n'a plus lieu d'etre.
if (APPLE)
  set(PLATFORM_LIBS "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif(UNIX)
  find_package(X11 REQUIRED)
  set(PLATFORM_LIBS ${X11_LIBRARIES} m pthread dl GL)
  target_include_directories(${PROJECT_NAME}        PRIVATE ${X11_INCLUDE_DIR})
  target_include_directories(${PROJECT_NAME}_static PUBLIC  ${X11_INCLUDE_DIR})
endif()

target_link_libraries(${PROJECT_NAME}        PRIVATE ${PLATFORM_LIBS})
target_link_libraries(${PROJECT_NAME}_static PUBLIC  ${PLATFORM_LIBS})

#----------- includes -----------

foreach(flavour ${PROJECT_NAME} ${PROJECT_NAME}_static)
  target_include_directories(${flavour} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/window
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/graphic
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/event
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/audio
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
  )
endforeach()

# raylib_impl.dylib, pas libraylib_impl.dylib : c'est ce nom que
# ModuleManager::Load() recoit dans les tests et les exemples.
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

#----------- examples -----------

# Les deux facons de recuperer le vendor, cote a cote.

# dynamique : utilitaires + modulemanager + contrats. Ne linke PAS le vendor, il
# le dlopen au runtime - d'ou l'argv[1] qui accepte n'importe quelle dll.
add_executable(${PROJECT_NAME}_demo_dynamic examples/dynamic.cpp)
add_dependencies(${PROJECT_NAME}_demo_dynamic ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}_demo_dynamic PRIVATE examples)
target_compile_definitions(${PROJECT_NAME}_demo_dynamic PRIVATE
  RAYLIB_IMPL_PATH="$<TARGET_FILE:${PROJECT_NAME}>"
  ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)
target_link_libraries(${PROJECT_NAME}_demo_dynamic PRIVATE
  igraphic
  iaudio
  modulemanager
  ${CMAKE_DL_LIBS}
)

# statique : utilitaires + vendor + contrats. Ni modulemanager ni ${CMAKE_DL_LIBS}
# dans cette liste - c'est la difference, et elle se lit ici. Les include
# dirs et les frameworks viennent de la cible statique elle-meme, plus rien
# a recopier.
add_executable(${PROJECT_NAME}_demo_static examples/static.cpp)

target_include_directories(${PROJECT_NAME}_demo_static PRIVATE examples)
target_compile_definitions(${PROJECT_NAME}_demo_static PRIVATE
  ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)
target_link_libraries(${PROJECT_NAME}_demo_static PRIVATE ${PROJECT_NAME}_static)

#----------- tests -----------

find_package(GoogleTest REQUIRED)

enable_testing()

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
add_dependencies(${PROJECT_NAME}_tests ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME}_tests PRIVATE
  RAYLIB_IMPL_PATH="$<TARGET_FILE:${PROJECT_NAME}>"
  ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

target_link_libraries(${PROJECT_NAME}_tests
	gtest_main
  igraphic
  iaudio
  modulemanager
  ${CMAKE_DL_LIBS}
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
