INF-744 - Assigment 1
Danilo Arthur Bertelli

What it is?
Implementation of RSA encrypt for image (.ppm) files.

How to use:
To compile the code, just run:
- gcc pk_encrypt_image.c ppm.c -o auth_encrypt_image -lcrypto

Generate your RSA keys (or use the one I attached):
- openssl genrsa -out private.pem 2048
- openssl rsa -in private.pem -outform PEM -pubout -out public.pem

To run, first encrypt the provided tux.ppm using (use the public key):
- ./auth_encrypt_image -e -RSA public.pem tux.ppm tux-e.ppm

Check the tux-e.ppm file, it's encrypted now.
A file called "temp" it's also created to store auth information.

To decrypt, simply run (now, use the private key):
- ./auth_encrypt_image -d -RSA private.pem tux-e.ppm tux-d.ppm

It will generate a tux-d.ppm file now decrypted.


_________________________ FILE IMPLEMENTATION _________________________

When I was playing with RSA implementation, I tried to encrypt/decrypt a
normal .txt file, for that, I've changed a little bit the implementation.

How to use:
To compile the code, just run:
- gcc rsa_encrypt_files.c -o rsa_encrypt_files -lcrypto

Generate your RSA keys (or just use the same above):
- openssl genrsa -out private.pem 2048
- openssl rsa -in private.pem -outform PEM -pubout -out public.pem

To run, first encrypt the provided test.txt using use the public key:
- ./rsa_encrypt_files -e public.pem test.txt test-e.txt

Check the test-e.txt file, it's encrypted now.
This time, we are storing the auth information in the same file, so there's
no temp file generated.

To decrypt, simply run (now, use the private key):
- ./rsa_encrypt_files -d private.pem test-e.txt test-d.txt

It will generate a test-d.txt file now decrypted.

