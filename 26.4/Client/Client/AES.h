#pragma once
#include <iostream>
#include <cstring>
#include "AES_Tables.h"

#define AES_BLOCK_SIZE 16
#define AES_ROUNDS 10

uint8_t *AES_Encrypt(uint8_t *message, uint8_t *key);
uint8_t *AES_Decrypt(uint8_t *message, uint8_t *key);
uint8_t *XOR(uint8_t *a, uint8_t *b);