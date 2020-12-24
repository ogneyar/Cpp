#include "base64.h"


int main(int argc, char **argv)
{
	const char *data = "ABC123Test Lets Try this' input and see What \"happens\"";
	char       *enc;
	char       *out;
	size_t      out_len;

	printf("data:    '%s'\n", data);

	enc = b64_encode((const unsigned char *)data, strlen(data));
	printf("encoded: '%s'\n", enc);

	printf("dec size %s data size\n", b64_decoded_size(enc) == strlen(data) ? "==" : "!=");

	/* +1 for the NULL terminator. */
	out_len = b64_decoded_size(enc);
	out = (char *) malloc(out_len);

	if (!b64_decode(enc, (unsigned char *)out, out_len)) {
		printf("Decode Failure\n");
		return 1;
	}
	out[out_len] = '\0';

	printf("dec:     '%s'\n", out);
	printf("data %s dec\n", strcmp(data, out) == 0 ? "==" : "!=");
	free(out);

	system("pause");

	return 0;
}