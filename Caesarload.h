

#ifndef CAESARLOAD_H
#define CAESARLOAD_H

#include <windows.h>
#include <iostream>
#include <cstring>



class Caesarload {
    HINSTANCE dll = nullptr;
    typedef char* (*crypt_func)(char*, int);
    crypt_func encrypt_func = nullptr;
    crypt_func decrypt_func = nullptr;
    int key;

public:
    Caesarload(const std::string& dllPath, int key) : key(key) {
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
        char* buf = new char[input.size() + 1];
        std::strcpy(buf, input.c_str());
        char* res = encrypt_func(buf, key);
        std::string out(res);
        delete[] buf;
        return out;
    }

    std::string decrypt(const std::string& input) {
        char* buf = new char[input.size() + 1];
        std::strcpy(buf, input.c_str());
        char* res = decrypt_func(buf, key);
        std::string out(res);
        delete[] buf;
        return out;
    }


};

#endif //CAESARLOAD_H
