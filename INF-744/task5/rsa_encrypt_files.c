/*
 * File encryption tool.
 *
 * Usage: auth_encrypt_files [-e|-d] input_file output_file
 * Modified by: Danilo Arthur Bertelli to include RSA method for files (removing image encryption)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include <arpa/inet.h> /* For htonl() */

/* Program arguments. */
FILE *input, *output, *keyFile;

/* Main program. */
int main(int argc, char *argv[]) {
	int encrypt = 0;

	/* Check number of arguments. */
	if (argc != 5) {
		fprintf(stderr,
				"\nUsage: auth_encrypt_image [-e|-d] [-ECB|-CTR|-CBC|-GCM|-RSA] key input_image output_image\n");
		exit(1);
	}
	/* Check mode. */
	if (argv[1][0] != '-' || strlen(argv[1]) != 2) {
		fprintf(stderr, "\nUse -e to encrypt and -d to decrypt..\n");
		exit(1);
	}
	encrypt = (argv[1][1] == 'e');

	/* Check files. */
	if ((keyFile = fopen(argv[2], "rb")) == NULL) {
		fprintf(stderr, "\nCould not read key, file not found.\n");
		exit(1);
	}
	if ((input = fopen(argv[3], "rb")) == NULL) {
		fprintf(stderr, "\nCould not read input, file not found.\n");
		exit(1);
	}
	if ((output = fopen(argv[4], "wb")) == NULL) {
		fprintf(stderr, "\nCould not write output, file not found.\n");
		exit(1);
	}

        // create EVP context
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	if (encrypt) {
	    printf("Will encrypt using RSA\n");
            RSA *rsa_pkey = NULL;
            EVP_PKEY *pkey = EVP_PKEY_new();
	    unsigned char buffer[4096];
	    unsigned char buffer_out[4096 + EVP_MAX_IV_LENGTH];
	    size_t len;
	    int len_out = 0;
	    unsigned char *ek;
	    int eklen;
	    uint32_t eklen_n;
	    unsigned char iv[EVP_MAX_IV_LENGTH];
	    
            PEM_read_RSA_PUBKEY(keyFile, &rsa_pkey, NULL, NULL);
	    EVP_PKEY_assign_RSA(pkey, rsa_pkey);
	    ek = malloc(EVP_PKEY_size(pkey));
            EVP_SealInit(ctx, EVP_aes_128_cbc(), &ek, &eklen, iv, &pkey, 1);
	    eklen_n = htonl(eklen);
	    fwrite(&eklen_n, sizeof eklen_n, 1, output);
	    fwrite(ek, eklen, 1, output);
	    fwrite(iv, EVP_CIPHER_iv_length(EVP_aes_128_cbc()), 1, output);
	    
	    while ((len = fread(buffer, 1, sizeof buffer, input)) > 0)
	    {
		EVP_SealUpdate(ctx, buffer_out + len_out, &len_out, buffer, len);
		fwrite(buffer_out, len_out, 1, output);
	    }

	    EVP_SealFinal(ctx, buffer_out + len_out, &len_out);
	    fwrite(buffer_out, len_out, 1, output);

	    EVP_PKEY_free(pkey);
	    free(ek);
	} else {
            /* Implement RSA decryption. */
	    printf("Will decrypt using RSA\n");
	    RSA *rsa_pkey = NULL;
	    EVP_PKEY *pkey = EVP_PKEY_new();
	    unsigned char buffer[4096];
	    unsigned char buffer_out[4096 + EVP_MAX_IV_LENGTH];
	    size_t len;
	    int len_out = 0;
	    unsigned char *ek;
	    unsigned int eklen;
	    uint32_t eklen_n;
	    unsigned char iv[EVP_MAX_IV_LENGTH];

	    PEM_read_RSAPrivateKey(keyFile, &rsa_pkey, NULL, NULL);
	    EVP_PKEY_assign_RSA(pkey, rsa_pkey);

	    ek = malloc(EVP_PKEY_size(pkey));

            fread(&eklen_n, sizeof eklen_n, 1, input);
	    eklen = ntohl(eklen_n);
	    if (eklen > EVP_PKEY_size(pkey))
	    {
		fprintf(stderr, "Bad encrypted key length (%u > %d)\n", eklen,
                EVP_PKEY_size(pkey));
	    }
	    if (fread(ek, eklen, 1, input) != 1)
	    {
		perror("input file");
	    }
	    if (fread(iv, EVP_CIPHER_iv_length(EVP_aes_128_cbc()), 1, input) != 1)
	    {
		perror("input file");
	    }

	    EVP_OpenInit(ctx, EVP_aes_128_cbc(), ek, eklen, iv, pkey);

	    while ((len = fread(buffer, 1, sizeof buffer, input)) > 0)
	    {
		EVP_OpenUpdate(ctx, buffer_out + len_out, &len_out, buffer, len);
		fwrite(buffer_out, len_out, 1, output);
	    }

	    EVP_OpenFinal(ctx, buffer_out + len_out, &len_out);
	    fwrite(buffer_out, len_out, 1, output);

	    EVP_PKEY_free(pkey);
	    free(ek);
	}

	EVP_CIPHER_CTX_free(ctx);
	fclose(input);
	fclose(output);
}
