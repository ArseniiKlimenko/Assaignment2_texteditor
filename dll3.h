#ifndef ASSAIGNMENT3_LIBRARY_H
#define ASSAIGNMENT3_LIBRARY_H

#ifdef _WIN32
  #ifdef BUILDING_DLL
    #define DLL __declspec(dllexport)
  #else
    #define DLL __declspec(dllimport)
  #endif
#else
  #define DLL
#endif

#ifdef __cplusplus
extern "C" {
#endif

    DLL char* encrypt(char* rawText);
    DLL char* decrypt(char* encryptedText);

#ifdef __cplusplus
}
#endif

#endif