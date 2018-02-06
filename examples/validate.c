/*
 * Copyright 2018 Kopano and its licensors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "kcoidc.h"

int main(int argc, char** argv)
{
	int res;
	int res2;
	struct timeval begin, end, time_spent;

	char* iss_s = argv[1];
	char* token_s = argv[2];
	struct kcoidc_validate_token_s_return token_result;
	struct kcoidc_fetch_userinfo_with_accesstoken_s_return userinfo_result;

	// Allow insecure operations.
	res = kcoidc_insecure_skip_verify(1);
	if (res != 0) {
		printf("> Error: insecure_skip_verify failed: 0x%x\n", res);
		goto exit;
	}
	// Initialize with issuer identifier.
	res = kcoidc_initialize(iss_s);
	if (res != 0) {
		printf("> Error: initialize failed: 0x%x\n", res);
		goto exit;
	}
	// Wait until oidc validation becomes ready.
	res = kcoidc_wait_until_ready(10);
	if (res != 0) {
		printf("> Error: failed to get ready in time: 0x%x\n", res);
		goto exit;
	}

	gettimeofday(&begin, NULL);
	// Validate token passed from commandline.
	token_result = kcoidc_validate_token_s(token_s);
	gettimeofday(&end, NULL);
	timersub(&end, &begin, &time_spent);

	// Handle validation result.
	res = token_result.r1;
	printf("> Result code   : 0x%x\n", res);

	// Show the result.
	printf("> Token subject : %s -> %s\n", token_result.r0, token_result.r1 == 0 ? "valid" : "invalid");
	printf("> Time spent    : %ld.%06lds\n", (long int)time_spent.tv_sec, (long int)time_spent.tv_usec);

	// Free the returned subject memory.
	free(token_result.r0);

	printf("> Standard      : %s\n", token_result.r3);
	printf("> Extra         : %s\n", token_result.r4);
	printf("> Token type    : %d\n", token_result.r2);

	if (res == 0 && token_result.r2 == KCOIDC_TOKEN_TYPE_KCACCESS) {
		userinfo_result = kcoidc_fetch_userinfo_with_accesstoken_s(token_s);
		printf("> Userinfo      : 0x%llu\n", userinfo_result.r1);
		if (userinfo_result.r1 == 0) {
			printf("%s\n", userinfo_result.r0);

			// Free userinfo data.
			free(userinfo_result.r0);
		}
	}

	// Free the rest.
	free(token_result.r3);
	free(token_result.r4);

	// Remember to uninitialize on success as well.
	res2 = kcoidc_uninitialize();
	if (res2 != 0) {
		printf("> Error: failed to uninitialize: 0x%x\n", res2);
	}

exit:
	if (res != 0) {
		return -1;
	}
	return 0;
}
