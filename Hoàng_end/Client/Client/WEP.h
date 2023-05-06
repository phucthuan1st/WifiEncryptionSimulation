#pragma once
#include<iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include<cstring>
#include <string>
#include <sstream>
// Key and IV sizes for WEP
#define KEY_SIZE 5
#define IV_SIZE 3
using namespace std;
//void wep_encrypt(unsigned char* key, unsigned char* iv, unsigned char* plaintext, int length);
//void wep_decrypt(unsigned char* key, unsigned char* iv, unsigned char* ciphertext, int length);
//void crc32(const unsigned char* data, int length, unsigned char* checksum);
void printHex(const unsigned char* data, int len);
unsigned char* generateKeystream(const string& wepKey, const unsigned char* iv);
void encryptDecrypt(unsigned char* data, int length, const unsigned char* keystream);
unsigned char* convertPlaintext(string plaintext, int len);
string initChatText(string* plaintext);
void charToUint8_t(const char* str, uint8_t* arr, size_t len);
void copyCStringtoUint8(CString input, uint8_t* output, int length);

//using namespace std;
//
//void printHex(const unsigned char *data, int len)
//{
//    stringstream ss;
//    ss << hex;
//    for (int i = 0; i < len; ++i)
//        ss << uppercase << static_cast<int>(data[i]);
//    cout << ss.str() << endl;
//}
//// tao key
//unsigned char *generateKeystream(const string &wepKey, const unsigned char *iv)
//{
//    unsigned char S[256], T[256];
//    int keyLength = wepKey.length();
//    int j = 0, t = 0;
//    for (int i = 0; i < 256; i++)
//    {
//        S[i] = i;
//        T[i] = wepKey[i % keyLength];
//    }
//    for (int i = 0; i < 256; i++)
//    {
//        j = (j + S[i] + T[i]) % 256;
//        swap(S[i], S[j]);
//    }
//    int i = 0;
//    j = 0;
//    unsigned char* keystream = new unsigned char[1024];
//    int keystreamLength = sizeof(keystream) / sizeof(keystream[0]);
//    for (int k = 0; k < keystreamLength; k++)
//    {
//        i = (i + 1) % 256;
//        j = (j + S[i]) % 256;
//        swap(S[i], S[j]);
//        keystream[k] = S[(S[i] + S[j]) % 256];
//    }
//    return keystream;
//}
//// ma hoa va giai ma 
//void encryptDecrypt(unsigned char *data, int length, const unsigned char *keystream)
//{
//    for (int i = 0; i < length; i++)
//    {
//        data[i] ^= keystream[i];
//    }
//}
//
////convert plaintex
//unsigned char* convertPlaintext(string plaintext, int len) {
//    unsigned char* message = new unsigned char[len+1];
//    memcpy(message, plaintext.c_str(), len+1);
//    message[len] = '\0';
//    return message;
//}
//
////dung de lay tin nhan cua nguoi dung r tao ra chuoi
//string initChatText(string *plaintext) {
//    getline(cin,*plaintext);
//    return *plaintext;
//}

//int main()
//{
//    // Khóa WEP và vector khởi tạo (IV)
//    string wepKey = "SecretWEPKey";
//    unsigned char iv[3] = {0x12, 0x34, 0x56};
//    // Tạo RC4 keystream từ IV và khóa WEP
//    unsigned char* keystream = generateKeystream(wepKey, iv);
//    // Mã hóa và giải mã với WEP
//    string plaintext = "";
//    int messageLength = initChatText(&plaintext).length();
//    unsigned char* message = convertPlaintext(plaintext, messageLength);
//    // Mã hóa với WEP
//    encryptDecrypt(message, messageLength, keystream);
//
//    // In ra bản mã hóa
//    cout << "Ciphertext: ";
//    printHex(message, messageLength);
//    // Giải mã với WEP
//    encryptDecrypt(message, messageLength, keystream);
//
//    // In ra bản giải mã
//    cout << "Decryptedtext: " << message << endl;
//
//
//    // loi cho nay ne
//    delete message;
//    return 0;
//}
