#include "sky_mavlink.h"
#include "mavlink.h"

#define MAVLINK_MEMPOOL_BLOCK_SIZE (256)
#define BUFFER_LENGTH 2041 // minimum buffer size that can be used with qnx (I don't know why)

//unsigned char *tx_buf = RT_NULL;

uint8_t buf[BUFFER_LENGTH];  //用于存放发送的原始数据

mavlink_system_t mavlink_system;

void mavlink_init(void)
{
	mavlink_system.sysid = 20;                   ///< ID 20 for this airplane
	mavlink_system.compid = MAV_COMP_ID_IMU;     ///< The component sending the message is the IMU, it could be also a Linux process
	mavlink_system.type = MAV_TYPE_QUADROTOR;    ///< This system is an quadcopter
}


/*Send Heartbeat */
void mavlink_send_heartbeat()
{
	mavlink_message_t msg;
	mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_HELICOPTER, MAV_AUTOPILOT_GENERIC, 
		MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);//打包心跳数据，心跳用于指示是否连接正常
	int len = mavlink_msg_to_send_buffer(buf, &msg);
}
		