#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(mrpc)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()


SET(CMAKE_MODULE_PATH /home/fuyue/zzz/Software/ROOT/root-6.10.06/etc/cmake)
#SET(CMAKE_MODULE_PATH /usr/common/usg/software/ROOT/6.08.00/etc/root/cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wno-unused-variable -pthread")
find_package(ROOT REQUIRED)
#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Add option to control storage 
#
#option(G4STORE_DATA "Stores output data" OFF)

#if(G4STORE_DATA)
  #add_definitions(-DG4STORE_DATA)
#endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${ROOT_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(mrpc mrpcsimulation.cc ${sources} ${headers})
target_link_libraries(mrpc ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})


#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build gammaray_telescope. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(mrpc_SCRIPTS
    vis.mac
   run1.mac
    run2.mac
   GasData.txt
    )

foreach(_script ${mrpc_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
#add_custom_target(gammaray_telescope DEPENDS GammaRayTel)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS mrpc DESTINATION bin)

