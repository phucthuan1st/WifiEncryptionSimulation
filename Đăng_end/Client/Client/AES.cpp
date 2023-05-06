#include"stdafx.h"
#include "AES.h"
int getBit(int n, int pos)
{
    int res = (n >> (31 - pos)) & 1;
    return res;
}

void setBit(int &n, int pos, int val)
{
    if (val)
        n |= (1 << (31 - pos));
    else
        n &= (~(1 << (31 - pos)));
}

int leftRotate(int n, int val)
{
    for (int i = 0; i < val; i++)
    {
        int bit = getBit(n, 0);
        n <<= 1;
        setBit(n, 31, bit);
    }

    return n;
}

int *byteToFourInts(uint8_t *byte)
{
    int *ints = new int[4]{0};

    for (int i = 0; i < 4; i++)
    {
        ints[i] = byte[4 * i] << 24 |
                  byte[4 * i + 1] << 16 |
                  byte[4 * i + 2] << 8 |
                  byte[4 * i + 3];
    }

    return ints;
}

uint8_t *fourIntsToByte(int ints[4])
{
    uint8_t *bytes = new uint8_t[AES_BLOCK_SIZE + 1]{0};

    for (int i = 0; i < 4; i++)
    {
        bytes[4 * i] = (uint8_t)(ints[i] >> 24);
        bytes[4 * i + 1] = (uint8_t)(ints[i] >> 16);
        bytes[4 * i + 2] = (uint8_t)(ints[i] >> 8);
        bytes[4 * i + 3] = (uint8_t)(ints[i]);
    }

    return bytes;
}

int sbox(int c, uint8_t *box)
{
    uint8_t temp[4]{0};
    memcpy(temp, &c, 4);

    for (int i = 0; i < 4; i++)
        temp[i] = box[temp[i]];

    int res = 0;
    memcpy(&res, temp, 4);

    return res;
}

uint8_t **keySchedule(uint8_t *K)
{
    uint8_t **roundKeys = new uint8_t *[AES_ROUNDS]
    { 0 };

    int *Ki = byteToFourInts(K);
    for (int i = 1; i <= AES_ROUNDS; i++)
    {
        Ki[0] = Ki[0] ^ sbox(leftRotate(Ki[3], 8), SBOX) ^ RC[i];
        Ki[1] = Ki[1] ^ Ki[0];
        Ki[2] = Ki[2] ^ Ki[1];
        Ki[3] = Ki[3] ^ Ki[2];

        roundKeys[i - 1] = fourIntsToByte(Ki);
    }

    return roundKeys;
}

uint8_t *XOR(uint8_t *a, uint8_t *b)
{
    uint8_t *res = new uint8_t[AES_BLOCK_SIZE + 1]{0};

    for (int i = 0; i < AES_BLOCK_SIZE; i++)
    {
        res[i] = a[i] ^ b[i];
    }

    return res;
}

void addRoundKey(uint8_t *state, uint8_t *roundKey)
{
    for (int i = 0; i < AES_BLOCK_SIZE; i++)
        state[i] ^= roundKey[i];
}

void subBytes(uint8_t *state, uint8_t *box)
{
    for (int i = 0; i < AES_BLOCK_SIZE; i++)
        state[i] = box[state[i]];
}

void shiftRow(uint8_t *state, uint8_t *permutation)
{
    uint8_t temp[AES_BLOCK_SIZE + 1]{0};
    memcpy(temp, state, AES_BLOCK_SIZE);

    for (int i = 0; i < AES_BLOCK_SIZE; i++)
        temp[i] = state[permutation[i]];

    memcpy(state, temp, AES_BLOCK_SIZE);
}

//! Reference: https://www.samiam.org/galois.html
uint8_t gmul(uint8_t a, uint8_t b)
{
    uint8_t product = 0;

    for (int i = 0; i < 8; i++)
    {
        if ((b & 1) == 1)
            product ^= a;
        uint8_t hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set == 0x80)
            a ^= 0x1b;
        b >>= 1;
    }
    return product;
}

//! Reference: https://www.samiam.org/mix-column.html
void mixSingleColumn(uint8_t *col, uint8_t matrix[4][4])
{
    uint8_t temp[4]{0};

    for (int i = 0; i < 4; i++)
    {
        temp[i] = col[i];
        col[i] = 0;
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            col[i] ^= gmul(temp[j], matrix[i][j]);
        }
    }
}

void mixColumns(uint8_t *state, uint8_t matrix[4][4])
{
    uint8_t temp[4]{0};

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            temp[j] = state[4 * i + j];

        mixSingleColumn(temp, matrix);

        for (int j = 0; j < 4; ++j)
            state[4 * i + j] = temp[j];
    }
}

uint8_t *AES_Encrypt(uint8_t *message, uint8_t *key)
{
    uint8_t **roundKeys = keySchedule(key);

    addRoundKey(message, key);

    for (int i = 1; i <= AES_ROUNDS; i++)
    {
        subBytes(message, SBOX);
        shiftRow(message, ROW_PERMUTATION);

        if (i < AES_ROUNDS)
            mixColumns(message, AX);

        addRoundKey(message, roundKeys[i - 1]);
    }

    return message;
}

uint8_t *AES_Decrypt(uint8_t *message, uint8_t *key)
{
    uint8_t **roundKeys = keySchedule(key);

    addRoundKey(message, roundKeys[AES_ROUNDS - 1]);

    for (int i = AES_ROUNDS - 1; i >= 0; i--)
    {
        shiftRow(message, INV_ROW_PERMUTATION);
        subBytes(message, INVERSE_SBOX);

        if (i > 0)
        {
            addRoundKey(message, roundKeys[i - 1]);
            mixColumns(message, INV_AX);
        }
    }

    addRoundKey(message, key);

    return message;
}
