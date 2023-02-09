#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/evp.h>

// GLOBAL PARAMETERS
#define TOTAL_SENDERS 1000
#define TOTAL_MESSAGES 500000
#define PERTINENT_MESSAGES 100
#define BENCH_ITERATIONS 150

#define BENCH_ST(id) clock_t ___bench_clock_##id = clock()
#define BENCH_END_SILENT(id) double ___bench_result_##id = (double)(clock() - ___bench_clock_##id) * 1000.0f / CLOCKS_PER_SEC
#define BENCH_END(id,name) BENCH_END_SILENT(id); printf("%s done in %f ms\n", name, ___bench_result_##id)
#define BENCH_RESULT(id) ___bench_result_##id

EC_KEY* create_key(int nid) {
	EC_KEY *key;
	if (NULL == (key = EC_KEY_new_by_curve_name(nid))) {
		printf("Failed to create key curve\n");
		return NULL;
	}

	if (1 != EC_KEY_generate_key(key)) {
		printf("Failed to generate key\n");
		return NULL;
	}

	return key;
}

unsigned char* get_secret(EC_KEY* key, const EC_POINT* peer_pub_key, size_t* secret_len) {
	int field_size;
	unsigned char* secret;

	field_size = EC_GROUP_get_degree(EC_KEY_get0_group(key));
	*secret_len = (field_size + 7) / 8;

	if (NULL == (secret = OPENSSL_malloc(*secret_len))) {
		printf("Failed to allocate memory for secret");
		return NULL;
	}

	*secret_len = ECDH_compute_key(secret, *secret_len,	peer_pub_key, key, NULL);

	if (*secret_len <= 0) {
		OPENSSL_free(secret);
		return NULL;
	}
	return secret;
}

typedef struct {
	int sender_id;
	int recipient_id;
	unsigned char* message; // 16 bytes
} s_msg;

typedef struct {
	EC_KEY* recipient_priv;
	EC_POINT* recipient_pub;

	EC_KEY** senders_priv;
	EC_POINT** senders_pub;
	int senders_count;

	s_msg* messages;
	int messages_count;
	int* pertinent_msgs_indexes;
	int pertinent_msgs_count;
} s_env;

s_env setup_keys(int nid, int amount) {
	EC_KEY** senders_priv = malloc(sizeof(EC_KEY*)*amount);
	EC_POINT** senders_pub = malloc(sizeof(EC_POINT*)*amount);
	for(int i = 0; i < amount; i++) {
		senders_priv[i] = create_key(nid);
		senders_pub[i] = EC_KEY_get0_public_key(senders_priv[i]);
	}
	EC_KEY* recipient_priv = create_key(nid);
	EC_POINT* recipient_pub = EC_KEY_get0_public_key(recipient_priv);

	s_env env = {
		recipient_priv,
		recipient_pub,
		senders_priv,
		senders_pub,
		amount,
		NULL,
		0,
		NULL,
		0
	};

	return env;
}

void free_msg(s_msg* msg) {
	free(msg->message);
}

void free_env(s_env* env) {
	EC_KEY_free(env->recipient_priv);
	for(int i = 0; i < env->senders_count; i++) {
		EC_KEY_free(env->senders_priv[i]);
	}
	free(env->senders_priv);
	free(env->senders_pub);
	env->senders_count = 0;
	for(int i = 0; i < env->messages_count; i++) {
		free_msg(& env->messages[i]);
	}
	free(env->messages);
	env->messages_count = 0;
	free(env->pertinent_msgs_indexes);
	env->pertinent_msgs_count = 0;
}

void print_hex(char* preamble, unsigned char* data, int len) {
	printf("%s ", preamble);
	for (int i = 0; i < len; i++)
	{
		printf("%02x", data[i]);
	}
	printf("\n");
}

int compare(const void *a, const void *b) { // comparator for qsort
	const int *aa = a;
	const int *bb = b;
	return (*aa > *bb) - (*aa < *bb);
}

int get_public_ley_length(EC_KEY* privkey, EC_POINT* pubkey) {
	int len = EC_POINT_point2oct(EC_KEY_get0_group(privkey), pubkey, POINT_CONVERSION_COMPRESSED, NULL, 0, NULL);
	return len;
}

// message has to be 16 bytes long
unsigned char* aes_encrypt(unsigned char* message, unsigned char* password, int password_len, int* ciphertext_total_len) {
	// derive key and IV from password for AES-128-CBC
	unsigned char key[16];
	unsigned char iv[16];
	EVP_BytesToKey(EVP_aes_128_cbc(), EVP_sha256(), NULL, password, password_len, 1, key, iv);

	// encrypt message (16 bytes) - no salt, no padding
	unsigned char* ciphertext = malloc(16);
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
	EVP_CIPHER_CTX_set_padding(ctx, 0);

	int ciphertext_len = 0;
	*ciphertext_total_len = 0;
	EVP_EncryptUpdate(ctx, ciphertext, &ciphertext_len, message, 16);
	*ciphertext_total_len += ciphertext_len;
	EVP_EncryptFinal_ex(ctx, ciphertext + ciphertext_len, &ciphertext_len);
	*ciphertext_total_len += ciphertext_len;

	assert(*ciphertext_total_len == 16);

	EVP_CIPHER_CTX_free(ctx);

	return ciphertext;
}

