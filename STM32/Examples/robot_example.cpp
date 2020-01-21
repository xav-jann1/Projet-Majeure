#include "main.h"
#include "mainpp.h"
#include "robot_VelocityJoint.h"
#include "robot_node.h"
#include "robot_joint.h"
#include "robot_config.h"
#include "ros.h"

// VelocityJoint:
robot::VelocityJoint wheel_l_joint(wheel_l_defs);
robot::VelocityJoint wheel_r_joint(wheel_r_defs);

// Node:
ros::NodeHandle nh;

// Loop delays:
int t = LOOP_JOINTS_MS + 1, t1 = LOOP_DATA_MS + 1;

// Setup node:
void setup(void) {
  // Joint:
  robot::initJoint(wheel_l_joint, nh);
  robot::initJoint(wheel_r_joint, nh);

  // Node:
  robot::initNode(nh);
  nh.loginfo("STM32 Connecté !");

  // Allume LED:
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  t = t1 = 0;
}

// Loop:
void loop() {
  nh.spinOnce();
}

// Loop 1 ms:
void HAL_SYSTICK_Callback() {
  // Mise à jour des joints:
  if (++t == LOOP_JOINTS_MS) {
    t = 0;
    wheel_l_joint.update();
    wheel_r_joint.update();
    robot::publishJointsData();
  }

  // Publie des données:
  if (++t1 == LOOP_DATA_MS) {
    t1 = 0;

    wheel_l_joint.publishData();
    wheel_r_joint.publishData();

    //wheel_l_joint.publishEncoderData();
    //wheel_r_joint.publishEncoderData();

    //wheel_l_joint.publishPidValues();
    //wheel_r_joint.publishPidValues();
  }
}