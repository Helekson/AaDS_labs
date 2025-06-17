#ifndef ENCODER_H
#define ENCODER_H

class encoder {
private:
    unsigned char* key;
    size_t key_length;

    void KSA(unsigned char* S);
    void PRGA(unsigned char* S, unsigned char* keystream, size_t data_length);
    void process_file(const char* input_path, const char* output_path, bool is_encrypt);

public:
    encoder(const unsigned char* key, size_t length);
    ~encoder();
    void set_key(const unsigned char* new_key, size_t length);
    void encode(const char* input_path, const char* output_path, bool encrypt);
    encoder(const encoder& other) = delete;
    encoder& operator=(const encoder& other) = delete;
};

#endif
