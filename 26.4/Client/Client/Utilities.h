#pragma once
#include <cstring>
#include <iostream>

#define ASCII_BITS 8

/*
 * Description: get length of the given string (character or binary string)
 * Input: a character string
 * Output: an int indicate length of that string
 */
uint16_t strlen(uint8_t *str)
{
    uint16_t len = 0;
    char *p = (char *)str;

    while (*p != '\0')
    {
        len++;
        p++;
    }

    return len;
}

/*
 * Description: get numberOfChar characters from string from start
 * Parameters:
 * src: string to get sub string from
 * start: start position
 * numberOfChar: number of characters to gets
 * Output: sub string (char or binary)
 */
uint8_t *subString(uint8_t *src, uint16_t start, uint16_t numberOfChar)
{
    uint8_t *dest;

    dest = new uint8_t[numberOfChar + 1];
    dest[numberOfChar] = '\0';

    for (uint16_t i = 0; i < numberOfChar; i++)
    {
        dest[i] = src[i + start];
    }

    return dest;
}

/*
 * Description: Convert a binary string into an integer
 * Parameters: binary string
 * Output: integer
 */
uint8_t binaryToDecimal(uint8_t *binary)
{
    uint16_t len = strlen(binary);

    uint16_t result = 0, power = 1;

    for (uint16_t i = len - 1; i >= 0; i--)
    {
        if (binary[i] == '1')
        {
            result += power;
        }
        power = power << 1;

        if (i == 0)
        {
            break;
        }
    }
    return result;
}

/*
 * Description: Convert an integer to a binary string
 * Parameters: integer
 * Output: binary string
 */
uint8_t *decimalToBinary(uint8_t decimal, uint8_t outputLength)
{
    uint8_t *result = new uint8_t[outputLength + 1];
    result[outputLength] = '\0';
    memset(result, '0', outputLength);
    uint8_t index = outputLength - 1;

    while (decimal > 0)
    {
        result[index--] = (decimal % 2) + '0';
        decimal = decimal / 2;
    }

    return result;
}

/*
 * Description: Convert a binary string into a character string
 * Parameters: binary string
 * Output: character string
 */
uint8_t *bitsetToString(uint8_t *bits)
{
    // length of bit set array
    uint16_t bitsetLen = strlen(bits);

    // number of characters in string
    uint16_t n = bitsetLen / ASCII_BITS;

    uint8_t *result = new uint8_t[n + 1];
    result[n] = '\0';

    uint8_t *chunk;

    for (uint16_t i = 0; i < n; i++)
    {
        chunk = subString(bits, i * ASCII_BITS, i * ASCII_BITS + ASCII_BITS);
        result[i] = (uint8_t)binaryToDecimal(chunk);
    }

    delete chunk;

    return result;
}

/*
 * Description: Convert a character string into a binary string
 * Parameters: character string
 * Output: binary string
 */
uint8_t *stringToBitset(uint8_t *src)
{
    uint16_t srclength = strlen(src);

    uint8_t *result = new uint8_t[srclength * ASCII_BITS + 1];
    result[srclength * ASCII_BITS] = '\0';
    memset(result, '0', srclength * ASCII_BITS);

    for (uint8_t i = 0; i < srclength; i++)
    {
        uint16_t asciiCode = (uint16_t)src[i];
        uint8_t *binaryString = decimalToBinary(asciiCode, ASCII_BITS);

        for (uint8_t j = 0; j < ASCII_BITS; j++)
        {
            result[j + i * ASCII_BITS] = binaryString[j];
        }

        delete binaryString;
    }

    return result;
}

/*
 * Description: divide string in two halves
 * Parameters: string (char or binary), left half and right half
 * Output: two halves of string save in the two halves parameters
 */
void divideStringToHalves(uint8_t *src, uint8_t *&leftHalf, uint8_t *&rightHalf)
{
    uint16_t n = strlen(src);

    leftHalf = new uint8_t[n / 2 + 1];
    leftHalf[n / 2] = '\0';

    rightHalf = new uint8_t[n / 2 + 1];
    rightHalf[n / 2] = '\0';

    for (uint16_t i = 0; i < n / 2; i++)
    {
        leftHalf[i] = src[i];
        rightHalf[i] = src[i + n / 2];
    }
}

/*
 * Description: concatenate two string into one string
 * Parameters: two strings (binary or character)
 * Output: one string (binary or character)
 */
uint8_t *concatenateString(uint8_t *left, uint8_t *right)
{
    uint16_t nLeft = strlen(left);
    uint16_t nRight = strlen(right);

    uint16_t n = (nLeft + nRight);
    uint8_t *result = new uint8_t[n + 1];
    result[n] = '\0';

    for (uint16_t i = 0; i < nLeft; i++)
    {
        result[i] = left[i];
    }

    for (uint16_t i = nLeft; i < n; i++)
    {
        result[i] = right[i - nLeft];
    }

    return result;
}

uint8_t **splitBlockToChunk(uint8_t *LargeBlock, int nBlock, int blockSize)
{
    uint8_t **blocks = new uint8_t *[nBlock];

    for (int i = 0; i < nBlock; i++)
    {
        blocks[i] = subString(LargeBlock, blockSize * i, blockSize);
    }

    return blocks;
}
