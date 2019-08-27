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

bool WindowsLFNBasenameUppercaseExtensionLowercase(void){  
    const char name[] = "WindowsLFNBasenameUppercaseExtensionLowercase";
    const uint16_t testFileName[] = {'T','E','S','T','1','.','t','x','t',0};
    FILEIO_OBJECT myFile;
    FILEIO_SEARCH_RECORD searchRecord;
    char data[10];
    char expectedResults[] = "TEST1";
    int dataRead;
    const int dataCountExpected = 5;
    
    memset(data, 0, sizeof(data));
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_READ) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s -  Failed to find file.\r\n", name); return false;}

    dataRead = FILEIO_Read(data, 1, sizeof(data), &myFile);
   
    if(FILEIO_Read(data, 1, sizeof(data), &myFile) != dataCountExpected){printf("TEST FAILED: %s - incorrect amount of data read: expected(%d) actual(%d)\r\n", name, dataRead, dataCountExpected); return false;} 
    if(memcmp(data, expectedResults, strlen(expectedResults)-1) != 0){printf("TEST FAILED: %s - File data mismatch: expected(%s) actual(%s)\r\n", name, expectedResults, data); return false;} 
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 

    return true;
}

bool WindowsLFNBasenameLowercaseExtensionUppercase(void){  
    const char name[] = "WindowsLFNBasenameLowercaseExtensionUppercase";
    const uint16_t testFileName[] = {'t','e','s','t','2','.','T','X','T',0};
    FILEIO_OBJECT myFile;
    FILEIO_SEARCH_RECORD searchRecord;
    char data[10];
    char expectedResults[] = "TEST2";
    int dataRead;
    const int dataCountExpected = 5;
    
    memset(data, 0, sizeof(data));
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_READ) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s -  Failed to find file.\r\n", name); return false;}

    dataRead = FILEIO_Read(data, 1, sizeof(data), &myFile);
   
    if(FILEIO_Read(data, 1, sizeof(data), &myFile) != dataCountExpected){printf("TEST FAILED: %s - incorrect amount of data read: expected(%d) actual(%d)\r\n", name, dataRead, dataCountExpected); return false;} 
    if(memcmp(data, expectedResults, strlen(expectedResults)-1) != 0){printf("TEST FAILED: %s - File data mismatch: expected(%s) actual(%s)\r\n", name, expectedResults, data); return false;} 
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 

    return true;
}

bool WindowsLFNBasenameLowercaseExtensionLowercase(void){  
    const char name[] = "WindowsLFNBasenameLowercaseExtensionLowercase";
    const uint16_t testFileName[] = {'t','e','s','t','3','.','t','x','t',0};
    FILEIO_OBJECT myFile;
    FILEIO_SEARCH_RECORD searchRecord;
    char data[10];
    char expectedResults[] = "TEST3";
    int dataRead;
    const int dataCountExpected = 5;
    
    memset(data, 0, sizeof(data));
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_READ) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s -  Failed to find file.\r\n", name); return false;}

    dataRead = FILEIO_Read(data, 1, sizeof(data), &myFile);
   
    if(FILEIO_Read(data, 1, sizeof(data), &myFile) != dataCountExpected){printf("TEST FAILED: %s - incorrect amount of data read: expected(%d) actual(%d)\r\n", name, dataRead, dataCountExpected); return false;} 
    if(memcmp(data, expectedResults, strlen(expectedResults)-1) != 0){printf("TEST FAILED: %s - File data mismatch: expected(%s) actual(%s)\r\n", name, expectedResults, data); return false;} 
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 

    return true;
}

bool WindowsLFNBasenameUppercaseExtensionLowercaseShort(void){  
    const char name[] = "WindowsLFNBasenameUppercaseExtensionLowercaseShort";
    const uint16_t testFileName[] = {'T','E','S','T','4','.','t','x',0};
    FILEIO_OBJECT myFile;
    FILEIO_SEARCH_RECORD searchRecord;
    char data[10];
    char expectedResults[] = "TEST4";
    int dataRead;
    const int dataCountExpected = 5;
    
    memset(data, 0, sizeof(data));
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_READ) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s -  Failed to find file.\r\n", name); return false;}

    dataRead = FILEIO_Read(data, 1, sizeof(data), &myFile);
   
    if(FILEIO_Read(data, 1, sizeof(data), &myFile) != dataCountExpected){printf("TEST FAILED: %s - incorrect amount of data read: expected(%d) actual(%d)\r\n", name, dataRead, dataCountExpected); return false;} 
    if(memcmp(data, expectedResults, strlen(expectedResults)-1) != 0){printf("TEST FAILED: %s - File data mismatch: expected(%s) actual(%s)\r\n", name, expectedResults, data); return false;} 
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 

    return true;
}

