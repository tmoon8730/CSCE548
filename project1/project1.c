#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{

	unsigned char iv[16] = {0}; // Given

	char words[16], t; // Each word in the dictionary
	FILE *key, *outFile;
	unsigned char outbuf[1024 + EVP_MAX_BLOCK_LENGTH];

	int outlen, tmplen;
	int num;

	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	char inText[] = "This is a top secret."; // Given
	char cipherText[] = "8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9"; // given
	key = fopen("words.txt", "r"); // Given
	if(remove("output.txt") == -1){
		perror("Error deleting file");
	}
	outFile = fopen("output.txt", "a+"); // Open the matching file with append mode
	if( key < 0 || outFile < 0){
		perror("Connot open file");
		exit(1);
	}



	char pbuffer[1024];
	int i;
	while(fgets(words, 16, key)) {

		i = strlen(words); // number of words
		words[i-1]='\0'; // Remove null or end of file characters
		i = strlen(words); // Set the value again
		if(i < 16){ // Use 16 because of AES-128
			//pad(words, (16)); // Need to add space chars to the end of words that are shorter
					 						  // than 16 characters long
			int l;
			l = strlen(words);
			while(l<16){
				words[l] = ' '; // Insert a space
				l++;
			}
			words[l] = '\0'; // terminate the string with a null		
		}
		EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, words, iv); // Set the encryption method 
									     																					  // to aes 128 cbc
		if(!EVP_EncryptUpdate(&ctx, outbuf, &outlen, inText, strlen(inText))){
			EVP_CIPHER_CTX_cleanup(&ctx);
			return 0;
		}
		if(!EVP_EncryptFinal_ex(&ctx, outbuf + outlen, &tmplen)){
			EVP_CIPHER_CTX_cleanup(&ctx);
			return 0;
		}
		outlen += tmplen;

		int i;
		char* buf_str = (char*) malloc (2*outlen + 1);
		char* buf_ptr = buf_str;
		for(i=0;i<outlen;i++){
			buf_ptr += sprintf(buf_ptr, "%02X", outbuf[i]);
		}
		*(buf_ptr + 1) = '\0';

		// Compare against the known decrypted value
		int stringCheck;
		char const *a, *b;
		a = cipherText;
		b = buf_str;
		for(;; a++, b++){
			int d = tolower(*a) - tolower(*b);
			if(d != 0 || !*a){
				stringCheck = d;
				break;
			}
		}
		// If the value is 0 then it matches and the correct key was found
		if(stringCheck == 0){
			int q,z;
			for(q=0;q<strlen(words);q++)
				fprintf(outFile,"%c",words[q]);
			fprintf(outFile,"%c",'\n');
		}
	}
	fclose(key);
	fclose(outFile);
	return 0;
}
