#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "fileio.h"
#include "emulated_disk.h"
#include "drive_list.h"

static struct EMULATED_DRIVE *drives[] = {
    &DRV001,
    &DRV002,
    &DRV003,
    &DRV004,
    &DRV005,
    &DRV006,
    &DRV007,
    &DRV008,
    &DRV009,
    &DRV010,
    &DRV011,
    &DRV012,
    &DRV013,
    &DRV014,
    &DRV015,
    &DRV016,
    &DRV017,
    &DRV018,
    &DRV019,
    &DRV021,
    &DRV022,
    &DRV023,
    &DRV024,
    &DRV025,
    &DRV026,
    &DRV027,
    &DRV028,
    &DRV029,
    &DRV030,
    &DRV031,
    &DRV032,
    &DRV033,
    &DRV034,
    &DRV035,
    &DRV036,
    &DRV037,
    &DRV039,
    &DRV040,
    &DRV041,
    &DRV042,
    &DRV043,
    &DRV044,
    &DRV045,
    &DRV046,
    &DRV047,
    &DRV048,
    &DRV049,
    &DRV050,
    &DRV051,
    &DRV052,
    &DRV053,
    &DRV054,
    &DRV055,
    &DRV056,
    &DRV057,
    &DRV058,
    &DRV059,
    &DRV060,
    &DRV061,
    &DRV062,
    &DRV063,
    &DRV064,
    &DRV065,
    &DRV066,
    &DRV067,
    &DRV068,
    &DRV069,
    &DRV070,
    &DRV071,
    &DRV072,
    &DRV073,
    &DRV074,
    &DRV075,
    &DRV076,
    &DRV077,
    &DRV078,
    &DRV079,
    &DRV080,
    &DRV081,
    &DRV082,
    &DRV083,
    &DRV084,
    &DRV085,
    &DRV086,
    &DRV087,
    &DRV088,
    &DRV089,
    &DRV091,
    &DRV093,
    &DRV094,
    &DRV095,
    &DRV096,
    &DRV097
};

extern const FILEIO_DRIVE_CONFIG EmulatedDisk;

void RunMountingTests(void){
    int drive_index;
    struct EMULATED_DRIVE *drive = drives[0];
    uint32_t passed;
    const uint32_t drive_count = (sizeof (drives) / sizeof (struct EMULATED_DRIVE*));
    
    passed = 0;
    for (drive_index = 0; drive_index < drive_count; drive_index++) {
        drive = drives[drive_index];
        
        FILEIO_Initialize();
        if(FILEIO_DriveMount ('A', &EmulatedDisk, (void*)drive) == FILEIO_ERROR_NONE){
            passed++;   
            FILEIO_DriveUnmount ('A');
        } else {
            printf("Drive %s failed to mount.\r\n", drive->id());
        } 
    }
    
    printf( "Drive mounting: drives: %i, passed: %i, failed: %i\r\n", drive_count, passed, drive_count-passed );
}
