#if defined(__PCEMU__)
 #include "pcemu.h"

 #define CHECK_PATH "thats_so_magic"
 #define FW_PATH "abbyfw.bin"
 #define DF_PATH "abbydf.bin"
#else
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

 #define CHECK_PATH "ux0:/data/thats_so_magic"
 #define FW_PATH "ux0:/data/abbyfw.bin"
 #define DF_PATH "ux0:/data/abbydf.bin"
#endif
#include <string.h>

// !!!!
// If for whatever reason you need to flash full fw (you normally don't) - set this to 1
// !!!!
#define FLASH_FULL_FW 0

int ksceSysconBatteryStartBLMode(void);
int ksceSysconBatteryStopBLMode(void);
int ksceSysconBatterySWReset(void);

int ksceSysconBatterySetBLCommand(SceUInt16 cmdNumber,SceUInt8 offset,void *data,int size);
int ksceSysconBatteryExecBLCommand(SceUInt16 cmdNumber);
int ksceSysconBatteryReadBLCommand(SceUInt16 cmdNumber,SceUInt8 addr,SceUInt8 reg,void *data,SceUInt32 size);

static uint16_t cmdnum = 1;

int startBLMode()
{
    int ret = ksceSysconBatteryStartBLMode();
    ksceDebugPrintf("ksceSysconBatteryStartBLMode = 0x%08x\n", ret);
    ksceKernelDelayThread(1000000);
    return ret;
}

int stopBLModeAndReset()
{
    int ret = ksceSysconBatteryStopBLMode();
    ksceDebugPrintf("ksceSysconBatteryStopBLMode = 0x%08x\n", ret);
    ksceKernelDelayThread(4000000);

    ret = ksceSysconBatterySWReset();
    ksceDebugPrintf("ksceSysconBatterySWReset = 0x%08x\n", ret);
    ksceKernelDelayThread(4000000);
    return ret;
}

int fwMassErase()
{
  uint8_t cmd[4] = {0};
  int ret = 0;

  // mass-erase fw
  cmd[0] = 0x16;
  cmd[1] = 0x00;
  cmd[2] = 0x04;
  ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 3);
  ksceKernelDelayThread(200);
  ret = ksceSysconBatteryExecBLCommand(cmdnum);
  cmdnum++;
  ksceKernelDelayThread(2000);

  cmd[0] = 0x16;
  cmd[1] = 0x04;
  cmd[2] = 0x83;
  cmd[3] = 0xDE;
  ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
  ksceKernelDelayThread(200);
  ret = ksceSysconBatteryExecBLCommand(cmdnum);
  cmdnum++;
  ksceKernelDelayThread(2000);

  cmd[0] = 0x16;
  cmd[1] = 0x64;
  cmd[2] = 0x65;
  cmd[3] = 0x01;
  ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
  ksceKernelDelayThread(200);
  ret = ksceSysconBatteryExecBLCommand(cmdnum);
  cmdnum++;
  ksceKernelDelayThread(200000);

  uint8_t check = 0;
  ret = ksceSysconBatteryReadBLCommand(cmdnum,0x16,0x66,&check,1);
  cmdnum++;
  if (check != 0) return -1;

  return ret;
}

int fwEraseFirstPage()
{
  uint8_t cmd[4] = {0};
  int ret = 0;

  // erase first two rows of fw
  cmd[0] = 0x16;
  cmd[1] = 0x00;
  cmd[2] = 0x03;
  ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 3);
  ksceKernelDelayThread(200);
  ret = ksceSysconBatteryExecBLCommand(cmdnum);
  cmdnum++;
  ksceKernelDelayThread(2000);

  if (ret != 0) return ret;

  cmd[0] = 0x16;
  cmd[1] = 0x01;
  cmd[2] = 0x00;
  cmd[3] = 0x00;
  ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
  ksceKernelDelayThread(200);
  ret = ksceSysconBatteryExecBLCommand(cmdnum);
  cmdnum++;
  ksceKernelDelayThread(2000);

  if (ret != 0) return ret;

  cmd[0] = 0x16;
  cmd[1] = 0x64;
  cmd[2] = 0x03;
  cmd[3] = 0x00;
  ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
  ksceKernelDelayThread(200);
  ret = ksceSysconBatteryExecBLCommand(cmdnum);
  cmdnum++;
  ksceKernelDelayThread(200000);

  if (ret != 0) return ret;

  uint8_t check = 0;
  ret = ksceSysconBatteryReadBLCommand(cmdnum,0x16,0x66,&check,1);
  cmdnum++;
  if (check != 0) return -1;

  return ret;
}

