#include "AES.h"
#include "Utilities.h"

void incrementCounter(uint8_t *counter)
{
    for (int i = AES_BLOCK_SIZE - 1; i >= 0; i--)
    {
        if (++counter[i] != 0)
            break;
    }
}

void CBCCtrEnc(uint8_t *iv, uint8_t *message, uint8_t *key)
{
    uint16_t length = strlen(message);
    if (length == 0)
        return;

    int nBlock = (length + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;

    uint8_t **blocks = splitBlockToChunk(message, nBlock, AES_BLOCK_SIZE);

    uint8_t *ciphertexts = new uint8_t[length + 1]{0};
    uint8_t *ciphertextChunk = new uint8_t[AES_BLOCK_SIZE + 1]{0};

    uint8_t *counter = new uint8_t[AES_BLOCK_SIZE + 1]{0};
    memcpy(counter, iv, AES_BLOCK_SIZE);

    for (int i = 0; i < nBlock; i++)
    {
        incrementCounter(counter);

        uint8_t *plaintext = blocks[i];
        memcpy(ciphertextChunk, XOR(AES_Encrypt(counter, key), plaintext), AES_BLOCK_SIZE);
        ciphertexts = concatenateString(ciphertexts, ciphertextChunk);
    }

    memcpy(message, ciphertexts, length);
    delete[] blocks;
    delete[] ciphertexts;
    delete[] ciphertextChunk;
    delete[] counter;
}

void CBCCtrDec(uint8_t *iv, uint8_t *encryptedMessage, uint8_t *key)
{
    uint16_t length = strlen(encryptedMessage);
    if (length == 0)
        return;

    int nBlock = (length + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;

    uint8_t **blocks = splitBlockToChunk(encryptedMessage, nBlock, AES_BLOCK_SIZE);

    uint8_t *plaintexts = new uint8_t[length + 1]{0};
    uint8_t *plaintextChunk = new uint8_t[AES_BLOCK_SIZE + 1]{0};

    uint8_t *counter = new uint8_t[AES_BLOCK_SIZE + 1]{0};
    memcpy(counter, iv, AES_BLOCK_SIZE);

    for (int i = 0; i < nBlock; i++)
    {
        incrementCounter(counter);

        uint8_t *ciphertext = blocks[i];
        memcpy(plaintextChunk, XOR(AES_Encrypt(counter, key), ciphertext), AES_BLOCK_SIZE);
        plaintexts = concatenateString(plaintexts, plaintextChunk);
    }

    memcpy(encryptedMessage, plaintexts, length);
    delete[] blocks;
    delete[] plaintexts;
    delete[] plaintextChunk;
    delete[] counter;
}

int main()
{
    uint8_t key[] = "Thats my Kung Fu";
    uint8_t message[] = "Two One Nine Two";
    uint8_t iv[AES_BLOCK_SIZE] = {0};

    printf("\tMessage:\t");
    for (int i = 0; i < strlen(message); i++)
    {
        printf("%02x ", message[i]);
    }
    printf("\n");

    CBCCtrEnc(iv, message, key);
    printf("Encrypted Message:\t");
    for (int i = 0; i < strlen(message); i++)
    {
        printf("%02x ", message[i]);
    }
    printf("\n");

    CBCCtrDec(iv, message, key);
    printf("Decrypted Message:\t");
    for (int i = 0; i < strlen(message); i++)
    {
        printf("%02x ", message[i]);
    }
    printf("\n");

    return 0;
}