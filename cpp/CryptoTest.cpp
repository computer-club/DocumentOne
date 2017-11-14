#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

#include "Sys.h"
#include "Cryptography.h"

int main(int argc,char* argv[])
{
 unsigned char* key=(unsigned char*)"01234567890123456789012345678901";
 unsigned char* iv=(unsigned char*)"0123456789012345";
 unsigned char* plainText=(unsigned char*)
  "The quick brown fox jumps over the lazy dog";

 unsigned char cipherText[128];
 size_t cipherTextLength;
 unsigned char decryptedText[128];
 size_t decryptedTextLength;
 
 ERR_load_crypto_strings();
 OpenSSL_add_all_algorithms();
 OPENSSL_config(NULL);

 Cipher cipher;

 cipherTextLength=cipher.encrypt(plainText,strlen((char*)plainText),
  key,iv,cipherText);
 printf("Ciphertext is:\n");
 BIO_dump_fp(stdout,(const char*)cipherText,cipherTextLength);
}
