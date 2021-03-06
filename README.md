# Kopano OpenID Connection validation library

This project implements a C shared library with a public API to validate Kopano
Konnect tokens (JSON Web Tokens).

In the `python` subdirectory a CPython module is available which makes all the
functions from the C shared library usable from Python.

Also this project can be used directly from Go as an importable module.

## Compiling

Make sure you have Go 1.13 or later installed. This project uses Go modules.

As this is a C library, it is furthermore assumed that there is a working C
compiler toolchain in your path which includes autoconf and make.

```
git clone <THIS-PROJECT> libkcoidc
cd libkcoidc
./bootstrap.sh
./configure
make
```

This will produce the compiled library `.so` and the matching C header file in
the `./.libs` directory.

## Environment variables

| Environment variable       | Description                                   |
|----------------------------|-----------------------------------------------|
| KCOIDC_DEBUG               | When set, `libkcoidc` will print debug info to stdout  |

### Compile python module

The Python module supports Python 3 and Python 2. By default it compiles with
whatever Python in your path as `python3`, `python` or `python2` whichever is
found first. Use the `PYTHON` environment variable to compile for a specific
Python.

```
PYTHON=python3 make python
```

### Use a Go module

```
import "stash.kopano.io/kc/libkcoidc"

provider, err := kcoidc.NewProvider(nil, nil, false)
```

## Errors

The library returns error codes in the form of integer values. Please see
`errors.go` for a list.

## Examples

This project contains C example applications in the `examples` folder which can
be used to test the library form the commandline.

```
export TOKEN_VALUE='eyJhbGciOiJSUzI1NiIsImtpZCI6ImRlZmF1bHQiLCJ0eXAiOiJKV1QifQ.eyJrYy5pc0FjY2Vzc1Rva2VuIjp0cnVlLCJrYy5hdXRob3JpemVkU2NvcGVzIjpbIm9wZW5pZCIsInByb2ZpbGUiLCJlbWFpbCJdLCJhdWQiOiJwbGF5Z3JvdW5kLXRydXN0ZWQuanMiLCJleHAiOjE1MTYyOTEzMTEsImlhdCI6MTUxNjI5MDcxMSwiaXNzIjoiaHR0cHM6Ly9tb3NlNDo4NDQzIiwic3ViIjoidWlkPXVzZXIxLG91PXVzZXJzLGRjPWZhcm1lcixkYz1sYW4iLCJrYy5pZGVudGl0eSI6eyJrYy5pLmRuIjoiSm9uYXMgQnJla2tlIiwia2MuaS51biI6InVzZXIxIn19.A28u8R_Euv492qVsIEub5836qo3wzinM8up78vFVEZ1o48PA7-7LrNqJ14EfC_Me-vd2QrW6GtofScSreLUrnqTACYnG6G7R3RVJhCjiuMd6eOFnLAjLl-2ubGa8DYHTK4k9p_Ynuv06AEvCqlplqtK5Mlg0OIbLTxfKxyg77quH6OA0MUbvndKG5t1S9ADj3v39OlSzdpnvSV8LKs7soCtXfotR6Bg8xSXdBI-tNhrjSbzCI2BaghVSdaRbQkcTBe3W5KimaBjbBpTIH74ViFJYzIGOMmGKr__CH4KYn_F-r5ULyVE7m4Qn4K6wqt17TXR3xG6T7Hhs19xVvzoGKg'
make examples && KCOIDC_DEBUG= bin/validate-c 'https://mose4:8443' "$TOKEN_VALUE" && echo 'yay' || echo 'nay'
```

First parameter of the validate example binary is the full qualified OpenID
Connect Issuer Identifier which is usually a `https://` URL without path. The
second parameter is a string value which will be used as token. So any encoded
JWT can be passed here.

Similarly there is also a simple C++ benchmark.

```
make examples && KCOIDC_DEBUG= bin/benchmark-cpp 'https://mose4:8443' "$TOKEN_VALUE" && echo 'yay' || echo 'nay'
> Info : using 8 threads with 100000 runs per thread
> Info : thread 2 started ...
> Info : thread 3 started ...
> Info : thread 5 started ...
> Info : thread 1 started ...
> Info : thread 4 started ...
> Info : thread 6 started ...
> Info : thread 7 started ...
> Info : thread 8 started ...
> Info : thread 1 done:100000 failed:0
> Info : thread 2 done:100000 failed:0
> Info : thread 4 done:100000 failed:0
> Info : thread 3 done:100000 failed:0
> Info : thread 7 done:100000 failed:0
> Info : thread 6 done:100000 failed:0
> Info : thread 8 done:100000 failed:0
> Info : thread 5 done:100000 failed:0
> Time : 19.101s
> Rate : 41882.6 op/s
yay
```

So on my machine (Intel(R) Core(TM) i7-3930K CPU @ 3.20GHz), this gives around
42000 validations per second with 8 parallel threads.

The same applications are also implemented in Python and Go for your reference/example.
