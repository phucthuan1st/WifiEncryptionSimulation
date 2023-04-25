#include "AES.h"
#include "Utilities.h"

uint8_t *CBCRandomEnc(uint8_t *message, uint8_t *key)
{
    uint16_t length = strlen(message);
    if (length % AES_BLOCK_SIZE != 0 || length == 0)
        return NULL;

    int nBlock = length / AES_BLOCK_SIZE;

    uint8_t **blocks = splitBlockToChunk(message, nBlock, AES_BLOCK_SIZE);

    uint8_t *ciphertexts = new uint8_t[length + 1]{0};
    uint8_t *ciphertextChunk = new uint8_t[AES_BLOCK_SIZE + 1]{0};

    for (int i = 0; i < nBlock; i++)
    {
        uint8_t *plaintext = blocks[i];
        memcpy(ciphertextChunk, AES_Encrypt(XOR(ciphertextChunk, plaintext), key), AES_BLOCK_SIZE);
        ciphertexts = concatenateString(ciphertexts, ciphertextChunk);
    }

    return ciphertexts;
}

uint8_t *CBCRandomDec(uint8_t *IV, uint8_t *encryptedMessage, uint8_t *key)
{
    uint16_t length = strlen(encryptedMessage);
    if (length % AES_BLOCK_SIZE != 0 || length == 0)
        return NULL;

    int nBlock = length / AES_BLOCK_SIZE;

    uint8_t **blocks = splitBlockToChunk(encryptedMessage, nBlock, AES_BLOCK_SIZE);

    uint8_t *plaintexts = new uint8_t[length + 1]{0};
    uint8_t *plaintextChunk = new uint8_t[AES_BLOCK_SIZE + 1]{0};

    uint8_t *ciphertextChunk = IV;
    for (int i = 0; i < nBlock; i++)
    {
        uint8_t *prevChunk = ciphertextChunk;
        ciphertextChunk = blocks[i];
        memcpy(plaintextChunk, XOR(AES_Decrypt(ciphertextChunk, key), prevChunk), AES_BLOCK_SIZE);
        plaintexts = concatenateString(plaintexts, plaintextChunk);
    }

    return plaintexts;
}

int main()
{
    uint8_t key[] = "Thats my Kung Fu";
    uint8_t message[] = "Two One Nine Two";

    printf("\tMessage:\t");
    for (int i = 0; i < strlen(message); i++)
    {
        printf("%02x ", message[i]);
    }
    printf("\n");

    uint8_t *encryptedMessage = CBCRandomEnc(message, key);
    printf("Encrypted Message:\t");
    for (int i = 0; i < strlen(encryptedMessage); i++)
    {
        printf("%02x ", encryptedMessage[i]);
    }
    printf("\n");

    uint8_t *IV = new uint8_t[AES_BLOCK_SIZE + 1]{0};
    uint8_t *decryptedMessage = CBCRandomDec(IV, encryptedMessage, key);
    printf("Decrypted Message:\t");
    for (int i = 0; i < strlen(decryptedMessage); i++)
    {
        printf("%02x ", decryptedMessage[i]);
    }
    printf("\n");

    delete IV;
    delete encryptedMessage;
    delete decryptedMessage;

    return 0;
}