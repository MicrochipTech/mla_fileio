#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "fileio_lfn.h"
#include "emulated_disk.h"
#include "drive_list.h"

extern FILEIO_DRIVE_CONFIG EmulatedDisk;

bool TestSetup(struct EMULATED_DRIVE *test_drive){
    const char name[] = "TestSetup";
    
    if(FILEIO_Initialize() != true) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DriveMount ('A', &EmulatedDisk, (void*)test_drive) != FILEIO_ERROR_NONE){printf("TEST FAILED: %s\r\n", name); return false;}
    
    return true;
}

void TestTearDown(void){
    FILEIO_DriveUnmount ('A');
}

bool CreateFileInRoot(void){ 
    const char name[] = "CreateFileInRoot";
    FILEIO_OBJECT myFile;
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    const uint16_t testDir1Name[] = {'T','E','S','T',0};
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write(testDir1Name, 1, 4, &myFile) != 4) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}    
    
    return true;
}

bool ReadFileInRoot(void){ 
    const char name[] = "ReadFileInRoot";
    FILEIO_OBJECT myFile;
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    const uint16_t testDir1Name[] = {'T','E','S','T',0};
    uint8_t buffer[4];
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write(testDir1Name, 1, 4, &myFile) != 4) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_READ) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Read (buffer, 1, 4, &myFile) != 4){printf("TEST FAILED: %s\r\n", name); return false;}
    if(memcmp(buffer, testDir1Name, 4) != 0){printf("TEST FAILED: %s\r\n", name); return false;}
    
    return true;
}

bool EndOfFile(void){ 
    const char name[] = "EndOfFile";
    FILEIO_OBJECT myFile;
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    const uint16_t testDir1Name[] = {'T','E','S','T',0};
    uint8_t buffer[4];
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write(testDir1Name, 1, 4, &myFile) != 4) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_READ) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Read (buffer, 1, 3, &myFile) != 3){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Eof(&myFile) == true){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Read (buffer, 1, 1, &myFile) != 1){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Eof(&myFile) != true){printf("TEST FAILED: %s\r\n", name); return false;}
    
    return true;
}

bool DirectoryRemoveAfterFolderDeletedButFileRemains(void){ 
    const char name[] = "DirectoryRemoveAfterFolderDeletedButFileRemains";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    const uint16_t testDir1Name[] = {'T','E','S','T',0};
    const uint16_t testDir2Name[] = {'T','E','S','T','2',0};
    const uint16_t testDirUpName[] = {'.','.',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_DirectoryMake(testDir1Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DirectoryChange(testDir1Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DirectoryMake(testDir2Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write(testDir1Name, 1, 4, &myFile) != 4) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 
    if(FILEIO_DirectoryRemove(testDir2Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DirectoryChange(testDirUpName) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DirectoryRemove(testDir1Name) != FILEIO_RESULT_FAILURE) {printf("TEST FAILED: %s\r\n", name); return false;}
    
    return true;
}

bool DirectoryRemoveAfterFileDeleted(void){ 
    const char name[] = "DirectoryRemoveAfterFileDeleted";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    const uint16_t testDir1Name[] = {'T','E','S','T',0};
    const uint16_t testDirUpName[] = {'.','.',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_DirectoryMake(testDir1Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DirectoryChange(testDir1Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write(testDir1Name, 1, 4, &myFile) != 4) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}    
    if(FILEIO_Remove(testFileName) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DirectoryChange(testDirUpName) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DirectoryRemove(testDir1Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    
    return true;
}

bool DirectoryRemoveWhileNotEmpty(void){ 
    const char name[] = "DirectoryRemoveWhileNotEmpty";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    const uint16_t testDir1Name[] = {'T','E','S','T',0};
    const uint16_t testDirUpName[] = {'.','.',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_DirectoryMake(testDir1Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DirectoryChange(testDir1Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write(testDir1Name, 1, 4, &myFile) != 4) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}    
    if(FILEIO_DirectoryChange(testDirUpName) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DirectoryRemove(testDir1Name) != FILEIO_RESULT_FAILURE) {printf("TEST FAILED: %s\r\n", name); return false;}
    
    return true;
}

bool SeekCurrent(void){ 
    const char name[] = "SeekCurrent";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1, 26, &myFile) != 26) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, -1, FILEIO_SEEK_CUR) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_GetChar(&myFile) != 'Z') {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, -26, FILEIO_SEEK_CUR) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_GetChar(&myFile) != 'A') {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 2, FILEIO_SEEK_CUR) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_GetChar(&myFile) != 'D') {printf("TEST FAILED: %s\r\n", name); return false;}
    
    return true;
}

bool SeekSet(void){ 
    const char name[] = "SeekSet";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1, 26, &myFile) != 26) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 1, FILEIO_SEEK_SET) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_GetChar(&myFile) != 'B') {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 25, FILEIO_SEEK_SET) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_GetChar(&myFile) != 'Z') {printf("TEST FAILED: %s\r\n", name); return false;}
    
    return true;
}

