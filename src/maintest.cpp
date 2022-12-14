#include "maintest.h"
#include <stdarg.h> //windows&linux for va_start
#include <OcrLiteCApi.h>
#include <stdexcept>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#endif

void Logger(const char *format, ...) {
    char *buffer = (char *) malloc(8192);
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    printf("%s", buffer);
    free(buffer);
}

int main(int argc, char *argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    //gpuIndex: -1 means use cpu, 0 means use gpu0, 1 means use gpu1...
    OCR_HANDLE handle = OcrInit(DET_MODEL, CLS_MODEL, REC_MODEL, KEY_FILE, THREAD_NUM,0);
    if (!handle) {
        printf("cannot initialize the OCR Engine.\n");
        return -1;
    }
    OCR_PARAM param = {0};
    OCR_BOOL bRet = OcrDetect(handle, DEFAULT_IMG_PATH, DEFAULT_IMG_NAME, &param);
    if (bRet) {
        int nLen = OcrGetLen(handle);
        if (nLen > 0) {
            char *szInfo = (char *) malloc(nLen);
            if (szInfo) {
                if (OcrGetResult(handle, szInfo, nLen)) {
                    printf("%s", szInfo);
                }
                free(szInfo);
            }
        }
    }
    if (handle) {
        OcrDestroy(handle);
    }
    return 0;
}
