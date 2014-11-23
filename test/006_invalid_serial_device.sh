#! /bin/bash -xeu

pwd

! $TEST_BINARY --output 006_out --device file-probably-does-not-exist

! $TEST_BINARY --output 006_out --device ${srcdir}/image.bin