bool WindowsLFNBasenameLowercaseExtensionUppercaseShort(void){  
    const char name[] = "WindowsLFNBasenameLowercaseExtensionUppercaseShort";
    const uint16_t testFileName[] = {'t','e','s','t','5','.','T','X',0};
    FILEIO_OBJECT myFile;
    FILEIO_SEARCH_RECORD searchRecord;
    char data[10];
    char expectedResults[] = "TEST5";
    int dataRead;
    const int dataCountExpected = 5;
    
    memset(data, 0, sizeof(data));
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_READ) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s -  Failed to find file.\r\n", name); return false;}

    dataRead = FILEIO_Read(data, 1, sizeof(data), &myFile);
   
    if(FILEIO_Read(data, 1, sizeof(data), &myFile) != dataCountExpected){printf("TEST FAILED: %s - incorrect amount of data read: expected(%d) actual(%d)\r\n", name, dataRead, dataCountExpected); return false;} 
    if(memcmp(data, expectedResults, strlen(expectedResults)-1) != 0){printf("TEST FAILED: %s - File data mismatch: expected(%s) actual(%s)\r\n", name, expectedResults, data); return false;} 
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 

    return true;
}

bool WindowsLFNBasenameUppercaseFullExtensionLowercase(void){  
    const char name[] = "WindowsLFNBasenameUppercaseFullExtensionLowercase";
    const uint16_t testFileName[] = {'T','E','S','T','1','2','3','4','.','t','x','t',0};
    FILEIO_OBJECT myFile;
    FILEIO_SEARCH_RECORD searchRecord;
    char data[10];
    char expectedResults[] = "TEST1234";
    int dataRead;
    const int dataCountExpected = 8;
    
    memset(data, 0, sizeof(data));
    if(FILEIO_Open(&myFile, testFileName, FILEIO_OPEN_READ) != FILEIO_RESULT_SUCCESS){printf("TEST FAILED: %s -  Failed to find file.\r\n", name); return false;}

    dataRead = FILEIO_Read(data, 1, sizeof(data), &myFile);
   
    if(FILEIO_Read(data, 1, sizeof(data), &myFile) != dataCountExpected){printf("TEST FAILED: %s - incorrect amount of data read: expected(%d) actual(%d)\r\n", name, dataRead, dataCountExpected); return false;} 
    if(memcmp(data, expectedResults, strlen(expectedResults)-1) != 0){printf("TEST FAILED: %s - File data mismatch: expected(%s) actual(%s)\r\n", name, expectedResults, data); return false;} 
    if(FILEIO_Close(&myFile) != FILEIO_RESULT_SUCCESS) {printf("TEST FAILED: %s\r\n", name); return false;} 

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

TEST_FUNCTION windowsSpecificTests[]={
    &WindowsLFNBasenameUppercaseExtensionLowercase,
    &WindowsLFNBasenameLowercaseExtensionUppercase,
    &WindowsLFNBasenameLowercaseExtensionLowercase,
    &WindowsLFNBasenameUppercaseExtensionLowercaseShort,
    &WindowsLFNBasenameLowercaseExtensionUppercaseShort,
    &WindowsLFNBasenameUppercaseFullExtensionLowercase
};

const uint32_t test_count = (sizeof(tests)/sizeof(TEST_FUNCTION));
const uint32_t test_count_windows = (sizeof(windowsSpecificTests)/sizeof(TEST_FUNCTION));

static int RunTests(struct EMULATED_DRIVE *drive){
    int test_index;
    uint32_t passed = 0;

    for(test_index=0; test_index < test_count; test_index++){
        if(TestSetup(drive) == true){
            passed += tests[test_index]();
            TestTearDown();
        }
    }

    return passed;    
}

/* These tests require a drive that has very specific Windows created files that
 * have reserved bits set for short cutting LFNs for the files. Only drives with
 * these files already on them will work correctly. The files on the drive must
 * be:
 *     NAME      |  FILE CONTENT
 *  TEST1.txt    |    "TEST1"
 *  test2.TXT    |    "TEST2"
 *  test3.txt    |    "TEST3"
 *  TEST4.tx     |    "TEST4"
 *  test5.TX     |    "TEST5"
 *  TEST1234.txt |   "TEST1234"
 */
static int RunWindowsSpecificTests(struct EMULATED_DRIVE *drive)
{
    int test_index;
    uint32_t passed = 0;

    for(test_index=0; test_index < test_count_windows; test_index++){
        if(TestSetup(drive) == true){
            passed += windowsSpecificTests[test_index]();
            TestTearDown();
        }
    }

    return passed;   
}

void RunFunctionalTests(void){
    int passed = RunTests(&DRV096);
    
    printf( "Functional tests: run: %i, passed: %i, failed: %i\r\n", test_count, passed, test_count-passed);
    
    passed = RunWindowsSpecificTests(&DRV_LFN_TESTS);
    
    printf( "Windows Specific LFN tests: run: %i, passed: %i, failed: %i\r\n", test_count_windows, passed, test_count_windows-passed);
}

