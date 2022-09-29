#mk version of liboauth2
VERSION="0.3.0"

CFLAGS+= -DVERSION=$(VERSION)
LDFLAGS+= -lcurl

LIBPRE=lib
LIBEXT=a
ARFLAGS=cr

# Windows
# LIBPRE=
# LIBEXT=lib
# ARFLAGS=
