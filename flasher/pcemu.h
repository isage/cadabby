#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

typedef uint8_t SceUInt8;
typedef uint16_t SceUInt16;
typedef uint32_t SceUInt32;

#define ksceDebugPrintf printf
#define SCE_KERNEL_START_SUCCESS 0

#define ksceIoOpen open
#define ksceIoClose close
#define ksceIoGetstat lstat
#define ksceIoLseek lseek
#define ksceIoRead read
#define ksceIoWrite write
#define SceIoStat struct stat
#define SCE_O_RDONLY O_RDONLY
#define SCE_O_WRONLY O_WRONLY
#define SCE_O_CREAT O_CREAT
#define SCE_SEEK_SET SEEK_SET


#define REG_CMD 0x00
#define REG_ROW0 0x01
#define REG_ROW1 0x02
#define REG_ROW3 0x03
#define REG_DATA0 0x04 // 96 regs

#define REG_CHECKSUM0 0x64
#define REG_CHECKSUM1 0x65

#define CMD_IF_READ_ROW   0x00
#define CMD_IF_WRITE_WORD 0x01 // "word" is 3 bytes
#define CMD_IF_WRITE_ROW  0x02
#define CMD_IF_ERASE_PAGE 0x03
#define CMD_IF_ERASE_ALL  0x04
#define CMD_IF_CHECKSUM   0x05
#define CMD_UNK_0x06      0x06
#define CMD_UNK_0x07      0x07

#define CMD_DF_CHECKSUM  0x08
#define CMD_UNK_0x09     0x09
#define CMD_DF_WRITE_ROW 0x0A
#define CMD_DF_ERASE_ROW 0x0B
#define CMD_DF_ERASE_ALL 0x0C
#define CMD_UNK_0x0D     0x0D
#define CMD_UNK_0x0E     0x0E
#define CMD_EXIT_ROM     0x0F

#define CMD_NONE         0xFF


#define ERROR_INVARG    0x80250001
#define ERROR_NOMEM     0x80250002
#define ERROR_I2C_NACK  0x80250222
#define ERROR_I2C_FAIL  0x80250225


#define STATE_WAITING_CODE 0x00
#define STATE_WAITING_ROW 0x01
#define STATE_WAITING_CHECKSUM 0x02

#define STATE_NONE 0xFF

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

typedef struct {
    uint32_t size;
    uint8_t* data;
} BLCommand;

static uint8_t fw_data[512*96];
static uint8_t df_data[32*32];
static BLCommand cmds[4096];
static uint8_t isBLMode = 0;
static uint8_t current_cmd = CMD_NONE;
static uint8_t state = STATE_NONE;

static uint16_t checksum = 0;
static uint32_t current_row = 0;
static uint8_t regdata[96] = {0};
static uint8_t regdatasize = 0;

int ksceIoRemove(const char* filename)
{
    return 0;
}


int ksceKernelDelayThread(int time)
{
    return 0;
}

int ksceSysconBatteryStartBLMode(void)
{
    printf("Starting BL mode\n");
    // load default fw/df

    FILE* fp = fopen("abby_emufw.bin", "rb");
    if (!fp)
    {
        return ERROR_I2C_FAIL;
    }
    fread(fw_data, 1, 96*512, fp);
    fclose(fp);

    fp = fopen("abby_emudf.bin", "rb");
    if (!fp)
    {
        return ERROR_I2C_FAIL;
    }
    fread(df_data, 1, 32*32, fp);
    fclose(fp);

    isBLMode = 1;
    return 0;
}

int ksceSysconBatteryStopBLMode(void)
{
    printf("Stopping BL mode\n");

    // save new fw/df
    FILE* fp = fopen("abby_emufw_new.bin", "wb");
    if (!fp)
    {
        return ERROR_I2C_FAIL;
    }
    fwrite(fw_data, 1, 96*512, fp);
    fclose(fp);

    fp = fopen("abby_emudf_new.bin", "wb");
    if (!fp)
    {
        return ERROR_I2C_FAIL;
    }
    fwrite(df_data, 1, 32*32, fp);
    fclose(fp);

    isBLMode = 0;
    return 0;
}

