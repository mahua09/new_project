#include <libcouchbase/couchbase.h>
#include <stdlib.h>
#include <stdio.h>

static void error_callback(lcb_t instance,
		lcb_error_t err,
		const char *errinfo)
{
	fprintf(stderr, "Error %s: %s", lcb_strerror(instance, err),
			errinfo ? errinfo : "");
	exit(EXIT_FAILURE);
}

static void get_callback(lcb_t instance,
		const void *cookie,
		lcb_error_t error,
		const lcb_get_resp_t *resp)
{
	if (error != LCB_SUCCESS) {
		fprintf(stderr, "Failed to retrieve \"");
		fwrite(resp->v.v0.key, 1, resp->v.v0.nkey, stderr);
		fprintf(stderr, "\": %s\n", lcb_strerror(instance, error));
	} else {
		fprintf(stderr, "Data for key: \"");
		fwrite(resp->v.v0.key, 1, resp->v.v0.nkey, stderr);
		fprintf(stderr, "\" is : ");
		fwrite(resp->v.v0.bytes, 1, resp->v.v0.nbytes, stderr);
	}
}

int main(void)
{
	struct lcb_create_st create_options;
	lcb_t instance;
	lcb_error_t err;

	memset(&create_options, 0, sizeof(create_options));
	create_options.v.v0.host = "10.128.34.251:8091";
	create_options.v.v0.user = "Administrator";
	create_options.v.v0.passwd = "jiubugaosuni";
	create_options.v.v0.bucket = "beer-sample";

	err = lcb_create(&instance, &create_options);
	if (err != LCB_SUCCESS) {
		fprintf(stderr, "Failed to create libcouchbase instance: %s\n",
				lcb_strerror(NULL, err));
		return 1;
	}

	/* Set up the handler to catch all errors! */
	lcb_set_error_callback(instance, error_callback);

	/*
	 *      * Initiate the connect sequence in libcouchbase
	 *           */
	if ((err = lcb_connect(instance)) != LCB_SUCCESS) {
		fprintf(stderr, "Failed to initiate connect: %s\n",
				lcb_strerror(NULL, err));
		return 1;
	}

	/* Run the event loop and wait until we've connected */
	lcb_wait(instance);

	/*
	 *      * Set up a callback for our get requests
	 *           */
	lcb_set_get_callback(instance, get_callback);

	lcb_get_cmd_t cmd;
	const lcb_get_cmd_t * const commands[1] = { &cmd };
	memset(&cmd, 0, sizeof(cmd));
	cmd.v.v0.key = "foo";
	cmd.v.v0.nkey = 3;

	err = lcb_get(instance, NULL, 1, commands);
	if (err != LCB_SUCCESS) {
		fprintf(stderr, "Failed to get: %s\n",
				lcb_strerror(NULL, err));
		return 1;
	}

	lcb_wait(instance);

	lcb_destroy(instance);
	exit(EXIT_SUCCESS);
}
