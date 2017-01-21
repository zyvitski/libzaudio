#! /bin/sh

if [[ "$OSTYPE" == "linux-gnu" ]]; then
        # ...
        libtoolize
elif [[ "$OSTYPE" == "darwin"* ]]; then
        # Mac OSX
        glibtoolize
else
        libtoolize
fi

aclocal \
&& automake --add-missing \
&& autoconf
