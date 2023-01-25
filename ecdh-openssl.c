#include <assert.h>
#include <stdio.h>
#include <time.h>

#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/evp.h>

#define BENCH_ST(id) clock_t ___bench_clock_##id = clock()
#define BENCH_END(id,name) printf("%s done in %f ms\n", name, (double)(clock() - ___bench_clock_##id) * 1000.0f / CLOCKS_PER_SEC)

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

struct s_msg {
	int sender_id;
	unsigned char* message;
};

struct s_env {
	EC_KEY* recipient_priv;
	EC_POINT* recipient_pub;

	EC_KEY** senders_priv;
	EC_POINT** senders_pub;
	int senders_count;

	struct s_msg** messages;
	int messages_count;
};

struct s_env setup_keys(int nid, int amount) {
	EC_KEY** senders_priv = malloc(sizeof(EC_KEY*)*amount);
	EC_POINT** senders_pub = malloc(sizeof(EC_POINT*)*amount);
	for(int i = 0; i < amount; i++) {
		senders_priv[i] = create_key(nid);
		senders_pub[i] = EC_KEY_get0_public_key(senders_priv[i]);
	}
	EC_KEY* recipient_priv = create_key(nid);
	EC_POINT* recipient_pub = EC_KEY_get0_public_key(recipient_priv);

	struct s_env env = {
		recipient_priv,
		recipient_pub,
		senders_priv,
		senders_pub,
		amount,
		NULL,
		0
	};

	return env;
}

void free_env(struct s_env env) {
	EC_KEY_free(env.recipient_priv);
	for(int i = 0; i < env.senders_count; i++) {
		EC_KEY_free(env.senders_priv[i]);
	}
	free(env.senders_priv);
	free(env.senders_pub);
}

void create_messages(int amount, int pertinent_amount) {
	// generate random indexes for pertinent messages
	int* indexes = malloc(sizeof(int)*pertinent_amount);
	for(int i = 0; i < pertinent_amount; i++) {
		indexes[i] = rand() % amount;
	}
}

int create_shared_secret(int nid) {
	EC_KEY* alice = create_key(nid);
	EC_KEY* bob = create_key(nid);
	assert(alice != NULL && bob != NULL);

	const EC_POINT *alice_public = EC_KEY_get0_public_key(alice);
	const EC_POINT *bob_public = EC_KEY_get0_public_key(bob);

	size_t alice_secret_len;
	size_t bob_secret_len;

	unsigned char *alice_secret = get_secret(alice, bob_public, &alice_secret_len);
	unsigned char *bob_secret = get_secret(bob, alice_public, &bob_secret_len);

	printf("Secret length: %d\n", alice_secret_len);

	EC_KEY_free(alice);
	EC_KEY_free(bob);
	OPENSSL_free(alice_secret);
	OPENSSL_free(bob_secret);

	return 0;
}

#define BENCH(nid) bench(nid,#nid)
void bench(int nid, char* curve_name) {
	const int AGENTS = 1000;

	printf("Generating %d keys using ", AGENTS+1);
	setup_keys(nid, 10); // warmup
	BENCH_ST(1);
	struct s_env env = setup_keys(nid, AGENTS);
	BENCH_END(1, curve_name);


}

int main(int argc, char *argv[]) {
	BENCH(NID_secp112r1);
	BENCH(NID_secp112r2);
	BENCH(NID_secp128r1);
	BENCH(NID_secp128r2);
	BENCH(NID_secp160k1);
	BENCH(NID_secp160r1);
	BENCH(NID_secp160r2);
	BENCH(NID_secp192k1);
	BENCH(NID_secp224k1);
	BENCH(NID_secp224r1);
	BENCH(NID_secp256k1);
	BENCH(NID_secp384r1);
	BENCH(NID_secp521r1);

	BENCH(NID_sect113r1);
	BENCH(NID_sect113r2);
	BENCH(NID_sect131r1);
	BENCH(NID_sect131r2);
	BENCH(NID_sect163k1);
	BENCH(NID_sect163r1);
	BENCH(NID_sect163r2);
	BENCH(NID_sect193r1);
	BENCH(NID_sect193r2);
	BENCH(NID_sect233k1);
	BENCH(NID_sect233r1);
	BENCH(NID_sect239k1);
	BENCH(NID_sect283k1);
	BENCH(NID_sect283r1);
	BENCH(NID_sect409k1);
	BENCH(NID_sect409r1);
	BENCH(NID_sect571k1);
	BENCH(NID_sect571r1);

	BENCH(NID_X9_62_prime192v1);
	BENCH(NID_X9_62_prime192v3);
	BENCH(NID_X9_62_prime192v2);
	BENCH(NID_X9_62_prime239v2);
	BENCH(NID_X9_62_prime239v1);
	BENCH(NID_X9_62_prime256v1);
	BENCH(NID_X9_62_prime239v3);
}