unsigned char* aes_decrypt(unsigned char* ciphertext, unsigned char* password, int password_len, int* message_total_len) {
	// derive key and IV from password for AES-128-CBC
	unsigned char key[16];
	unsigned char iv[16];
	EVP_BytesToKey(EVP_aes_128_cbc(), EVP_sha256(), NULL, password, password_len, 1, key, iv);

	// decrypt message (16 bytes) - no salt, no padding
	unsigned char* message = malloc(16);
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
	EVP_CIPHER_CTX_set_padding(ctx, 0);

	int message_len;
	*message_total_len = 0;
	EVP_DecryptUpdate(ctx, message, &message_len, ciphertext, 16);
	*message_total_len += message_len;
	EVP_DecryptFinal_ex(ctx, message + message_len, &message_len);
	*message_total_len += message_len;

	assert(*message_total_len == 16);

	EVP_CIPHER_CTX_free(ctx);

	return message;
}

void generate_pertinent_indexes(int total, int pertinent, int* indexes) {
	assert(pertinent <= total);

	// initial fill
	for(int i = 0; i < pertinent; i++) {
		indexes[i] = rand() % total;
	}

	// sort indexes
	const int max_attempts = 1000;
	int attempt = 0;
	while(attempt++ < max_attempts) {
		qsort(indexes, pertinent, sizeof(int), compare);

		// find first duplicate
		int first_duplicate = -1;
		for(int i = 1; i < pertinent; i++) {
			if(indexes[i] == indexes[i - 1]) {
				first_duplicate = i;
				break;
			}
		}
		if (first_duplicate == -1) {
			return;
		}

		// we have duplicates, so we need to generate new index
		indexes[first_duplicate] = rand() % total;
	}
	printf("Failed to generate pertinent indexes after %d attempts\n", max_attempts);
	exit(1);
}