int ksceSysconBatterySWReset(void)
{
    printf("SW reset\n");
    return 0;
}

int ksceSysconBatterySetBLCommand(SceUInt16 cmdNumber,SceUInt8 offset,void *data,int size)
{
    printf("ksceSysconBatterySetBLCommand(%d, %d, data, %d)\n", cmdNumber, offset, size);
    if (size > 16)
    {
        fprintf(stderr, "Wrong size!\n");
        return ERROR_INVARG;
    }


    if (!data)
    {
        fprintf(stderr, "Wrong data!\n");
        return ERROR_INVARG;
    }

    for (int i =0; i < MIN(16, size); i++)
    {
        printf("0x%02X ", ((uint8_t*)data)[i]);
    }
    printf("\n");

    if (cmds[cmdNumber].size < offset+size)
    {
        cmds[cmdNumber].size = offset + size;
        uint8_t* newptr = realloc(cmds[cmdNumber].data, cmds[cmdNumber].size);
        if (!newptr)
        {
            fprintf(stderr, "Can't reallocate data!\n");
            return ERROR_NOMEM;
        }
        cmds[cmdNumber].data = newptr;
    }

    memcpy(cmds[cmdNumber].data + offset, data, size);

    return 0;
}

int process_cmd(uint8_t cmd)
{
    current_cmd = cmd;
    printf("cmd 0x%02x\n", cmd);
    switch(cmd)
    {
        case CMD_IF_ERASE_ALL:
            // set checksum
            checksum = cmd;
            // set state to waiting code
            state = STATE_WAITING_CODE;
            break;
        case CMD_DF_ERASE_ALL:
            // set checksum
            checksum = cmd;
            // set state to waiting code
            state = STATE_WAITING_CODE;
            break;
        case CMD_IF_ERASE_PAGE:
            // set checksum
            checksum = cmd;
            // set state to waiting code
            state = STATE_WAITING_CHECKSUM;
            break;
        default:
            checksum = cmd;
            break;
    }
    return 0;
}

int process_data(uint8_t offset, uint8_t* data, uint32_t size)
{
    if (current_cmd == CMD_IF_ERASE_ALL || current_cmd == CMD_DF_ERASE_ALL)
    {
        if (state != STATE_WAITING_CODE)
        {
            fprintf(stderr, "wrong state\n");
            return ERROR_I2C_FAIL;
        }

        for (int i = 0; i < size; ++i)
        {
            checksum += data[i];

        }

        if (data[0] == 0x83 && data[1] == 0xDE)
        {
            state = STATE_WAITING_CHECKSUM;
        }
        else
        {
            fprintf(stderr, "wrong key, got: 0x%02x 0x%02x\n");
        }
    }

    if (current_cmd == CMD_IF_WRITE_ROW || current_cmd == CMD_DF_WRITE_ROW)
    {
        memcpy(regdata+offset, data, size);
        regdatasize = size;
        for (int i = 0; i < size; ++i)
        {
            checksum += data[i];
        }
    }

    return 0;
}

int process_row(uint8_t* data, uint32_t size)
{
    current_row = (data[1] << 8) + data[0];
    checksum += data[0] + data[1];

    return 0;
}