int fwFlash(const char* filename)
{
    uint8_t cmd[4] = {0};
    int ret = 0;
    int fp_fw = ksceIoOpen(filename, SCE_O_RDONLY, 0777);

    // start writing, skip first two rows (we'll write them after df if we have one)
    ksceIoSeek(fp_fw, 96*2, SCE_SEEK_SET);

    for (int row = 2; row < 512; ++row)
    {
        uint8_t fwrow[96] = {0};
        ksceIoRead(fp_fw, fwrow, 96);
        // write if row cmd
        cmd[0] = 0x16;
        cmd[1] = 0x00;
        cmd[2] = 0x02;
        cmd[3] = 0x00;
        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 3);
        ksceKernelDelayThread(200);
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
        cmdnum++;
        ksceKernelDelayThread(200000);
        if (ret != 0) return ret;

        // set row
        cmd[0] = 0x16;
        cmd[1] = 0x01;
        cmd[2] = row & 0xFF;
        cmd[3] = (row >> 8) & 0xFF;
        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
        ksceKernelDelayThread(200);
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
        cmdnum++;
        ksceKernelDelayThread(200000);
        if (ret != 0) return ret;

        // write row
        cmd[0] = 0x16;
        cmd[1] = 0x04;
        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 2);
        // 16 bytes is a limit for SetCommand, so split
        for (int i = 0; i< 6; ++i)
        {
            ret = ksceSysconBatterySetBLCommand(cmdnum, 2+i*16, fwrow+(i*16), 16);
        }
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
        cmdnum++;
        ksceKernelDelayThread(200000);
        if (ret != 0) return ret;

        // write checksum
        uint32_t checksum = 0x02; // cmd
        checksum += row & 0xFF;
        checksum += (row >> 8) & 0xFF;
        for (int i = 0; i< 96; ++i)
            checksum += fwrow[i];
        cmd[0] = 0x16;
        cmd[1] = 0x64;
        cmd[2] = checksum & 0xFF;
        cmd[3] = (checksum >> 8) & 0xFF;
        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
        ksceKernelDelayThread(200);
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
        cmdnum++;
        ksceKernelDelayThread(200000);
        if (ret != 0) return ret;

        uint8_t check = 0;
        ret = ksceSysconBatteryReadBLCommand(cmdnum,0x16,0x66,&check,1);
        cmdnum++;
        if (check != 0)
        {
            ksceIoClose(fp_fw);
            return -1;
        }
    }
    ksceIoClose(fp_fw);
    return ret;
}

