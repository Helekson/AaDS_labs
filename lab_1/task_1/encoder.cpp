#include "encoder.h"
#include <cstdio>
#include <cstdlib>

encoder::encoder(const unsigned char* key, size_t length) : key_length(length) {
    this->key = new unsigned char[length];
    for (size_t i = 0; i < length; ++i) {
        this->key[i] = key[i];
    }
}

encoder::~encoder() {
    delete[] key;
}

void encoder::set_key(const unsigned char* new_key, size_t length) {
    delete[] key;
    key_length = length;
    key = new unsigned char[length];
    for (size_t i = 0; i < length; ++i) {
        key[i] = new_key[i];
    }
}

void encoder::KSA(unsigned char* S) {
    for (int i = 0; i < 256; ++i) {
        S[i] = i;
    }
    int j = 0;
    for (int i = 0; i < 256; ++i) {
        j = (j + S[i] + key[i % key_length]) % 256;
        std::swap(S[i], S[j]);
    }
}

void encoder::PRGA(unsigned char* S, unsigned char* keystream, size_t data_length) {
    int i = 0, j = 0;
    for (size_t n = 0; n < data_length; ++n) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        std::swap(S[i], S[j]);
        keystream[n] = S[(S[i] + S[j]) % 256];
    }
}

void encoder::process_file(const char* input_path, const char* output_path, bool is_encrypt) {
    FILE* input = fopen(input_path, "rb");
    if (!input) throw std::runtime_error("Input file error");

    FILE* output = fopen(output_path, "wb");
    if (!output) {
        fclose(input);
        throw std::runtime_error("Output file error");
    }

    unsigned char S[256];
    KSA(S);

    fseek(input, 0, SEEK_END);
    size_t data_length = ftell(input);
    fseek(input, 0, SEEK_SET);

    unsigned char* data = new unsigned char[data_length];
    unsigned char* keystream = new unsigned char[data_length];

    fread(data, 1, data_length, input);
    PRGA(S, keystream, data_length);

    for (size_t i = 0; i < data_length; ++i) {
        data[i] ^= keystream[i];
    }

    fwrite(data, 1, data_length, output);

    delete[] data;
    delete[] keystream;
    fclose(input);
    fclose(output);
}

void encoder::encode(const char* input_path, const char* output_path, bool encrypt) {
    process_file(input_path, output_path, encrypt);
}