int try_execute_cmd(uint8_t* data, uint32_t size)
{
    if (state != STATE_WAITING_CHECKSUM)
    {
        fprintf(stderr, "wrong state\n");
        return ERROR_I2C_FAIL;
    }

    // check checksum
    uint32_t new_checksum = (data[1] << 8) + data[0];
    checksum = checksum & 0xFFFF;

    if (checksum != new_checksum)
    {
        fprintf(stderr, "wrong checksum. expected 0x%08x, got 0x%08x\n", checksum, new_checksum);
        return ERROR_I2C_FAIL;
    }

    switch(current_cmd)
    {
        case CMD_IF_ERASE_ALL:
            memset(fw_data, 0, 512*96);
            break;
        case CMD_DF_ERASE_ALL:
            memset(df_data, 0, 32*32);
            break;
        case CMD_IF_ERASE_PAGE:
            memset(fw_data+(current_row*(96*2)), 0, 96*2);
            break;
        case CMD_DF_ERASE_ROW:
            memset(df_data+(current_row*32), 0, 32);
            break;

        case CMD_DF_WRITE_ROW:
            memcpy(df_data+(current_row*32), regdata, regdatasize);
            break;
        case CMD_IF_WRITE_ROW:
            memcpy(fw_data+(current_row*96), regdata, regdatasize);
            break;

        case CMD_IF_READ_ROW:
            memcpy(regdata, df_data+(current_row*96), 96);
            break;

        case CMD_IF_CHECKSUM:
            {
                uint32_t ifchecksum = 0;
                for (int i = 0; i < 512*96; i+=3)
                    ifchecksum += ((fw_data[i+2] << 16) + (fw_data[i+1] << 8) + fw_data[i]);
                memcpy(regdata, &ifchecksum , 4);
            }
            break;

        case CMD_DF_CHECKSUM:
            {
                uint16_t dfchecksum = 0;
                for (int i = 0; i < 32*32; ++i)
                    dfchecksum += df_data[i];
                memcpy(regdata, &dfchecksum , 2);
            }
            break;

        default:
            return ERROR_I2C_FAIL;
            break;
    }

    return 0;
}

int ksceSysconBatteryExecBLCommand(SceUInt16 cmdNumber)
{
    printf("ksceSysconBatteryExecBLCommand(%d)\n", cmdNumber);
    if (cmds[cmdNumber].size == 0 || !cmds[cmdNumber].data)
    {
        fprintf(stderr, "Wrong command number!\n");
        return ERROR_INVARG;
    }

    uint8_t* data = cmds[cmdNumber].data;

    if (data[0] != 0x16)
    {
        fprintf(stderr, "Unknown target device!\n");
        free(cmds[cmdNumber].data);
        cmds[cmdNumber].size = 0;
        return ERROR_INVARG;
    }

    if(!isBLMode)
    {
        fprintf(stderr, "Device isn't in ROM mode!\n");
        free(cmds[cmdNumber].data);
        cmds[cmdNumber].size = 0;
        return ERROR_INVARG;
    }

    // switch command, do stuff
    uint8_t first_register = data[1];
    int ret = 0;
    switch(first_register)
    {
        case REG_CMD:
            printf("process cmd\n");
            ret = process_cmd(data[2]);
            break;
        case REG_DATA0:
            printf("process data\n");
            ret = process_data(0, data+2, cmds[cmdNumber].size - 2);
            break;
        case REG_ROW0:
            printf("process row\n");
            ret = process_row(data+2, cmds[cmdNumber].size - 2);
            break;
        case REG_CHECKSUM0:
            printf("process checksum\n");
            ret = try_execute_cmd(data+2, cmds[cmdNumber].size - 2);
            break;
        default:
            ret = ERROR_INVARG;
            fprintf(stderr, "Unknown command!\n");
            break;
    }

    // clean command
    free(cmds[cmdNumber].data);
    cmds[cmdNumber].size = 0;
    printf("current state: 0x%02x\n", state);
    return ret;
}

int ksceSysconBatteryReadBLCommand(SceUInt16 cmdNumber,SceUInt8 addr,SceUInt8 reg,void *data,SceUInt32 size)
{
    if (addr != 0x16)
    {
        fprintf(stderr, "Unknown device!");
        exit(-1);
    }

    if (reg == 0x04)
    {
        memcpy(data, regdata, size);
    }
    else if (reg == 0x66)
    {
        memset(data, 0, size);
    }
    else
    {
        fprintf(stderr, "Unknown reg!");
        exit(-1);
    }
    return 0;
}

