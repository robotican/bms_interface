# bms_interface
ROS driver package for Dantech Battery Management System

## Prerequisites
Ubuntu 16.04 and ROS kinetic installed

## Running test/demo
find BMS port with ls /dev/ and in bms_test.launch file, edit port param accordinglly.
type in command line
`
$ roslaunch bms_interface bms_test.launch 
`
