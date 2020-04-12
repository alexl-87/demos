#ifndef DirWalk
#define DirWalk

enum DirWalkStatus {Dir_OK, Dir_ParamE, dirE};

typedef enum DirWalkStatus DirWalkStatus;
typedef void (*onFileFoundedAction)(char* filePath);

DirWalkStatus getFiles(char* dirPath, onFileFoundedAction userFunc);



#endif