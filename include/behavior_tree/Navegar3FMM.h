// Copyright 2019 Intelligent Robotics Lab
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef BEHAVIOR_TREES_NAVEGAR3FMM_H
#define BEHAVIOR_TREES_NAVEGAR3FMM_H

#include "behaviortree_cpp_v3/behavior_tree.h"
#include "behaviortree_cpp_v3/bt_factory.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PoseStamped.h"
#include "move_base_msgs/MoveBaseActionResult.h"

#include <string>
#include "ros/ros.h"

namespace behavior_trees
{

class Navegar3FMM : public BT::ActionNodeBase
{
  public:

    explicit Navegar3FMM(const std::string& name , const BT::NodeConfiguration& config);

    void halt();

    BT::NodeStatus tick();

    void messageCallback(const move_base_msgs::MoveBaseActionResult::ConstPtr& msg);


    
    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<std::string>("object")};
    }


  private:
    ros::NodeHandle nh_;
    ros::Publisher activador ;
    ros::Subscriber sub ;
    ros::Publisher resetPub;
    ros::Publisher dataPub;
    std::string feedBack = "" ;
    
    ros::Time i;
    int a = 0;
    int size = 6;
    geometry_msgs::PoseStamped positions[6];
    int counter = 0;
    std_msgs::Bool reset;
    std_msgs::Int32 result;
    bool all;


};

}  // namespace behavior_trees

#endif  // BEHAVIOR_TREES_Navegar3_H
