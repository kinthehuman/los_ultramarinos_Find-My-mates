// Copyright 2022 los ultramarinos
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

#include <string>

#include "behavior_tree/NavegarFMM.h"

namespace behavior_trees
{
NavegarFMM::NavegarFMM(const std::string& name, const BT::NodeConfiguration & config):
BT::ActionNodeBase(name, config), nh_(), feedBack(" ")
{
  activador = nh_.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 10);
  sub = nh_.subscribe("/move_base/result", 10, &NavegarFMM::messageCallback, this);
}

void NavegarFMM::messageCallback(const move_base_msgs::MoveBaseActionResult::ConstPtr& msg)
{
  feedBack = msg->status.text;
  std::cout << "Resultado Navegacion : " << feedBack << "\n";
}

void NavegarFMM::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus NavegarFMM::tick()
{
  if (a == 0){
    feedBack = "";
  }
  if (a == 20)
  {
    std::cout << a << "\n";

    geometry_msgs::PoseStamped msg;

    msg.header.stamp = i;
    msg.header.frame_id = "map";

    msg.pose.position.x = 0.3;
    msg.pose.position.y = 5.65;
    msg.pose.position.z = 0.0;

    msg.pose.orientation.x = 0.0;
    msg.pose.orientation.y = 0.0;
    msg.pose.orientation.z = 0.7;
    msg.pose.orientation.w = -0.7;

    activador.publish(msg);
  }
  a++;

  if (feedBack != "")
  {
    if (feedBack == "Goal reached.")
    {
      return BT::NodeStatus::SUCCESS;
    }
    else
    {
      return BT::NodeStatus::FAILURE;
    }
  }
  return BT::NodeStatus::RUNNING;
}
}  // namespace behavior_trees

BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::NavegarFMM>("NavegarFMM");
}
