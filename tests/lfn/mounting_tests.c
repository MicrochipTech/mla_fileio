#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "fileio_lfn.h"
#include "emulated_disk.h"
#include "drive_list.h"

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
