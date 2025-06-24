

#ifndef CAESARLOAD_H
#define CAESARLOAD_H

#include <windows.h>
#include <iostream>



class Caesarload {
    HINSTANCE dll = nullptr;
    typedef char* (*crypt_func)(char*);
    crypt_func encrypt_func = nullptr;
    crypt_func decrypt_func = nullptr;

public:
    Caesarload(const std::string& dllPath) {
        dll = LoadLibraryA(dllPath.c_str());
        if (!dll) {
            std::cerr << "Failed to load DLL: " << dllPath << "\n";
            return;
        }
        encrypt_func = (crypt_func)GetProcAddress(dll, "encrypt");
        decrypt_func = (crypt_func)GetProcAddress(dll, "decrypt");

        if (!encrypt_func || !decrypt_func) {
            std::cerr << "Failed to load functions\n";
            FreeLibrary(dll);
            dll = nullptr;
        }
    }

    ~Caesarload() {
        if (dll) FreeLibrary(dll);
    }

    bool valid() const { return dll && encrypt_func && decrypt_func; }

    std::string encrypt(const std::string& input) {
        char* buffer = new char[input.size() + 1];
        strcpy(buffer, input.c_str());
        char* result = encrypt_func(buffer);
        std::string output(result);
        delete[] buffer;
        return output;
    }

    std::string decrypt(const std::string& input) {
        char* buffer = new char[input.size() + 1];
        strcpy(buffer, input.c_str());
        char* result = decrypt_func(buffer);
        std::string output(result);
        delete[] buffer;
        return output;
    }
};

#endif //CAESARLOAD_H
