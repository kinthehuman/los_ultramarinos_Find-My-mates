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

#include "behavior_tree/ObservarFMM.h"

namespace behavior_trees
{

ObservarFMM::ObservarFMM(const std::string& name , const BT::NodeConfiguration & config): BT::ActionNodeBase(name, config),nh_(),feedBack(" ")
{   
  activador = nh_.advertise<std_msgs::Bool>("/control_observador",10);
  controlPub = nh_.advertise<std_msgs::Bool>("/reset_observador", 10);

  sub = nh_.subscribe("/status_observador", 10, &ObservarFMM::messageCallback, this); 
  failures = 0;
}


void ObservarFMM::messageCallback(const std_msgs::String::ConstPtr& msg)
{
  feedBack = msg->data;
  //std::cout << msg->data;
}


void
ObservarFMM::halt()
{
  ROS_INFO("Observar halt");
}

BT::NodeStatus
ObservarFMM::tick()
{
if (a == 0){
  feedBack = "";
}
if (a == 5){
  act.data = true;
  std::cout << "activando observador \n";
  activador.publish(act);
  }
  a++;

  if (feedBack == "SUCCESS") {
    act.data = false;
		controlPub.publish(act);
    activador.publish(act);
    failures = 0;
    a = 0;
    return BT::NodeStatus::SUCCESS;
  }
  else if (feedBack == "FAILURE") {
      failures++;
      if(failures > 50){
        act.data = false;
        activador.publish(act);
        failures = 0;
        a = 0;
        return BT::NodeStatus::FAILURE;   
        }
      else{
         return BT::NodeStatus::RUNNING;
      }
  }
  else{
     return BT::NodeStatus::RUNNING;

  }   

}
}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::ObservarFMM>("ObservarFMM");
}