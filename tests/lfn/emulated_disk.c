#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "emulated_disk.h"
#include "fileio_lfn.h"

struct SECTOR
{
    uint32_t lba;
    uint8_t *data;
};

struct SECTOR_LIST_NODE
{
    struct SECTOR sector;
    struct SECTOR_LIST_NODE* next;
};

const FILEIO_DRIVE_CONFIG EmulatedDisk =
{
    (FILEIO_DRIVER_IOInitialize)NULL,                     // Function to initialize the I/O pins used by the driver.
    (FILEIO_DRIVER_MediaDetect)EmulatedDiskMediaDetect,                   // Function to detect that the media is inserted.
    (FILEIO_DRIVER_MediaInitialize)EmulatedDiskMediaInitialize,           // Function to initialize the media.
    (FILEIO_DRIVER_MediaDeinitialize)EmulatedDiskMediaDeinitialize,              // Function to de-initialize the media.
    (FILEIO_DRIVER_SectorRead)EmulatedDiskSectorRead,                     // Function to read a sector from the media.
    (FILEIO_DRIVER_SectorWrite)EmulatedDiskSectorWrite,                   // Function to write a sector to the media.
    (FILEIO_DRIVER_WriteProtectStateGet)EmuldatedDiskWriteProtectStateGet,    // Function to determine if the media is write-protected.
};

static FILEIO_MEDIA_INFORMATION media_info;
static struct EMULATED_DISK *current_disk = NULL;

void EmulatedDiskCreate(struct EMULATED_DISK* disk, uint16_t sector_size){
    disk->sector_size = sector_size;
    current_disk = disk;
}

void EmulatedDiskPrint(struct EMULATED_DISK* disk){
    uint16_t i;
    uint32_t lowest_lba = 0;
    
    struct SECTOR_LIST_NODE* sector_node = (struct SECTOR_LIST_NODE*)(disk->sectors);
    struct SECTOR_LIST_NODE* lowest_lda_node = NULL;
    
    while(sector_node != NULL){
        printf("Sector[%u] = \r\n", (unsigned int)sector_node->sector.lba);
        
        for(i=0; i<disk->sector_size; i++){
            printf("0x%02x ", sector_node->sector.data[i]);
            if((i+1)%16 == 0){
                printf("\r\n");
            }
        }
        
        sector_node = sector_node->next;
    }
}

static struct SECTOR_LIST_NODE* FindSector(struct EMULATED_DISK *disk, uint32_t lba)
{
    struct SECTOR_LIST_NODE* sector_node = (struct SECTOR_LIST_NODE*)(disk->sectors);
    
    while(sector_node != NULL){
        if(sector_node->sector.lba == lba){
            return sector_node;
        }
        sector_node = sector_node->next;
    }
    
    return sector_node;
}


static struct SECTOR_LIST_NODE* AddSector(struct EMULATED_DISK *disk, uint32_t lba)
{
    struct SECTOR_LIST_NODE* parent_node = (struct SECTOR_LIST_NODE*)(disk->sectors);
    struct SECTOR_LIST_NODE* new_node = (struct SECTOR_LIST_NODE*)(disk->sectors);
    
    if(parent_node != NULL){
        while(parent_node->next != NULL){
            if(lba < parent_node->next->sector.lba){
                break;
            }
            parent_node = parent_node->next;
        }
    }
    
    new_node = malloc(sizeof(struct SECTOR_LIST_NODE));
    
    if(new_node != NULL){
        new_node->sector.lba = lba;
        new_node->next = NULL;
        
        new_node->sector.data = malloc(disk->sector_size);
        
        if(new_node->sector.data == NULL){
            free(new_node);
        } else {
            if(parent_node != NULL){
                new_node->next = parent_node->next;
                parent_node->next = new_node;
            } else {
                disk->sectors = new_node;
            }
            
            return new_node;
        }
    }
    
    return NULL;
}
    
bool EmuldatedDiskWriteProtectStateGet(void * mediaConfig){
    return false;
}

uint8_t EmulatedDiskSectorWrite(void * mediaConfig, uint32_t sector_addr, uint8_t* buffer, bool allowWriteToZero)
{
    struct SECTOR_LIST_NODE* sector_node;
    
    if(current_disk == NULL) { return false; }
    
    sector_node = FindSector(current_disk, sector_addr);
    
    if(sector_node == NULL){
        sector_node = AddSector(current_disk, sector_addr);
        
        if(sector_node == NULL){
            return false;
        }
    }
    
    memcpy(sector_node->sector.data, buffer, current_disk->sector_size);
    
    return true;
}

bool EmulatedDiskMediaDetect(void * mediaConfig){    
    return (current_disk != NULL);
}
        
FILEIO_MEDIA_INFORMATION *EmulatedDiskMediaInitialize(void * mediaConfig){
    ((struct EMULATED_DRIVE*)mediaConfig)->initialize();
    
    media_info.sectorSize = current_disk->sector_size;
    media_info.errorCode = MEDIA_NO_ERROR;
    media_info.maxLUN = 1;
    media_info.validityFlags.bits.sectorSize = 1;
    media_info.validityFlags.bits.maxLUN = 1;

    return &media_info;
}

bool EmulatedDiskMediaDeinitialize(void * mediaConfig){
    struct SECTOR_LIST_NODE* current_node = (struct SECTOR_LIST_NODE*)(current_disk->sectors);
    struct SECTOR_LIST_NODE* parent_node;
    
    if(current_node == NULL) { return true; }
        
    do{
        current_node = (struct SECTOR_LIST_NODE*)(current_disk->sectors);
        parent_node = current_node;
        
        while(current_node->next != NULL){
            parent_node = current_node;
            current_node = current_node->next;
        }

        if(current_node->sector.data != NULL){
            free(current_node->sector.data);
        }
        parent_node->next = NULL;
        free(current_node); 
    } while(parent_node != current_node);
    
    current_disk->sectors = NULL;
    current_disk = NULL;
    
    return true;
}

bool EmulatedDiskSectorRead(void * mediaConfig, uint32_t lba, uint8_t* data){
    struct SECTOR_LIST_NODE* sector_node;
    
    if(current_disk == NULL){ return false; }
    
    sector_node = FindSector(current_disk, lba);
    
    if(sector_node == NULL){       
        memset(data, 0, current_disk->sector_size);
    } else {
        memcpy(data, sector_node->sector.data, current_disk->sector_size);
    }
    
    return true;
}
