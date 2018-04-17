/*******************************************************************************
* Copyright (c) 2018 RoboTICan
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the disclaimer
* below) provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*
*     * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*
*     * Neither the name of the copyright holder nor the names of its
*     contributors may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
* THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
* IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include <ros/ros.h>
#include <bms_interface/bms_interface.h>

#define BMS_PORT "/dev/armadillo2/BMS"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "bms_node");
    ros::NodeHandle nh;

    bms::BMSInterface bms;
    try
    {
        bms.connect(BMS_PORT);
    }
    catch (bms::BMSException exp)
    {
        ROS_ERROR("[bms_test]: %s", exp.what());
        //ros::shutdown();
    }

    ros::Rate loop_rate(2);


    while (ros::ok())
    {

        try
        {
            bms.read();
        }
        catch(bms::BMSErrorException exp)
        {
            ROS_ERROR("[bms_test]: %s", exp.what());
            ros::shutdown();
            exit(EXIT_FAILURE);
        }
        catch(bms::BMSWarnException exp)
        {
            ROS_WARN("[bms_test]: %s", exp.what());
        }


        loop_rate.sleep();
        ros::spinOnce();
    }

}