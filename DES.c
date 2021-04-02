#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TEXT_SIZE 64
#define KEY_SIZE 56
#define HALF_TEXT_SIZE 32
#define HALF_KEY_SIZE 28
#define ROUND 16
#define S_BOX_STEP_SIZE 8
#define BOX_BIT_NUM 6

int INITIAL_KEY_SELECTION[] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

int SHIFT_SCHEDULE[] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

int COMPRESSION_PERMUTATION[] = {
    14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

int INITIAL_PERMUTATION[] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

int INVERSE_OF_INITIAL_PERMUTATION[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25
};

int EXPANSION_PERMUTATION[] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

int S_BOX[8][4][16] =
{
    {
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
    },
    {
        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
    },
    {
        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
    },
    {
        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
    },
    {
        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
    },
    {
        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
    },
    {
        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
    },
    {
        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
    }
};

int P_BOX[] = {
    16,  7, 20, 21, 29, 12, 28, 17,
     1, 15, 23, 26,  5, 18, 31, 10,
     2,  8, 24, 14, 32, 27,  3,  9,
    19, 13, 30,  6, 22, 11,  4, 25
};

uint64_t subkey[ROUND];

uint64_t text_permutation(uint64_t text, int matrix[], int size)
{
    uint64_t ground_text = text;
    uint64_t text_with_permutation = 0;
    uint64_t step_bit = 0x8000000000000000;
    uint64_t test_bit = 1;
    int step_size = size / sizeof(int);

    for (int i = 0; i < step_size; i++)
    {
        if ((ground_text >> (TEXT_SIZE - matrix[i])) & test_bit)
        {
            text_with_permutation |= step_bit;
        }
        step_bit >>= 1;
    }

    return text_with_permutation;
}

void generate_key(uint64_t key)
{
    uint64_t selected_key = 0;
    uint64_t key_to_compress = 0;
    uint64_t left_key = 0;
    uint64_t right_key = 0;
    uint64_t top_bit = 0x8000000000000000;
    uint64_t shift_bit = top_bit >> (HALF_KEY_SIZE - 1);

    selected_key = text_permutation(key, INITIAL_KEY_SELECTION, sizeof(INITIAL_KEY_SELECTION));

    left_key = (selected_key >> (TEXT_SIZE - HALF_KEY_SIZE)) << (TEXT_SIZE - HALF_KEY_SIZE);
    right_key = selected_key << HALF_KEY_SIZE;

    for (int j = 0; j < ROUND; j++)
    {
        for (int k = 0; k < SHIFT_SCHEDULE[j]; k++)
        {
            if (left_key & top_bit)
            {
                left_key <<= 1;
                left_key |= shift_bit;
            }
            else
                left_key <<= 1;

            if (right_key & top_bit)
            {
                right_key <<= 1;
                right_key |= shift_bit;
            }
            else
                right_key <<= 1;
        }
        key_to_compress = (left_key | (right_key >> HALF_KEY_SIZE));

        subkey[j] = text_permutation(key_to_compress, COMPRESSION_PERMUTATION, sizeof(COMPRESSION_PERMUTATION));

    }
}

uint64_t s_box(uint64_t text)
{
    uint64_t box[S_BOX_STEP_SIZE];
    uint64_t step_bit = 0xFC00000000000000;
    uint64_t output_bit = 0;
    int row, col;

    for (int i = 0; i < S_BOX_STEP_SIZE; i++)
    {
        box[i] = (text & step_bit) >> (TEXT_SIZE - (BOX_BIT_NUM * (i + 1)));
        row = ((box[i] & 0x20) >> 4) | (box[i] & 1);
        col = (box[i] & 0x1E) >> 1;
        output_bit |= (uint64_t)S_BOX[i][row][col] << (TEXT_SIZE - (4 * (i + 1)));
        step_bit >>= BOX_BIT_NUM;
    }

    return output_bit;
}

uint64_t DES_round(uint64_t text, int decrypt_flag)
{
    uint64_t text_passed_round = 0;
    uint64_t expanded_text = 0;
    uint64_t s_box_text = 0;
    uint64_t s_box_output = 0;
    uint64_t F = 0;
    uint64_t left_text = 0;
    uint64_t right_text = 0;
    uint64_t temp_text = 0;

    left_text = (text >> HALF_TEXT_SIZE) << HALF_TEXT_SIZE;
    right_text = text << HALF_TEXT_SIZE;

    for (int i = 0; i < ROUND; i++)
    {
        temp_text = right_text;
        expanded_text = text_permutation(right_text, EXPANSION_PERMUTATION, sizeof(EXPANSION_PERMUTATION)); // 32 -> 48
        if(decrypt_flag)
            s_box_text = (expanded_text ^ subkey[ROUND - i - 1]);
        else
            s_box_text = (expanded_text ^ subkey[i]);
        s_box_output = s_box(s_box_text); // 48 -> 32
        F = text_permutation(s_box_output, P_BOX, sizeof(P_BOX));
        right_text = left_text ^ F;
        left_text = temp_text;
    }

    text_passed_round = (left_text | (right_text >> HALF_TEXT_SIZE));

    return text_passed_round;
}

uint64_t swap_32bit(uint64_t text)
{
    uint64_t swapped_text = 0;

    swapped_text |= text << HALF_TEXT_SIZE;
    swapped_text |= text >> HALF_TEXT_SIZE;

    return swapped_text;
}

uint64_t DES(uint64_t plaintext, uint64_t key, int decrypt_flag)
{
    uint64_t cipher_text;
    uint64_t text_buffer;
    
    if(!decrypt_flag)
        generate_key(key);

    text_buffer = text_permutation(plaintext, INITIAL_PERMUTATION, sizeof(INITIAL_PERMUTATION));
    text_buffer = DES_round(text_buffer, decrypt_flag);
    text_buffer = swap_32bit(text_buffer);
    cipher_text = text_permutation(text_buffer, INVERSE_OF_INITIAL_PERMUTATION, sizeof(INVERSE_OF_INITIAL_PERMUTATION));

    return cipher_text;
}

void main()
{
    uint64_t plaintext = 0x0123456789ABCDEF;
    uint64_t key = 0x85E813540F0AB405;
    uint64_t cipher_text;

    printf("Plain Text  : %016llX \n", plaintext);
    printf("Key         : %016llX \n", key);

    cipher_text = DES(plaintext, key, 0);

    printf("Cipher Text : %016llX \n", cipher_text);
    printf("Decrypted Cipher Text : %016llX \n", DES(cipher_text, key, 1));
}