#! /usr/bin/perl 
# Copyright (C) 2014 mru@sisyphus.teil.cc

# format EPO file ready for download to watch.
# works only for MTK7d.EPO files.

use strict;
use warnings;


open(my $epo, "<", $ARGV[0]) or die $!;
binmode($epo);

open(my $out, ">", $ARGV[0].".to_watch") or die $!;
binmode($out);

seek($out, 0x0e6000, 0);

my @entries;
my $data;
while ( read($epo, $data, 0x3c) ) {
  push @entries, $data;
}

my $null_entry = pack('H*', '00' x 0x3c);

# some fill entries
push @entries, $null_entry;
push @entries, $null_entry;
push @entries, $null_entry;
push @entries, $null_entry;

print @entries . "\n";
my $i = 0;
while (@entries) {

  my $idx = sprintf('%02x%02x', $i & 0xff, ($i >> 8) &0xff);

  # last block is marked with idx 'ffff'
  if (scalar @entries == 3) {
    $idx = "ffff";
  }

  my $leader = pack('H*', '0424bf00d202'.$idx);

  # add three EPO entries
  my $data = $leader;
  $data .= shift @entries;
  $data .= shift @entries;
  $data .= shift @entries;

  # calculate checksum
  my $cs = 0;
  my $data_as_hex = substr(unpack ('H*', $data), 4);
  foreach my $v ($data_as_hex =~ m/../g) {
    $cs = ($cs ^ hex $v);
  }

  my $trailer = pack('H*', sprintf("%02x0d0a", $cs));
  print $out $data . $trailer;
  $i ++ ;
}


# some more data... 
print $out pack('H*', '2C010000000000000D000000F300000000000000B0EE2E02DCAE570000000000010000000000000000000000');



