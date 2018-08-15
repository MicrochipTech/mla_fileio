#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fileio.h"
#include "emulated_disk.h"

extern struct EMULATED_DRIVE DRV001;
extern struct EMULATED_DRIVE DRV002;
extern struct EMULATED_DRIVE DRV003;
extern struct EMULATED_DRIVE DRV004;
extern struct EMULATED_DRIVE DRV005;
extern struct EMULATED_DRIVE DRV006;
extern struct EMULATED_DRIVE DRV007;
extern struct EMULATED_DRIVE DRV008;
extern struct EMULATED_DRIVE DRV009;
extern struct EMULATED_DRIVE DRV010;
extern struct EMULATED_DRIVE DRV011;
extern struct EMULATED_DRIVE DRV012;
extern struct EMULATED_DRIVE DRV013;
extern struct EMULATED_DRIVE DRV014;
extern struct EMULATED_DRIVE DRV015;
extern struct EMULATED_DRIVE DRV016;
extern struct EMULATED_DRIVE DRV017;
extern struct EMULATED_DRIVE DRV018;
extern struct EMULATED_DRIVE DRV019;
extern struct EMULATED_DRIVE DRV020;
extern struct EMULATED_DRIVE DRV021;
extern struct EMULATED_DRIVE DRV022;
extern struct EMULATED_DRIVE DRV023;
extern struct EMULATED_DRIVE DRV024;
extern struct EMULATED_DRIVE DRV025;
extern struct EMULATED_DRIVE DRV026;
extern struct EMULATED_DRIVE DRV027;
extern struct EMULATED_DRIVE DRV028;
extern struct EMULATED_DRIVE DRV029;
extern struct EMULATED_DRIVE DRV030;
extern struct EMULATED_DRIVE DRV031;
extern struct EMULATED_DRIVE DRV032;
extern struct EMULATED_DRIVE DRV033;
extern struct EMULATED_DRIVE DRV034;
extern struct EMULATED_DRIVE DRV035;
extern struct EMULATED_DRIVE DRV036;
extern struct EMULATED_DRIVE DRV037;
extern struct EMULATED_DRIVE DRV038;
extern struct EMULATED_DRIVE DRV039;
extern struct EMULATED_DRIVE DRV040;
extern struct EMULATED_DRIVE DRV041;
extern struct EMULATED_DRIVE DRV042;
extern struct EMULATED_DRIVE DRV043;
extern struct EMULATED_DRIVE DRV044;
extern struct EMULATED_DRIVE DRV045;
extern struct EMULATED_DRIVE DRV046;
extern struct EMULATED_DRIVE DRV047;
extern struct EMULATED_DRIVE DRV048;
extern struct EMULATED_DRIVE DRV049;
extern struct EMULATED_DRIVE DRV050;
extern struct EMULATED_DRIVE DRV051;
extern struct EMULATED_DRIVE DRV052;
extern struct EMULATED_DRIVE DRV053;
extern struct EMULATED_DRIVE DRV054;
extern struct EMULATED_DRIVE DRV055;
extern struct EMULATED_DRIVE DRV056;
extern struct EMULATED_DRIVE DRV057;
extern struct EMULATED_DRIVE DRV058;
extern struct EMULATED_DRIVE DRV059;
extern struct EMULATED_DRIVE DRV060;
extern struct EMULATED_DRIVE DRV061;
extern struct EMULATED_DRIVE DRV062;
extern struct EMULATED_DRIVE DRV063;
extern struct EMULATED_DRIVE DRV064;
extern struct EMULATED_DRIVE DRV065;
extern struct EMULATED_DRIVE DRV066;
extern struct EMULATED_DRIVE DRV067;
extern struct EMULATED_DRIVE DRV068;
extern struct EMULATED_DRIVE DRV069;
extern struct EMULATED_DRIVE DRV070;
extern struct EMULATED_DRIVE DRV071;
extern struct EMULATED_DRIVE DRV072;
extern struct EMULATED_DRIVE DRV073;
extern struct EMULATED_DRIVE DRV074;
extern struct EMULATED_DRIVE DRV075;
extern struct EMULATED_DRIVE DRV076;
extern struct EMULATED_DRIVE DRV077;
extern struct EMULATED_DRIVE DRV078;
extern struct EMULATED_DRIVE DRV079;
extern struct EMULATED_DRIVE DRV080;
extern struct EMULATED_DRIVE DRV081;
extern struct EMULATED_DRIVE DRV082;
extern struct EMULATED_DRIVE DRV083;
extern struct EMULATED_DRIVE DRV084;
extern struct EMULATED_DRIVE DRV085;
extern struct EMULATED_DRIVE DRV086;
extern struct EMULATED_DRIVE DRV087;
extern struct EMULATED_DRIVE DRV088;
extern struct EMULATED_DRIVE DRV089;
extern struct EMULATED_DRIVE DRV090;
extern struct EMULATED_DRIVE DRV091;
extern struct EMULATED_DRIVE DRV092;
extern struct EMULATED_DRIVE DRV093;
extern struct EMULATED_DRIVE DRV094;
extern struct EMULATED_DRIVE DRV095;
extern struct EMULATED_DRIVE DRV096;
extern struct EMULATED_DRIVE DRV097;


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