int fwFlashFirstPage(const char* filename)
{
    uint8_t cmd[4] = {0};
    int ret = 0;
    int fp_fw = ksceIoOpen(filename, SCE_O_RDONLY, 0777);
    ksceIoSeek(fp_fw, 0, SCE_SEEK_SET);
    for (int row = 0; row < 2; ++row)
    {
        uint8_t fwrow[96] = {0};
        ksceIoRead(fp_fw, fwrow, 96);

        // write if row cmd
        cmd[0] = 0x16;
        cmd[1] = 0x00;
        cmd[2] = 0x02;
        cmd[3] = 0x00;
        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 3);
        ksceKernelDelayThread(200);
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
        cmdnum++;
        ksceKernelDelayThread(200000);
        if (ret != 0) return ret;

        // set row
        cmd[0] = 0x16;
        cmd[1] = 0x01;
        cmd[2] = row & 0xFF;
        cmd[3] = (row >> 8) & 0xFF;
        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
        ksceKernelDelayThread(200);
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
        cmdnum++;
        ksceKernelDelayThread(200000);
        if (ret != 0) return ret;

        // write row
        cmd[0] = 0x16;
        cmd[1] = 0x04;
        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 2);
        // 16 bytes is a limit for SetCommand, so split
        for (int i = 0; i< 6; ++i)
        {
            ret = ksceSysconBatterySetBLCommand(cmdnum, 2+i*16, fwrow+(i*16), 16);
        }
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
        cmdnum++;
        ksceKernelDelayThread(200000);
        if (ret != 0) return ret;

        // write checksum
        uint32_t checksum = 0x02; // cmd
        checksum += row & 0xFF;
        checksum += (row >> 8) & 0xFF;
        for (int i = 0; i< 96; ++i)
            checksum += fwrow[i];
        cmd[0] = 0x16;
        cmd[1] = 0x64;
        cmd[2] = checksum & 0xFF;
        cmd[3] = (checksum >> 8) & 0xFF;
        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
        ksceKernelDelayThread(200);
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
        cmdnum++;
        ksceKernelDelayThread(200000);
        if (ret != 0) return ret;

        uint8_t check = 0;
        ret = ksceSysconBatteryReadBLCommand(cmdnum,0x16,0x66,&check,1);
        cmdnum++;
        if (check != 0)
        {
            ksceIoClose(fp_fw);
            return -1;
        }
    }
    ksceIoClose(fp_fw);
    return ret;
}

int dfMassErase()
{
  uint8_t cmd[4] = {0};
  int ret = 0;

  // mass-erase df
  cmd[0] = 0x16;
  cmd[1] = 0x00;
  cmd[2] = 0x0C;
  ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 3);
  ksceKernelDelayThread(200);
  ret = ksceSysconBatteryExecBLCommand(cmdnum);
  cmdnum++;
  ksceKernelDelayThread(2000);
  if (ret != 0) return ret;

  cmd[0] = 0x16;
  cmd[1] = 0x04;
  cmd[2] = 0x83;
  cmd[3] = 0xDE;
  ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
  ksceKernelDelayThread(200);
  ret = ksceSysconBatteryExecBLCommand(cmdnum);
  cmdnum++;
  ksceKernelDelayThread(2000);
  if (ret != 0) return ret;

  cmd[0] = 0x16;
  cmd[1] = 0x64;
  cmd[2] = 0x6D;
  cmd[3] = 0x01;
  ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
  ksceKernelDelayThread(200);
  ret = ksceSysconBatteryExecBLCommand(cmdnum);
  cmdnum++;
  ksceKernelDelayThread(200000);
  if (ret != 0) return ret;

  uint8_t check = 0;
  ret = ksceSysconBatteryReadBLCommand(cmdnum,0x16,0x66,&check,1);
  cmdnum++;
  if (check != 0) return -1;

  return ret;
}

