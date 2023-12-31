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

#include "behavior_tree/DataCheckFMM.h"

namespace behavior_trees
{

DataCheckFMM::DataCheckFMM(const std::string& name , const BT::NodeConfiguration & config):
BT::ActionNodeBase(name, config), nh_(), feedBack(" ")
{
  activador = nh_.advertise<std_msgs::Int32>("/control_data", 10);
  sub = nh_.subscribe<std_msgs::String>("/status_data", 10, &DataCheckFMM::messageCallback, this);
}

void DataCheckFMM::messageCallback(const std_msgs::String::ConstPtr& msg)
{
  feedBack = msg->data;
  //std::cout << msg->data;
}

void DataCheckFMM::halt()
{
  //ROS_INFO("Seguir halt");
  //std_msgs::Bool act;
  //act.data = false;
  //activador.publish(act);
}

BT::NodeStatus DataCheckFMM::tick()
{
  if (a == 0){
    feedBack = "";
  }
  std_msgs::Int32 person;
  person.data = person_counter;
  activador.publish(person);
  a++;

  if (feedBack == "FAILURE")
  {
    std::cout << "looking for more people \n";
    person_counter++;
    a = 0;
    return BT::NodeStatus::FAILURE;
  }

  if (feedBack == "SUCCESS")
  {
    std::cout << "done collecting info \n";
    a = 0;
    return BT::NodeStatus::SUCCESS;
    //person_counter++;
  }
  else
  {
    return BT::NodeStatus::RUNNING;
    //person_counter++;
  }
}
}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::DataCheckFMM>("DataCheckFMM");
}
