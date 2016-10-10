// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

// This file prevents the following error when compiling GIFLIB on Windows.
// ```
// dgif_lib.c(15): fatal error C1083: Cannot open include file: 'unistd.h': No such file or directory
// egif_lib.c(11): fatal error C1083: Cannot open include file: 'unistd.h': No such file or directory
// gif_hash.h(10): fatal error C1083: Cannot open include file: 'unistd.h': No such file or directory
// gif_hash.c(14): fatal error C1083: Cannot open include file: 'unistd.h': No such file or directory
// gif_hash.h(10): fatal error C1083: Cannot open include file: 'unistd.h': No such file or directory
// ```