int dfFlash(const char* filename)
{
    uint8_t cmd[4] = {0};
    int ret = 0;
    int fp_df = ksceIoOpen(filename, SCE_O_RDONLY, 0777);

    // start writing

    for (int row = 0; row < 32; ++row)
    {
        uint8_t dfrow[32] = {0};
        ksceIoRead(fp_df, dfrow, 32);
        // write df row cmd
        cmd[0] = 0x16;
        cmd[1] = 0x00;
        cmd[2] = 0x0A;
        cmd[3] = 0x00;
        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 3);
        ksceKernelDelayThread(200);
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
        cmdnum++;
        ksceKernelDelayThread(200000);
        if (ret != 0) return ret;

        // set row
        cmd[0] = 0x16;
        cmd[1] = 0x01;
        cmd[2] = row & 0xFF;
        cmd[3] = (row >> 8) & 0xFF;
        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
        ksceKernelDelayThread(200);
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
        cmdnum++;
        ksceKernelDelayThread(200000);
        if (ret != 0) return ret;

        // write row
        cmd[0] = 0x16;
        cmd[1] = 0x04;
        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 2);
        // 16 bytes is a limit for SetCommand, so split
        for (int i = 0; i< 2; ++i)
        {
            ret = ksceSysconBatterySetBLCommand(cmdnum, 2+i*16, dfrow+(i*16), 16);
        }
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
        cmdnum++;
        ksceKernelDelayThread(200000);
        if (ret != 0) return ret;

        // write checksum
        uint32_t checksum = 0x0A; // cmd
        checksum += row & 0xFF;
        checksum += (row >> 8) & 0xFF;
        for (int i = 0; i< 32; ++i)
            checksum += dfrow[i];
        cmd[0] = 0x16;
        cmd[1] = 0x64;
        cmd[2] = checksum & 0xFF;
        cmd[3] = (checksum >> 8) & 0xFF;
        ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
        ksceKernelDelayThread(200);
        ret = ksceSysconBatteryExecBLCommand(cmdnum);
        cmdnum++;
        ksceKernelDelayThread(200000);
        if (ret != 0) return ret;

        uint8_t check = 0;
        ret = ksceSysconBatteryReadBLCommand(cmdnum,0x16,0x66,&check,1);
        cmdnum++;
        if (check != 0)
        {
            ksceIoClose(fp_df);
            return -1;
        }
    }
    ksceIoClose(fp_df);
    return ret;
}

int checkDFChecksum(const char* filename)
{
  uint8_t cmd[4] = {0};
  int ret = 0;

  int fp_df = ksceIoOpen(filename, SCE_O_RDONLY, 0777);
  uint8_t b = 0;
  uint16_t file_checksum = 0;
  while (ksceIoRead(fp_df, &b, 1))
  {
    file_checksum += b;
  }
  ksceIoClose(fp_df);

  // calculate DF checksum
  cmd[0] = 0x16;
  cmd[1] = 0x00;
  cmd[2] = 0x08;
  ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 3);
  ksceKernelDelayThread(200);
  ret = ksceSysconBatteryExecBLCommand(cmdnum);
  cmdnum++;
  ksceKernelDelayThread(2000);
  if (ret != 0) return ret;

  // write cmd checksum
  cmd[0] = 0x16;
  cmd[1] = 0x64;
  cmd[2] = 0x08;
  cmd[3] = 0x00;
  ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
  ksceKernelDelayThread(200);
  ret = ksceSysconBatteryExecBLCommand(cmdnum);
  cmdnum++;
  ksceKernelDelayThread(200000);
  if (ret != 0) return ret;

  uint16_t checksum = 0;
  ret = ksceSysconBatteryReadBLCommand(cmdnum,0x16,0x04,&checksum,2);

  ksceDebugPrintf("DF: checksum: 0x%04X, file checksum: 0x%04X\n", checksum, file_checksum);

  if (checksum != file_checksum) return -1;

  return ret;
}

int checkFWChecksum(const char* filename)
{
  uint8_t cmd[4] = {0};
  int ret = 0;

  int fp_fw = ksceIoOpen(filename, SCE_O_RDONLY, 0777);
  uint8_t b = 0;
  uint16_t file_checksum = 0;
  while (ksceIoRead(fp_fw, &b, 1))
  {
    file_checksum += b;
  }
  ksceIoClose(fp_fw);

  // calculate FW checksum
  cmd[0] = 0x16;
  cmd[1] = 0x00;
  cmd[2] = 0x05;
  ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 3);
  ksceKernelDelayThread(200);
  ret = ksceSysconBatteryExecBLCommand(cmdnum);
  cmdnum++;
  ksceKernelDelayThread(2000);
  if (ret != 0) return ret;

  // write cmd checksum
  cmd[0] = 0x16;
  cmd[1] = 0x64;
  cmd[2] = 0x05;
  cmd[3] = 0x00;
  ret = ksceSysconBatterySetBLCommand(cmdnum, 0, cmd, 4);
  ksceKernelDelayThread(200);
  ret = ksceSysconBatteryExecBLCommand(cmdnum);
  cmdnum++;
  ksceKernelDelayThread(200000);
  if (ret != 0) return ret;

  uint16_t checksum = 0;
  ret = ksceSysconBatteryReadBLCommand(cmdnum,0x16,0x04,&checksum,2);

  ksceDebugPrintf("FW: checksum: 0x%04X, file checksum: 0x%04X\n", checksum, file_checksum);

  if (checksum != file_checksum) return -1;

  return ret;
}

