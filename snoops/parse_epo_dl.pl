#! /usr/bin/perl
#
# extract the epo data from a to_watch.bin ($1)
# dump the epo data to $1.epo
# dump single entries to $1.trans_$i

use warnings;
use strict;

open(my $epo_out, ">", $ARGV[0].".epo");
binmode($epo_out);

open(my $epo, "<", $ARGV[0]);
binmode($epo);

seek($epo, 0x0e6000, 0);

my $data;

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

my $count = 0;
my $count_block = 0;
my $cs = 0;
while ( read($epo, $data, 8) ) {

  print "l: " . to_hex($data) . "\n";
  my $arr = to_arr($data);

  my $block_data = $data;

  if ($arr->[0] == 0x04 ) {

    for (my $i = 0; $i < 3; $i ++) {
      read ($epo, $data, 0x3c);
      print " d: " . to_hex($data) . "\n";
      my $arr = to_arr($data);
      if ($arr->[0] != 0x00) {
        $count ++;
        print $epo_out $data;
      }
      $block_data .= $data;
    }

    read ($epo, $data, 3);
    $block_data .= $data;

    if (0) {
      my $block_arr = to_arr($block_data);
      print "cs diff " . ($block_arr->[0xbc] - $cs) . "\n";
      $cs = $block_arr->[0xbc];
    }

    print " t: cs=" . sprintf("%x", $cs). " ". to_hex($data) . "\n";

    open (my $block_out, ">", $ARGV[0].".block_$count_block");
    print $block_out $block_data;
    $count_block ++;
  }

  else {

    read ($epo, $data, 300);
    print " t: cs=" . sprintf("%x", $cs||0). " ". to_hex($data) . "\n";
  }
}


print "count: $count\n";
