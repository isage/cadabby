#include <psp2kern/kernel/cpu.h>
#include <psp2kern/kernel/debug.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/threadmgr/thread.h>
#include <psp2kern/kernel/suspend.h>
#include <psp2kern/lowio/i2c.h>
#include <psp2kern/idstorage.h>
#include <psp2kern/power.h>
#include <psp2kern/io/fcntl.h>
#include <psp2kern/io/stat.h>
#include <taihen.h>
#include <string.h>

int ksceSysconBatteryStartBLMode(void);
int ksceSysconBatteryStopBLMode(void);
int ksceSysconBatterySWReset(void);

int ksceSysconBatterySetBLCommand(SceUInt16 cmdNumber,SceUInt8 offset,void *data,int size);
int ksceSysconBatteryExecBLCommand(SceUInt16 cmdNumber);
int ksceSysconBatteryReadBLCommand(SceUInt16 cmdNumber,SceUInt8 addr,SceUInt8 reg,void *data,SceUInt32 size);

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(int args, void *argv) {
  (void)args;
  (void)argv;
  ksceDebugPrintf("loaded\n");
  ksceDebugPrintf("starting dump\n");

    int fp = ksceIoOpen("ux0:/data/abbyfw.bin", SCE_O_WRONLY | SCE_O_CREAT, 0777);

    int ret = ksceSysconBatteryStartBLMode();
    ksceDebugPrintf("ksceSysconBatteryStartBLMode = 0x%08x\n", ret);
    ksceKernelDelayThread(1000000);

    uint16_t cmdnum = 1;
    for(int i = 0; i < 512; ++i)
    {

        uint8_t cmd[3] = {0x16, 0x00, 0x00}; // read fw row cmd
        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 3);
//        ksceDebugPrintf("ksceSysconBatterySetBLComand = 0x%08x\n", ret);
        ksceKernelDelayThread(2000);
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
//        ksceDebugPrintf("ksceSysconBatteryExecBLComand = 0x%08x\n", ret);
        cmdnum++;
        ksceKernelDelayThread(2000);

        uint8_t rowcmd[4] = {0x16, 0x01, 0x00, 0x00}; // set row
        rowcmd[2] = i & 0xFF;
        rowcmd[3] = (i >> 8) & 0xFF;

        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, rowcmd, 4);
//        ksceDebugPrintf("ksceSysconBatterySetBLComand = 0x%08x\n", ret);
        ksceKernelDelayThread(2000);
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
//        ksceDebugPrintf("ksceSysconBatteryExecBLComand = 0x%08x\n", ret);
        cmdnum++;
        ksceKernelDelayThread(2000);

        uint16_t checksum = ((i & 0xFF) + ((i >> 8) & 0xFF)) & 0xFFFF;

        uint8_t checksumcmd[4] = {0x16, 0x64, 0x00, 0x00}; // checksum
        checksumcmd[2] = checksum & 0xFF;
        checksumcmd[3] = (checksum >> 8) & 0xFF;

        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, checksumcmd, 4);
//        ksceDebugPrintf("ksceSysconBatterySetBLComand = 0x%08x\n", ret);
        ksceKernelDelayThread(2000);
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
//        ksceDebugPrintf("ksceSysconBatteryExecBLComand = 0x%08x\n", ret);
        cmdnum++;
        ksceKernelDelayThread(2000);

        // can't read more that 16 at a time
        for (int offset = 0; offset < 96; offset+=16)
        {
            uint8_t data[16] = {0};
            ret = ksceSysconBatteryReadBLCommand(cmdnum, 0x16, 0x04+offset, data, 16);
//           ksceDebugPrintf("ksceSysconBatteryReadBLComand = 0x%08x\n", ret);
            cmdnum++;
            ksceIoWrite(fp, data, 16);
            ksceKernelDelayThread(2000);
        }
    }

    ksceIoClose(fp);

    ret = ksceSysconBatteryStopBLMode();
    ksceDebugPrintf("ksceSysconBatteryStopBLMode = 0x%08x\n", ret);

    ksceKernelDelayThread(4000000);

    ret = ksceSysconBatterySWReset();
    ksceDebugPrintf("ksceSysconBatterySWReset = 0x%08x\n", ret);
    ksceKernelDelayThread(4000000);

  ksceDebugPrintf("done\n");

  return SCE_KERNEL_START_SUCCESS;
}

int module_stop()
{
  return SCE_KERNEL_STOP_SUCCESS;
}
