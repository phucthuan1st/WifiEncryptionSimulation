#include"stdafx.h"
#include"WEP.h"

using namespace std;
void printHex(const unsigned char* data, int len)
{
    stringstream ss;
    ss << hex;
    for (int i = 0; i < len; ++i)
        ss << uppercase << static_cast<int>(data[i]);
    cout << ss.str() << endl;
}
// tao key
unsigned char* generateKeystream(const string& wepKey, const unsigned char* iv)
{
    unsigned char S[256], T[256];
    int keyLength = wepKey.length();
    int j = 0, t = 0;
    for (int i = 0; i < 256; i++)
    {
        S[i] = i;
        T[i] = wepKey[i % keyLength];
    }
    for (int i = 0; i < 256; i++)
    {
        j = (j + S[i] + T[i]) % 256;
        swap(S[i], S[j]);
    }
    int i = 0;
    j = 0;
    unsigned char* keystream = new unsigned char[1024];
    int keystreamLength = sizeof(keystream) / sizeof(keystream[0]);
    for (int k = 0; k < keystreamLength; k++)
    {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        swap(S[i], S[j]);
        keystream[k] = S[(S[i] + S[j]) % 256];
    }
    return keystream;
}
// ma hoa va giai ma 
void encryptDecrypt(unsigned char* data, int length, const unsigned char* keystream)
{
    for (int i = 0; i < length; i++)
    {
        data[i] ^= keystream[i];
    }
}

//convert plaintex
unsigned char* convertPlaintext(string plaintext, int len) {
    unsigned char* message = new unsigned char[len + 1];
    memcpy(message, plaintext.c_str(), len + 1);
    message[len] = '\0';
    return message;
}

//dung de lay tin nhan cua nguoi dung r tao ra chuoi
string initChatText(string* plaintext) {
    getline(cin, *plaintext);
    return *plaintext;
}

void charToUint8_t(const char* str, uint8_t* arr, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        arr[i] = static_cast<uint8_t>(str[i]);
    }
}
void copyCStringtoUint8(CString input, uint8_t* output, int length) {
    for (int i = 0; i < length; i++) {
        output[i] = input[i];
    }
}