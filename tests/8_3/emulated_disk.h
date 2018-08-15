#ifndef EMULATED_DISK_H
#define EMULATED_DISK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
    
#include "fileio.h"
    
struct EMULATED_DISK
{
    uint16_t sector_size;
    void* sectors;
};

struct EMULATED_DRIVE
{
    void (*initialize)(void);
    void (*print)(void);
    const char* (*id)(void);
};

void EmulatedDiskCreate(struct EMULATED_DISK* disk, uint16_t sector_size);
void EmulatedDiskDestroy(struct EMULATED_DISK* disk);
void EmulatedDiskPrint(struct EMULATED_DISK* disk);

extern bool EmuldatedDiskWriteProtectStateGet(void * mediaConfig);
extern uint8_t EmulatedDiskSectorWrite(void * mediaConfig, uint32_t sector_addr, uint8_t* buffer, bool allowWriteToZero);
extern bool EmulatedDiskMediaDetect(void * mediaConfig);
extern FILEIO_MEDIA_INFORMATION *EmulatedDiskMediaInitialize(void * mediaConfig);
extern bool EmulatedDiskMediaDeinitialize(void * mediaConfig);
extern bool EmulatedDiskSectorRead(void * mediaConfig, uint32_t sector_addr, uint8_t* buffer);

#ifdef __cplusplus
}
#endif

#endif /* EMULATED_DISK_H */

