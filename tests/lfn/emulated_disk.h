/*******************************************************************************
Copyright 2018 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/

#ifndef EMULATED_DISK_H
#define EMULATED_DISK_H

#include <stdint.h>
#include <stdbool.h>
    
#include "fileio_lfn.h"
    
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

#endif /* EMULATED_DISK_H */

