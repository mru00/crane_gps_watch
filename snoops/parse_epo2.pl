#! /usr/bin/perl
#
# parse EPO.DAT format - different from MTK7d.EPO

use warnings;
use strict;

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


open(my $epo, "<", $ARGV[0]);
binmode($epo);

my $data;

my $count  = 0;
while ( read($epo, $data, 0x48) ) {
  $count ++;
  my $arr = to_arr($data);
  my $year = $arr->[2];
  my $doy = $arr->[0] + ( $arr->[1] << 8 );
  my $hex = to_hex($data);

  print "$year/$doy $hex\n";
}


print "count: $count\n";

