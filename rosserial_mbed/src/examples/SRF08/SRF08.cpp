/*
 * rosserial SRF08 Ultrasonic Ranger Example
 *
 * This tutorial demonstrates the usage of SRF08 ultrasonic module
 *
 * Source Code Based on:
 * http://wiki.ros.org/rosserial_arduino/Tutorials/SRF08%20Ultrasonic%20Range%20Finder
 */

#include "mbed.h"
#include "SRF08.h"

#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

ros::NodeHandle  nh;

sensor_msgs::Range range_msg;
ros::Publisher pub_range("/ultrasound", &range_msg);

char frameid[] = "/ultrasound";

#ifdef TARGET_LPC1768
SRF08 srf08(p9, p10, 0xE0);             // Define SDA, SCL pin and I2C address
#elif defined(TARGET_KL25Z) || defined(TARGET_NUCLEO_F401RE)
SRF08 srf08(I2C_SDA, I2C_SCL, 0xE0);    // Define SDA, SCL pin and I2C address
#else
#error "You need to specify a pin for the sensor"
#endif

Timer t;
int main()
{
  t.start();
  nh.initNode();
  nh.advertise(pub_range);

  range_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
  range_msg.header.frame_id =  frameid;
  range_msg.field_of_view = 0.1;  // fake
  range_msg.min_range = 0.0;
  range_msg.max_range = 6.47;

  long range_time = 0;

  while (1)
  {
    //publish the adc value every 50 milliseconds
    //since it takes that long for the sensor to stablize
    if (t.read_ms() >= range_time)
    {
      range_msg.range = srf08.read() / 100.0;
      range_msg.header.stamp = nh.now();
      pub_range.publish(&range_msg);
      range_time =  t.read_ms() + 50;
    }

    nh.spinOnce();
  }
}