void create_messages(s_env* env) {
	// generate random indexes for pertinent messages
	int* indexes = malloc(sizeof(int) * env->pertinent_msgs_count);
	generate_pertinent_indexes(env->messages_count, env->pertinent_msgs_count, indexes);
	env->pertinent_msgs_indexes = indexes;

	s_msg* messages = malloc(sizeof(s_msg) * env->messages_count);

	// create messages
	for(int i = 0; i < env->messages_count; i++) {
		messages[i].sender_id = rand() % env->senders_count;
		messages[i].recipient_id = rand() % env->senders_count;
		size_t secret_len;
		unsigned char* secret = get_secret(env->senders_priv[messages[i].sender_id], env->senders_pub[messages[i].recipient_id], &secret_len);
		unsigned char message[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
		int cipher_len;
		messages[i].message = aes_encrypt(message, secret, secret_len, &cipher_len);
		assert(cipher_len == 16);
		OPENSSL_free(secret);
	}

	// create pertinent messages (TODO: this (c|sh)ould be done in the previous loop)
	for(int i = 0; i < env->pertinent_msgs_count; i++) {
		int j = indexes[i];
		messages[j].sender_id = rand() % env->senders_count;
		messages[j].recipient_id = -1;
		size_t secret_len;
		unsigned char* secret = get_secret(env->senders_priv[messages[j].sender_id], env->recipient_pub, &secret_len);
		unsigned char message[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
		free(messages[j].message); // free placeholder message
		int cipher_len;
		messages[j].message = aes_encrypt(message, secret, secret_len, &cipher_len);
		assert(cipher_len == 16);
		OPENSSL_free(secret);
	}

	env->messages = messages;
}

int detect(s_env* env) {
	int error_counter = 0;
	int cur_pert_index = 0;
	for(int i = 0; i < env->messages_count; i++) {
		size_t secret_len;
		unsigned char* secret = get_secret(env->recipient_priv, env->senders_pub[env->messages[i].sender_id], &secret_len);
		int message_len;
		unsigned char* message = aes_decrypt(env->messages[i].message, secret, secret_len, &message_len);
		OPENSSL_free(secret);

		if (i == env->pertinent_msgs_indexes[cur_pert_index]) {
			// this is a pertinent message
			assert(message_len == 16);
			for(int k = 0; k < message_len; k++)
				assert(message[k] == 1);
			cur_pert_index++;
		} else {
			unsigned char accum = 1;
			for(int k = 0; k < message_len; k++)
				accum &= message[k];
			if(accum != 0)
				error_counter++;
		}

		free(message);
	}

	return error_counter;
}

typedef struct {
	double keygen;
	double msggen;
	double detect;
	double fpcount;
} s_bench_res;

s_bench_res mean(s_bench_res* res, int count) {
	s_bench_res sum = {0,0,0,0};
	for(int i = 0; i < count; i++) {
		sum.keygen += res[i].keygen;
		sum.msggen += res[i].msggen;
		sum.detect += res[i].detect;
		sum.fpcount += res[i].fpcount;
	}
	sum.keygen /= count;
	sum.msggen /= count;
	sum.detect /= count;
	sum.fpcount /= count;
	return sum;
}

s_bench_res stdev(s_bench_res* res, int count) {
	s_bench_res m = mean(res, count);
	s_bench_res sum = {0,0,0,0};
	for(int i = 0; i < count; i++) {
		sum.keygen += (res[i].keygen - m.keygen) * (res[i].keygen - m.keygen);
		sum.msggen += (res[i].msggen - m.msggen) * (res[i].msggen - m.msggen);
		sum.detect += (res[i].detect - m.detect) * (res[i].detect - m.detect);
		sum.fpcount += (res[i].fpcount - m.fpcount) * (res[i].fpcount - m.fpcount);
	}
	sum.keygen /= count;
	sum.msggen /= count;
	sum.detect /= count;
	sum.fpcount /= count;
	sum.keygen = sqrt(sum.keygen);
	sum.msggen = sqrt(sum.msggen);
	sum.detect = sqrt(sum.detect);
	sum.fpcount = sqrt(sum.fpcount);
	return sum;
}

#define BENCH(nid) bench(nid,#nid)
void bench(int nid, char* curve_name) {
	printf("BENCHMARKING   %d agents; %d msgs; %d pertinent   using   %s curve   %d iterations\n",
	       TOTAL_SENDERS, TOTAL_MESSAGES, PERTINENT_MESSAGES, curve_name, BENCH_ITERATIONS);

	s_env env_temp = setup_keys(nid, 1); // key length
	printf("Public key length: %d bytes\n", get_public_ley_length(env_temp.recipient_priv, env_temp.recipient_pub));
	free_env(&env_temp);

	s_bench_res res[BENCH_ITERATIONS];
	for(int i = 0; i < BENCH_ITERATIONS; i++) {
		BENCH_ST(1);

			BENCH_ST(2);
			s_env env = setup_keys(nid, TOTAL_SENDERS);
			BENCH_END_SILENT(2);

			env.messages_count = TOTAL_MESSAGES;
			env.pertinent_msgs_count = PERTINENT_MESSAGES;
			BENCH_ST(3);
			create_messages(&env);
			BENCH_END_SILENT(3);

			BENCH_ST(4);
			int error = detect(&env);
			BENCH_END_SILENT(4);

			s_bench_res iter_res = {BENCH_RESULT(2),BENCH_RESULT(3),BENCH_RESULT(4),(double)error};
            res[i] = iter_res;

		BENCH_END_SILENT(1);

		free_env(&env);
	}

	s_bench_res avg = mean(res, BENCH_ITERATIONS);
	s_bench_res std = stdev(res, BENCH_ITERATIONS);
	printf("Average keygen: %f ms ± %f ms\n", avg.keygen, std.keygen);
	printf("Average msggen: %f ms ± %f ms\n", avg.msggen, std.msggen);
	printf("Average detect: %f ms ± %f ms\n", avg.detect, std.detect);
	printf("Average fpcount: %f ± %f\n", avg.fpcount, std.fpcount);
	printf("\n");
}

int main(int argc, char *argv[]) {
	// BENCH(NID_secp112r1);
	// BENCH(NID_secp112r2);
	// BENCH(NID_secp128r1);
	// BENCH(NID_secp128r2);
	// BENCH(NID_secp160k1);
	// BENCH(NID_secp160r1);
	// BENCH(NID_secp160r2);
	// BENCH(NID_secp192k1);
	// BENCH(NID_secp224k1);
	BENCH(NID_secp224r1);
	// BENCH(NID_secp256k1);
	// BENCH(NID_secp384r1);
	// BENCH(NID_secp521r1);

	BENCH(NID_sect113r1);
	BENCH(NID_sect113r2);
	// BENCH(NID_sect131r1);
	// BENCH(NID_sect131r2);
	// BENCH(NID_sect163k1);
	// BENCH(NID_sect163r1);
	// BENCH(NID_sect163r2);
	// BENCH(NID_sect193r1);
	// BENCH(NID_sect193r2);
	// BENCH(NID_sect233k1);
	// BENCH(NID_sect233r1);
	// BENCH(NID_sect239k1);
	// BENCH(NID_sect283k1);
	// BENCH(NID_sect283r1);
	// BENCH(NID_sect409k1);
	// BENCH(NID_sect409r1);
	// BENCH(NID_sect571k1);
	// BENCH(NID_sect571r1);

	// BENCH(NID_X9_62_prime192v1);
	// BENCH(NID_X9_62_prime192v3);
	// BENCH(NID_X9_62_prime192v2);
	// BENCH(NID_X9_62_prime239v2);
	// BENCH(NID_X9_62_prime239v1);
	BENCH(NID_X9_62_prime256v1);
	// BENCH(NID_X9_62_prime239v3);
}