bool SeekEnd(void){ 
    const char name[] = "SeekEnd";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1, 26, &myFile) != 26) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 1, FILEIO_SEEK_END) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_GetChar(&myFile) != 'Z') {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 26, FILEIO_SEEK_END) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_GetChar(&myFile) != 'A') {printf("TEST FAILED: %s\r\n", name); return false;}
    
    return true;
}

bool SeekAndWriteNotPassingEnd(void){ 
    const char name[] = "SeekAndWriteNotPassingEnd";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1, 26, &myFile) != 26) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 1, FILEIO_SEEK_SET) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("X", 1, 1, &myFile) != 1){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}    
    if(myFile.size != 26){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 1, FILEIO_SEEK_SET) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_GetChar(&myFile) != 'X') {printf("TEST FAILED: %s\r\n", name); return false;}
    return true;
}

bool SeekAndWriteAtEnd(void){ 
    const char name[] = "SeekAndWriteAtEnd";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1, 26, &myFile) != 26) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 25, FILEIO_SEEK_SET) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("X", 1, 1, &myFile) != 1){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}    
    if(myFile.size != 26){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 25, FILEIO_SEEK_SET) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_GetChar(&myFile) != 'X') {printf("TEST FAILED: %s\r\n", name); return false;}

    return true;
}

bool SeekAndWritePastEnd(void){ 
    const char name[] = "SeekAndWritePastEnd";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1, 26, &myFile) != 26) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 25, FILEIO_SEEK_SET) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("12", 1, 2, &myFile) != 2){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}    
    if(myFile.size != 27){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 26, FILEIO_SEEK_SET) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_GetChar(&myFile) != '2') {printf("TEST FAILED: %s\r\n", name); return false;}

    return true;
}

bool SeekAndWritePastEnd_2(void){ 
    const char name[] = "SeekAndWritePastEnd_2";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1, 26, &myFile) != 26) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 25, FILEIO_SEEK_SET) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("123", 1, 3, &myFile) != 3){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}    
    if(myFile.size != 28){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 27, FILEIO_SEEK_SET) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_GetChar(&myFile) != '3') {printf("TEST FAILED: %s\r\n", name); return false;}

    return true;
}

bool SeekAndWritePastEnd_3(void){ 
    const char name[] = "SeekAndWritePastEnd_3";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1, 26, &myFile) != 26) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 28, FILEIO_SEEK_SET) != FILEIO_RESULT_FAILURE) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_ErrorGet('A') != FILEIO_ERROR_INVALID_ARGUMENT){printf("TEST FAILED: %s\r\n", name); return false;}
    FILEIO_ErrorClear('A');

    return true;
}

bool ErrorClear(void){ 
    const char name[] = "SeekAndWritePastEnd_3";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1, 26, &myFile) != 26) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 28, FILEIO_SEEK_SET) != FILEIO_RESULT_FAILURE) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_ErrorGet('A') != FILEIO_ERROR_INVALID_ARGUMENT){printf("TEST FAILED: %s\r\n", name); return false;}
    FILEIO_ErrorClear('A');
    if(FILEIO_ErrorGet('A') != FILEIO_ERROR_NONE){printf("TEST FAILED: %s\r\n", name); return false;}
    
    return true;
}

