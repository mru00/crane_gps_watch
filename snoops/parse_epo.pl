#! /usr/bin/perl
#

use warnings;
use strict;
use POSIX;
use Data::Dumper;

sub to_hex {
  my ($data) = @_;
  my $hex = unpack('H*', $data);
  return join(" ", $hex =~ m/../g);
}

sub to_arr {
  my ($data) = @_;
  my $hex = unpack('H*', $data);
  my @fields = ( $hex =~ m/../g );

  return [ map { hex } @fields ];
}

my $filename = $ARGV[0];


my $is_mtk = $filename =~ /MTK/;
my $entry_size = $is_mtk ? 0x3c : 0x48;

my %leaders;

open(my $epo, "<", $ARGV[0]);
binmode($epo);

my $data;

my $count  = 0;
while ( read($epo, $data, $entry_size) ) {
  $count ++;
  $data = substr($data, 0, 0x3c);
  my $arr = to_arr($data);
  my $year = $arr->[2];
  my $doy = $arr->[0] + ( $arr->[1] << 8 ) + ($arr->[2] << 16);

  my $hex = to_hex($data);
  print "$hex\n" if $count < 11;

  $leaders{$doy & 0xffffff} ++;
}


print "count: $count\n";


my @doys = sort keys %leaders;

print $doys[scalar @doys - 1] - $doys[0] . "\n";

sub to_date {
  my ($rec_num) = @_;
  return POSIX::mktime(0, 0, $rec_num, 6, 0, 80, 0, 0, 0);
}

foreach my $doy (@doys) {
  printf "record number: %d, %s\n", $doy, scalar localtime (to_date($doy)) ;
}

printf "hours since 6 1 1980 %d\n", (time - POSIX::mktime(0, 0, 0, 6, 0, 80))/60/60;
printf "hours since 22 8 1999 %d\n", (time - POSIX::mktime(0, 0, 0, 22, 7, 99))/60/60;
printf "hours since 2 12 2009 %d\n", (time - POSIX::mktime(0, 0, 0, 2, 11, 109))/60/60;
printf "hours since 1 1 2010 %d\n", (time - POSIX::mktime(0, 0, 0, 1, 0, 110))/60/60;
printf "hours since 1 1 2014 %d\n", (time - POSIX::mktime(0, 0, 0, 1, 0, 114))/60/60;

#print Dumper(\%leaders);
