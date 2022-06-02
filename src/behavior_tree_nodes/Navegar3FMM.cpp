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

#include "behavior_tree/Navegar3FMM.h"


namespace behavior_trees
{
Navegar3FMM::Navegar3FMM(const std::string& name, const BT::NodeConfiguration & config):
BT::ActionNodeBase(name, config), nh_(), feedBack(" ")
{
  activador = nh_.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 10);
  resetPub = nh_.advertise<std_msgs::Bool>("/reset_data", 10);
  sub = nh_.subscribe("/move_base/result", 10, &Navegar3FMM::messageCallback, this);

     //P1
    positions[0].header.stamp = i;
    positions[0].header.frame_id = "map";

    positions[0].pose.position.x = 5;
    positions[0].pose.position.y = 0;
    positions[0].pose.orientation.x = 0;
    positions[0].pose.orientation.y = 0;
    positions[0].pose.orientation.z = 0;
    positions[0].pose.orientation.w = 1;


    //P2
    positions[1].header.stamp = i;
    positions[1].header.frame_id = "map";

    positions[1].pose.position.x = 6.8;
    positions[1].pose.position.y = 4;

    positions[1].pose.orientation.x = 0;
    positions[1].pose.orientation.y = 0;
    positions[1].pose.orientation.z = 0.382;
    positions[1].pose.orientation.w = 0.923;

     //P3
    positions[2].header.stamp = i;
    positions[2].header.frame_id = "map";

    positions[2].pose.position.x = 1.3;
    positions[2].pose.position.y = 6;
    positions[2].pose.orientation.x = 0;
    positions[2].pose.orientation.y = 0;
    positions[2].pose.orientation.z = 0.707;
    positions[2].pose.orientation.w = 0.707;

}

void Navegar3FMM::messageCallback(const move_base_msgs::MoveBaseActionResult::ConstPtr& msg)
{
  feedBack = msg->status.text;
  std::cout << "Resultado Navegacion : " << feedBack << "\n";
}

void Navegar3FMM::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus Navegar3FMM::tick()
{

  if (a == 0){
    feedBack = "";
  }

  if (a == 5)
  {
    std::cout << "Navegando a posicion " << counter << "\n";

    geometry_msgs::PoseStamped msg;
    all = counter == size;
    if (all){
      reset.data = false;
      resetPub.publish(reset);
      return BT::NodeStatus::FAILURE;

    }
    msg = positions[counter];
    activador.publish(msg);
  }
  a++;

  if (feedBack != "")
  {
    if (feedBack == "Goal reached.")
    {
      a = 0;
      counter++;
      feedBack = "";
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
  factory.registerNodeType<behavior_trees::Navegar3FMM>("Navegar3FMM");
}
