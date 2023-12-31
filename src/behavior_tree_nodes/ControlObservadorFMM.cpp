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

#include "behavior_tree/ControlObservadorFMM.h"

namespace behavior_trees
{

ControlObservadorFMM::ControlObservadorFMM(const std::string& name , const BT::NodeConfiguration & config):
BT::ActionNodeBase(name, config), nh_(), feedBack(true)
{
  sub = nh_.subscribe<std_msgs::Bool>("/reset_observador", 10, &ControlObservadorFMM::messageCallback, this);
}

void ControlObservadorFMM::messageCallback(const std_msgs::Bool::ConstPtr& msg)
{
  feedBack = msg->data;
  if(feedBack){
    std::cout << "Listo para observar \n";
  }
  else{
    std::cout << "Observacion finalizada \n";
  }
  //std::cout << msg->data;
}

void ControlObservadorFMM::halt()
{
  //ROS_INFO("Seguir halt");
  //std_msgs::Bool act;
  //act.data = false;
  //activador.publish(act);
}

BT::NodeStatus ControlObservadorFMM::tick()
{

  if (feedBack)
  {
    return BT::NodeStatus::FAILURE;
  }
  else
  {
    return BT::NodeStatus::SUCCESS;
  }
}
}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::ControlObservadorFMM>("ControlObservadorFMM");
}