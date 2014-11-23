#! /bin/bash -xeu

pwd

! $TEST_BINARY --output 005_out --from_image file-probably-does-not-exist

