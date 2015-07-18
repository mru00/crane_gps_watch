# Copyright (C) 2014 mru@sisyphus.teil.cc


TESTS = 000_help.sh \
				001_from_image.sh \
				002_from_to_image.sh \
				003_from_image_split.sh \
				004_exclusive_args.sh \
				005_image_not_found.sh \
				006_invalid_serial_device.sh \
				007_output_not_writable.sh \
				008_lint_images.sh \
				009_expect_coordinates.sh \
				010_knedlyk_running_cycling.sh 011_assert_profile.sh 012_dst_time.sh

# not passing:
# 010_knedlyk_running_cycling.sh 
#
#

# variables to be used in test scripts:
# required to be able to check mingw cross compiles
AM_TESTS_ENVIRONMENT = \
	export TEST_BINARY=../src/crane_gps_watch_client$(EXEEXT); 

if CROSS_COMPILING
AM_TESTS_ENVIRONMENT += \
	regedit $(srcdir)/quiet.reg; 
endif


EXTRA_DIST = TrainingCenterDatabasev2.xsd $(TESTS) common.sh $(wildcard lint_images/*.bin) $(wildcard *.bin)

MOSTLYCLEANFILES = *.tcx 002_image2.bin *_wd/*
