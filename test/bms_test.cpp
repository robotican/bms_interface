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

int main(int argc, char **argv)
{
    ros::init(argc, argv, "bms_test_node");
    ros::NodeHandle nh;

    std::string bms_port;
    if (!ros::param::get("~port", bms_port))
    {
        ROS_ERROR("[bms_test]: port param is missing on param server. make sure that this "
                          "param exist in bms_test.launch file. shutting down...\n");
        ros::shutdown();
        exit (EXIT_FAILURE);
    }

    bms::BMSInterface bms;
    ROS_INFO("[bms_test]: Trying to connect to tty port: %s ...\n", bms_port.c_str());
    try
    {
        bms.connect(bms_port);
    }
    catch (bms::BMSException exp)
    {
        ROS_ERROR("[bms_test]: %s\nShutting down...", exp.what());
        exit(EXIT_FAILURE);
    }

    ROS_INFO("[bms_test]: Connected to %s\n", bms_port.c_str());

    // read data at 2 hz. Changing rate might
    // cause communication problems with BMS.
    ros::Rate loop_rate(2);
    bms::data bms_data;
    while (ros::ok())
    {

        try
        {
            ROS_INFO("\n[bms_test]: Trying to read from BMS ...\n");
            bms_data = bms.read();
        }
        catch(bms::BMSErrorException exp)
        {
            ROS_ERROR("[bms_test]: %s\n", exp.what());
            ros::shutdown();
            exit(EXIT_FAILURE);
        }
        catch(bms::BMSWarnException exp)
        {
            ROS_WARN("[bms_test]: %s\n", exp.what());
        }

        ROS_INFO("\n[bms_test]: Read succesfull");
        ROS_INFO("[bms_test]: Percentage: %u", bms_data.soc);
        ROS_INFO("[bms_test]: Charging Current: %f", bms_data.chrg_current);
        ROS_INFO("[bms_test]: Discharging Current: %f", bms_data.dchrg_current);
        ROS_INFO("[bms_test]: Voltage (Ah): %f", bms_data.vbat);
        ROS_INFO("[bms_test]: Capacity (Ah): %d", bms_data.cap_full);
        ROS_INFO("[bms_test]: Is Charging? %s", bms_data.is_chrg ? "true" : "false");
        ROS_INFO("[bms_test]: Cells:");
        for (const float vcell : bms_data.vcells)
        {
            printf("| %f |", vcell);
        }
        printf("\n");
        loop_rate.sleep();
        ros::spinOnce();
    }

}