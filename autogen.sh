#!/bin/bash

if [[ "$OSTYPE" == "linux-gnu" ]]; then
  libtoolize
fi
if [[ "$OSTYPE" == "darwin"* ]]; then
  glibtoolize
fi

aclocal \
&& automake --add-missing \
&& autoconf
