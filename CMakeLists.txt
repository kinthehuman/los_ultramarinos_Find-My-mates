cmake_minimum_required(VERSION 3.0.2)
project(find_my_mates)

add_compile_options(-std=c++17)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  dialogflow_ros_msgs
  roslint
  std_srvs
  std_msgs
  sound_play
  actionlib
  actionlib_msgs
  geometry_msgs 
  message_generation 
  cv_bridge 
  image_transport
  tf
  behaviortree_cpp_v3
)

add_message_files(
  FILES
  Info.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
	INCLUDE_DIRS include
	LIBRARIES find_my_mates behaviortree_cpp_v3
	CATKIN_DEPENDS roscpp dialogflow_ros_msgs actionlib_msgs message_runtime
	DEPENDS
)

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${ZMQ_INCLUDE_DIRS}
)

link_directories(
	${PCL_LIBRARY_DIRS}
)

add_definitions(${PCL_DEFINITIONS})

add_library(find_my_mates
  src/find_my_mates/DialogInterface.cpp
)

list(APPEND plugin_libs
  
)

add_library(TextToSpeech SHARED src/behavior_tree_nodes/TextToSpeech.cpp)
add_library(Navegar SHARED src/behavior_tree_nodes/Navegar.cpp)
add_library(Navegar2 SHARED src/behavior_tree_nodes/Navegar2.cpp)
add_library(Navegar3 SHARED src/behavior_tree_nodes/Navegar3.cpp)
add_library(Observar SHARED src/behavior_tree_nodes/Observar.cpp)
add_library(DataCheck SHARED src/behavior_tree_nodes/DataCheck.cpp)
add_library(DataDump SHARED src/behavior_tree_nodes/DataDump.cpp)


add_dependencies(find_my_mates ${catkin_EXPORTED_TARGETS} sound_play_generate_messages_cpp)
target_link_libraries(find_my_mates ${catkin_LIBRARIES})

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/robocup-home-education-los-ultramarinos.cpp
# )

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide

add_executable(ObservadorPersonas_rc src/find_my_mates/ObservadorPersonas.cpp)
add_executable( data_collector src/find_my_mates/data_collector.cpp )
add_executable( Find_My_Mates src/Find_My_Mates.cpp )

target_link_libraries( ObservadorPersonas_rc
${catkin_LIBRARIES}
)

target_link_libraries( data_collector
${catkin_LIBRARIES}
)

target_link_libraries( Find_My_Mates
${catkin_LIBRARIES}
)






## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
# )


add_executable(Monologo_node src/Find_My_Mates.cpp)


add_dependencies(Monologo_node ${catkin_EXPORTED_TARGETS} )


target_link_libraries(Monologo_node ${catkin_LIBRARIES} find_my_mates ${ZMQ_LIBRARIES})


list(APPEND plugin_libs
  TextToSpeech
  Navegar
  Navegar2
  Navegar3
  Observar
  DataCheck
  DataDump
)

foreach(bt_plugin ${plugin_libs})
  target_link_libraries(${bt_plugin} ${catkin_LIBRARIES} find_my_mates)
  target_compile_definitions(${bt_plugin} PRIVATE BT_PLUGIN_EXPORT)
endforeach()

