#ifndef H_CRYPTOGRAPHY
#define H_CRYPTOGRAPHY

class Cipher
{
// public types
public:

 enum Mode
 {
  Encrypt,
  Decrypt
 };

// private types
private:

 enum Phase
 {
  PhaseUnprepared,
  PhaseProcessing
 };

// private data
private:

// public functions
public:
 Cipher();
 ~Cipher();

 size_t perform(unsigned char* plainText,size_t plainTextLength,
  unsigned char* key,unsigned char* iv,unsigned char* cipherText);
};

#endif
