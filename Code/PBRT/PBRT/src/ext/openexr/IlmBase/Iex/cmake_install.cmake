# Install script for directory: E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/pbrt-v3_clone/src/ext/openexr/IlmBase/Iex

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/PBRT-V3")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/PBRT/src/ext/openexr/IlmBase/Iex/Debug/Iex.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/PBRT/src/ext/openexr/IlmBase/Iex/Release/Iex.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/PBRT/src/ext/openexr/IlmBase/Iex/MinSizeRel/Iex.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/PBRT/src/ext/openexr/IlmBase/Iex/RelWithDebInfo/Iex.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/pbrt-v3_clone/src/ext/openexr/IlmBase/Iex/IexBaseExc.h"
    "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/pbrt-v3_clone/src/ext/openexr/IlmBase/Iex/IexMathExc.h"
    "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/pbrt-v3_clone/src/ext/openexr/IlmBase/Iex/IexThrowErrnoExc.h"
    "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/pbrt-v3_clone/src/ext/openexr/IlmBase/Iex/IexErrnoExc.h"
    "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/pbrt-v3_clone/src/ext/openexr/IlmBase/Iex/IexMacros.h"
    "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/pbrt-v3_clone/src/ext/openexr/IlmBase/Iex/Iex.h"
    "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/pbrt-v3_clone/src/ext/openexr/IlmBase/Iex/IexNamespace.h"
    "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/pbrt-v3_clone/src/ext/openexr/IlmBase/Iex/IexExport.h"
    "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/pbrt-v3_clone/src/ext/openexr/IlmBase/Iex/IexForward.h"
    )
endif()
