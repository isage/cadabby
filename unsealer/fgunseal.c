#include <psp2kern/kernel/debug.h>
#include <psp2kern/kernel/modulemgr.h>
#include <taihen.h>
#include <string.h>

// NIDs:
// sceSysconBatteryReadRegForDriver: 0xC2FB5565
// sceSysconBatteryWriteRegForDriver: 0x9B779DB0
// sceSysconBatteryControlCommandForDriver: 0xA5AB19B1

int sceSysconBatteryReadRegForDriver(SceUInt16 reg,SceUInt16 *data);
int sceSysconBatteryControlCommandForDriver(SceUInt16 reg,SceUInt16 *data);
int sceSysconBatteryWriteRegForDriver(SceUInt16 reg,SceUInt16 data);

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(int args, void *argv) {
  (void)args;
  (void)argv;
  ksceDebugPrintf("unseal loaded, gotta poof\n");

  SceUInt16 data = 0;

  int ret = sceSysconBatteryControlCommandForDriver(0x0000,&data);
  ksceDebugPrintf("reg: 0x0000, ret: 0x%08X, data: 0x%04X\n", ret, data);

  // unseal keys
  sceSysconBatteryWriteRegForDriver(0x00, 0x1404);
  sceSysconBatteryWriteRegForDriver(0x00, 0x7236);

  data = 0;
  ret = sceSysconBatteryControlCommandForDriver(0x0000,&data);
  ksceDebugPrintf("reg: 0x0000, ret: 0x%08X, data: 0x%04X\n", ret, data);

  // full access keys
  sceSysconBatteryWriteRegForDriver(0x00, 0xFFFF);
  sceSysconBatteryWriteRegForDriver(0x00, 0xFFFF);

  data = 0;
  ret = sceSysconBatteryControlCommandForDriver(0x0000,&data);
  ksceDebugPrintf("reg: 0x0000, ret: 0x%08X, data: 0x%04X\n", ret, data);

  return SCE_KERNEL_START_SUCCESS;
}

int module_stop()
{
  return SCE_KERNEL_STOP_SUCCESS;
}
