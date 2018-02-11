#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <string.h>

int main(int argc, char** argv)
{
	FILE *ifp = fopen(argv[1], "r");
	FILE *ofp = fopen(argv[2], "w");

	int bytes_read, bytes_written;
	unsigned char indata[AES_BLOCK_SIZE];
	unsigned char outdata[AES_BLOCK_SIZE];

	unsigned char ckey[] = "thiskeyisveybad";
	unsigned char iv[] = "dontusethisinput";

	AES_KEY key;

	AES_set_encrypt_key(ckey, 128, &key);

	int num = 0;

	while(1) {
		bytes_read = fread(indata, 1, AES_BLOCK_SIZE, ifp);
		AES_cfb128_encrypt(indata, outdata, bytes_read, &key, iv, &num, AES_DECRYPT);
    bytes_written = fwrite(outdata, 1, bytes_read, ofp);
		if(bytes_read < AES_BLOCK_SIZE)
			break;
	}
	return 1;
}




	/*FILE *in = fopen(argv[1], "r");
	FILE *out = fopen(argv[2], "w");
	int do_encrypt = (int)argv[3];
	printf("Got the files");

  unsigned char inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
	int inlen, outlen;
  EVP_CIPHER_CTX ctx;
 	unsigned char key[] = "0123456789abcdeF";
	unsigned char iv[] = "1234567887654321";
	
	EVP_CIPHER_CTX_init(&ctx);
	EVP_CipherInit_ex(&ctx, EVP_aes_128_cbc(), NULL, NULL, NULL, do_encrypt);
	OPENSSL_assert(EVP_CIPHER_CTX_key_length(&ctx) == 16);
	OPENSSL_assert(EVP_CIPHER_CTX_iv_length(&ctx) == 16);

	EVP_CipherInit_ex(&ctx, NULL, NULL, key, iv, do_encrypt);

	for(;;){
		inlen = fread(inbuf, 1, 1024, in);
		if(inlen <= 0) break;
		if(!EVP_CipherUpdate(&ctx, outbuf, &outlen, inbuf, inlen)){
			EVP_CIPHER_CTX_cleanup(&ctx);
			return 0;
		}
		fwrite(outbuf, 1, outlen, out);
		printf("Writing output");
	}
	if(!EVP_CipherFinal_ex(&ctx, outbuf, &outlen))
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		return 0;
	}
	fwrite(outbuf, 1, outlen, out);

	EVP_CIPHER_CTX_cleanup(&ctx);
  return 1;
}*/
