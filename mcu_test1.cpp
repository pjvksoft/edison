#include "mcu_api.h"
#include "mcu_errno.h"

/* the I2C address of MPU6050 */
#define MPU6050_ADDR    0x68
/* the register address of WHOAMI register, which is used to verify 
the identity of the device */
#define MPU6050_WHOAMI          ((unsigned char)0x75)
#define MPU6050_WHOAMI_VALUE    ((unsigned char)0x68)

static unsigned char mpu6050_read_whoami()
{
    unsigned char id;
    int res;
    res = i2c_read(MPU6050_ADDR, MPU6050_WHOAMI, &id, 1);
    if (res)
    {
        debug_print(DBG_ERROR, "mpu6050_i2c_read whoami fail...");
        return 0Xff;
    }
    if (id != MPU6050_WHOAMI_VALUE)
    {
        debug_print(DBG_ERROR, "wrong chip ID 0x%x, expected 0x%x\n", 
                    id, MPU6050_WHOAMI_VALUE);
        return 0xff;
    }
    return id;
}

void mcu_main()
{
    unsigned char device_id;
    char buf[16];
    int len;
    while (1)
    {
        mcu_sleep(300);
        debug_print(DBG_INFO, "after sleep 3 seconds\n");
        device_id = mpu6050_read_whoami();
        len = mcu_snprintf(buf, 16, "id = %d\n", device_id);
        host_send((unsigned char*)buf, len);
    }
}