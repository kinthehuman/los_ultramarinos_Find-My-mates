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
  src/find_my_mates/DialogInterfaceFMM.cpp
)

list(APPEND plugin_libs
  
)

add_library(NavegarFMM SHARED src/behavior_tree_nodes/NavegarFMM.cpp)
add_library(Navegar2FMM SHARED src/behavior_tree_nodes/Navegar2FMM.cpp)
add_library(Navegar3FMM SHARED src/behavior_tree_nodes/Navegar3FMM.cpp)
add_library(ObservarFMM SHARED src/behavior_tree_nodes/ObservarFMM.cpp)
add_library(DataCheckFMM SHARED src/behavior_tree_nodes/DataCheckFMM.cpp)
add_library(DataDumpFMM SHARED src/behavior_tree_nodes/DataDumpFMM.cpp)
add_library(ControlDumpFMM SHARED src/behavior_tree_nodes/ControlDumpFMM.cpp)
add_library(ControlDataFMM SHARED src/behavior_tree_nodes/ControlDataFMM.cpp)
add_library(ControlObservadorFMM SHARED src/behavior_tree_nodes/ControlObservadorFMM.cpp)



add_dependencies(find_my_mates ${catkin_EXPORTED_TARGETS} sound_play_generate_messages_cpp)
target_link_libraries(find_my_mates ${catkin_LIBRARIES})

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/robocup-home-education-los-ultramarinos.cpp
# )

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide

add_executable(ObservadorPersonasFMM src/find_my_mates/ObservadorPersonasFMM.cpp)
add_executable(data_collectorFMM src/find_my_mates/data_collectorFMM.cpp)
add_executable(TextToSpeechFMM src/find_my_mates/TextToSpeechFMM.cpp)
add_executable(Find_My_Mates src/Find_My_Mates.cpp)

target_link_libraries( ObservadorPersonasFMM
${catkin_LIBRARIES}
)

target_link_libraries( data_collectorFMM
${catkin_LIBRARIES}
)

target_link_libraries( TextToSpeechFMM
${catkin_LIBRARIES}
${PROJECT_NAME}
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
  NavegarFMM
  Navegar2FMM
  Navegar3FMM
  ObservarFMM
  DataCheckFMM
  DataDumpFMM
  ControlDataFMM
  ControlDumpFMM
  ControlObservadorFMM
)

foreach(bt_plugin ${plugin_libs})
  target_link_libraries(${bt_plugin} ${catkin_LIBRARIES} find_my_mates)
  target_compile_definitions(${bt_plugin} PRIVATE BT_PLUGIN_EXPORT)
endforeach()


