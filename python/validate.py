#!/bin/env python

from __future__ import print_function

import sys
import time

import pykcoidc


def main(args):
    iss_s = len(args) > 0 and args[0] or ""
    token_s = len(args) > 1 and args[1] or ""

    # Allow insecure operations.
    try:
        pykcoidc.insecure_skip_verify(1)
    except pykcoidc.Error as e:
        print("> Error: insecure_skip_verify failed: 0x%x" % e.args[0])
        return -1
    # Initialize with issuer identifier.
    try:
        pykcoidc.initialize(iss_s)
    except pykcoidc.Error as e:
        print("> Error: initialize failed: 0x%x" % e.args[0])
        return -1
    # Wait until oidc validation becomes ready.
    try:
        pykcoidc.wait_until_ready(10)
    except pykcoidc.Error as e:
        print("> Error: failed to get ready in time: 0x%x" % e.args[0])
        return -1

    token_result = None
    err = None
    begin = time.time()
    # Validate token passed from commandline.
    try:
        token_result = validate(token_s)
    except pykcoidc.Error as e:
        err = e
    end = time.time()
    time_spent = end - begin

    res = err and err.args[0] or 0
    print("> Result code   : 0x%x" % res)

    print("> Validation    : %s" % (err is None and "valid" or "invalid"))
    print("> Auth ID       : %s" % (token_result and token_result[0]))
    print("> Time spent    : %fs" % time_spent)

    print("> Standard      : %s" % (token_result and token_result[2]))
    print("> Extra         : %s" % (token_result and token_result[3]))
    print("> Token type    : %d" % (token_result and token_result[1] or 0))

    if res == 0:
        try:
            userinfo = fetch_userinfo(token_s)
        except pykcoidc.Error as e:
            print("> Userinfo      : 0x%x" % e.args[0])
        else:
            print("> Userinfo      : 0x0")
            print(userinfo)

    try:
        pykcoidc.uninitialize()
    except pykcoidc.Error as e:
        print("> Error: failed to uninitialize 0x%x" % e.args[0])

    return res != 0 and -1 or 0


def validate(token_s):
    return pykcoidc.validate_token_s(token_s)


def fetch_userinfo(token_s):
    return pykcoidc.fetch_userinfo_with_accesstoken_s(token_s)


if __name__ == "__main__":
    status = main(sys.argv[1:])
    sys.exit(status)
