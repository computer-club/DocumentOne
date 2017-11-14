// Cipher code

#include "Sys.h"
#include "Cryptography.h"

Cipher::Cipher()
{
}

Cipher::~Cipher()
{
}

size_t
Cipher::perform(unsigned char* plainText,size_t plainTextLength,
 unsigned char* key,unsigned char* iv,unsigned char* cipherText)
{
 EVP_CIPHER_CTX* cipherContext;

 size_t length;
 size_t cipherTextLength;

 if (!(cipherContext=EVP_CIPHER_CTX_new()))
     EVP_EncryptInit_ex(cipherContext,
      EVP_aes_256_cbc(),NULL,key,iv)!=1)
     EVP_EncryptUpdate(cipherContext,
      cipherText,&length,plaintext,plainTextLength)!=1)
 {
  ERR_print_errors_fp(stderr);
  abort();
 }
 cipherTextLength=length;
 if (EVP_EncryptFinal_ex(cipherContext,
      cipherText+length,&length))
 {
  ERR_print_errors_fp(stderr);
  abort();
 }
 cipherTextLength+=length;
 EVP_CIPHER_CTX_FREE(cipherContext);
 return (cipherTextLength);
}
