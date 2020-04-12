#ifndef MD5_calculator
#define MD5_calculator

enum MD5Status {MD5_OK, MD5_mallocE, MD5_paramE, MD5_fopenE};

typedef enum MD5Status MD5Status;

MD5Status calculateMD5(char* filePath, unsigned char** MD5);

#endif