bool CreateMultipleDirectoriesAtOnce(void){ 
    const char name[] = "CreateMultipleDirectoriesAtOnce";
    const uint16_t testDir1Name[] = {'T','E','S','T',0};
    const uint16_t testDir2Name[] = {'T','E','S','T','2',0};
    const uint16_t testDir3Name[] = {'T','E','S','T','3',0};
    const uint16_t testDir4Name[] = {'T','E','S','T','4',0};
    const uint16_t testDirString[] = {'T','E','S','T','/','T','E','S','T','2','/','T','E','S','T','3','/','T','E','S','T','4',0};
            
    if(FILEIO_DirectoryMake(testDirString) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DirectoryChange(testDir1Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DirectoryChange(testDir2Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DirectoryChange(testDir3Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_DirectoryChange(testDir4Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}

    return true;
}

bool Tell(void){ 
    const char name[] = "Tell";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_READ | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1, 26, &myFile) != 26) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 1, FILEIO_SEEK_SET) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Tell(&myFile) != 1) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_GetChar(&myFile) != 'B') {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Tell(&myFile) != 2) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Seek(&myFile, 25, FILEIO_SEEK_SET) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Tell(&myFile) != 25) {printf("TEST FAILED: %s\r\n", name); return false;}
    
    return true;
}

bool Rename(void){ 
    const char name[] = "Rename";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    const uint16_t testFile2Name[] = {'T','E','S','T','2','.','T','X','T',0};
    const uint16_t testDir1Name[] = {'T','E','S','T',0};
    FILEIO_OBJECT myFile;
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write(testDir1Name, 1, 4, &myFile) != 4) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 
    if(FILEIO_Rename(testFileName, testFile2Name) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 
    if(FILEIO_Open(&myFile, testFile2Name, FILEIO_OPEN_READ) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_READ) != FILEIO_RESULT_FAILURE){printf("TEST FAILED: %s\r\n", name); return false;}    
    
    return true;
}

bool FindFilesInRoot(void){ 
    const char name[] = "FindFilesInRoot";
    const uint16_t testFileName[] = {'T','E','S','T','.','T','X','T',0};
    const uint16_t testFile2Name[] = {'T','E','S','T','2','.','T','X','T',0};
    const uint16_t testFileSearchName[] = {'T','E','S','T','*','.','T','X','T',0};
    const uint16_t testDir1Name[] = {'T','E','S','T',0};
    const uint16_t testDir2Name[] = {'T','E','S','T','2',0};
    FILEIO_OBJECT myFile;
    FILEIO_SEARCH_RECORD searchRecord;
    
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write(testDir1Name, 1, 4, &myFile) != 4) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 
    
    if(FILEIO_Open(&myFile, testFile2Name, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE | FILEIO_OPEN_TRUNCATE) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Write(testDir2Name, 1, 5, &myFile) != 5) {printf("TEST FAILED: %s\r\n", name); return false;}
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 
    
    if(FILEIO_Find (testFileSearchName, FILEIO_ATTRIBUTE_MASK, &searchRecord, true) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 
    if(FILEIO_Find (testFileSearchName, FILEIO_ATTRIBUTE_MASK, &searchRecord, false) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 
    if(FILEIO_Find (testFileSearchName, FILEIO_ATTRIBUTE_MASK, &searchRecord, false) != FILEIO_RESULT_FAILURE) {printf("TEST FAILED: %s\r\n", name); return false;} 
    
    return true;
}

typedef bool (*TEST_FUNCTION)(void);
TEST_FUNCTION tests[]={
    &CreateFileInRoot,
    &ReadFileInRoot,
    &EndOfFile,
    &SeekCurrent,
    &SeekSet, 
    &SeekEnd, 
    &Tell,
    &Rename,
    &FindFilesInRoot,
    &DirectoryRemoveAfterFileDeleted,
    &DirectoryRemoveWhileNotEmpty,
    &DirectoryRemoveAfterFolderDeletedButFileRemains,
    &SeekAndWriteNotPassingEnd,
    &SeekAndWriteAtEnd,
    &SeekAndWritePastEnd,
    &SeekAndWritePastEnd_2,
    &SeekAndWritePastEnd_3,
    &ErrorClear,
    &CreateMultipleDirectoriesAtOnce
};

const uint32_t test_count = (sizeof(tests)/sizeof(TEST_FUNCTION));

static int RunTests(struct EMULATED_DRIVE *drive){
    int test_index;
    uint32_t passed;
           
    passed = 0;
    for(test_index=0; test_index < test_count; test_index++){
        if(TestSetup(drive) == true){
            passed += tests[test_index]();
            TestTearDown();
        }
    }

    return passed;    
}

void RunFunctionalTests(void){
    int passed = RunTests(&DRV096);
    
    printf( "Functional tests: run: %i, passed: %i, failed: %i\r\n", test_count, passed, test_count-passed);
}