#if defined(__PCEMU__)
int main(int argc, char *argv[]) {
#else
void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(int args, void *argv) {
  (void)args;
  (void)argv;
#endif
  ksceDebugPrintf("loaded\n");
  uint8_t cmd[4] = {0};
  int ret = 0;

  SceIoStat st;
  int stat_magic = ksceIoGetstat(CHECK_PATH, &st);
  int stat_fw = ksceIoGetstat(FW_PATH, &st);
  int stat_df = ksceIoGetstat(DF_PATH, &st);

  if (stat_magic == 0 && stat_fw == 0 && stat_df == 0)
  {
      ksceDebugPrintf("flashing\n");

      // remove magic file as precaution
      ksceIoRemove(CHECK_PATH);

      ret = startBLMode();

      if (ret < 0 && ret != 0x80250222)
      {
          stopBLModeAndReset(); // to be safe
          return SCE_KERNEL_START_SUCCESS; // bail out
      }

      // TODO: retry on any error!

      // if we have fw - flash it first
      if (FLASH_FULL_FW)
      {
          ksceDebugPrintf("erase and flash fw...\n");
          ret = fwMassErase();
          // TODO: check ret and retry
          ret = fwFlash(FW_PATH);
          //TODO retry flashing
      }
      else
      {
          ksceDebugPrintf("erase fw jumptable...\n");
          // retry erasing if fails
          for (int tries = 0; tries < 3; ++tries)
          {
              ret = fwEraseFirstPage();
              if (ret == 0) break;
              ksceDebugPrintf("erasing failed, retry...\n");
          }
      }

      // write df

      ksceDebugPrintf("erase df...\n");
      // retry erasing if fails
      for (int tries = 0; tries < 3; ++tries)
      {
          ret = dfMassErase();
          if (ret == 0) break;
          ksceDebugPrintf("erasing failed, retry...\n");
      }

      ksceDebugPrintf("flash df...\n");
      // retry flashing if fails
      for (int tries = 0; tries < 3; ++tries)
      {
          ret = dfFlash(DF_PATH);
          if (ret == 0) break;
          ksceDebugPrintf("flashing failed, retry...\n");
      }

      // check df checksum
      ksceDebugPrintf("check df...\n");
      ret = checkDFChecksum(DF_PATH);
      //TODO if checksum fails - erase and flash df again

      // write first page of fw
      ksceDebugPrintf("flash fw jumptable...\n");
      for (int tries = 0; tries < 3; ++tries)
      {
          ret = fwFlashFirstPage(FW_PATH);
          if (ret == 0) break;
          ksceDebugPrintf("flashing failed, retry...\n");
      }

      // check fw checksum
      ksceDebugPrintf("check fw...\n");
      ret = checkFWChecksum(FW_PATH);
      //TODO if checksum fails - erase and flash fw again

      ksceDebugPrintf("resetting...\n");
      ret = stopBLModeAndReset();
  }
  ksceDebugPrintf("done\n");

  return SCE_KERNEL_START_SUCCESS;
}
#if !defined(__PCEMU__)

int module_stop()
{
  return SCE_KERNEL_STOP_SUCCESS;
}
#endif
