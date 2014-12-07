#! /usr/bin/perl
#
# Copyright (C) 2014 mru@sisyphus.teil.cc
#

use strict;
use warnings;

use List::Util qw(sum);
use Data::Dumper;


sub parse_line {
  my ($line) = @_;

  $_ = $line;
  if (/^.?$/) {
    return {
      type => "noop"
    };
  }
  if (/Port ge.*ffnet/) {
    return {
      port => $2,
      process => $3,
      type => "open"
    };
  }
  if (/Port geschlossen/) {
    return {
      type => "close", 
      port => $2,
    };
  }
  if (/Answer: /) {
    return {
      type => "read",
      data => []
    };
  }
  if (/Request: /) {

    return {
      type => "write",
      data => []
    };
  }

  if (/ (.. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..)   ................/) {
    return {
      type => "data",
      data => $1
    };
  }

  die "failed to parse line: '$line' (line $.)";
}


sub parse_file {

  my @entries;
  my $current_entry;
  while(<>) {
    chomp;
    s/\r//;

    my $entry = parse_line($_);
    die "failed to parse line: '$_'" unless $entry;
    if ($entry->{type} eq "open") {
    }
    elsif ($entry->{type} eq "close") {
    }
    elsif ($entry->{type} eq "read" or $entry->{type} eq "write") {
      $current_entry = $entry;
      push (@entries, $current_entry);
    }
    elsif ($entry->{type} eq "data") {
      push (@{$current_entry->{data}}, $entry->{data});
    }
    elsif ($entry->{type} eq "noop") {
      #no-op
    }
    else {
      die "unknown entry type $entry->{type}";
    }
  }
  return \@entries;
}

our $rw;


sub foreach_entry {
  my ($entries, $filter, $callback) = @_;
  foreach my $entry (@$entries) {
    if ($entry->{type} =~ m/$filter/) {
      local $_ = $entry;
      local $rw = $entry->{type};
      $rw =~ s/(.).*/$1/;
      $callback->($entry);
    }
  }
}

sub postproc_data {
  my ($entries) = @_;
  foreach_entry($entries, "read|write", sub {
      my $data;
      $data = join(" ", map { $_ } @{$_->{data}});
      $data =~ s/ //g;
      $_->{data} = $data;
    });
}


my $entries = parse_file();

postproc_data($entries);

open(my $read_fh, ">", "from_watch.bin") or die $!;
binmode($read_fh);

open(my $write_fh, ">", "to_watch.bin") or die $!;
binmode($write_fh);

my $read_addr;

foreach my $entry (@$entries) {
  if ($entry->{data} =~ /A0A2(?<len>....)(?<opcode>..)(?<payload>(..)*)(?<checkum>....)B0B3/) {
    my $opcode = $+{opcode};
    my $data = $+{payload};

    print "#$opcode ";

    if ($opcode eq "10") {
      print "q: version";
    }
    elsif ($opcode eq "11") {
      print "a: version";
    }
    elsif ($opcode eq "12") {
      print "q: data";
      $data =~ /(?<r_addr>......)(?<r_len>..)/;
      $read_addr = $+{r_addr};
      $read_addr =~ s/(..)(..)(..)/$3$2$1/;
    }
    elsif ($opcode eq "13") {
      print "a: data";
      seek($read_fh, hex $read_addr, 0);
      print $read_fh pack('H*', $data);
    }
    elsif ($opcode eq "2C") {
      print "q: version long";
    }
    elsif ($opcode eq "2D") {
     print "a: version long";
    }
    elsif ($opcode eq "2E") {
      my $w_addr = $data;
      $w_addr =~ /(..)(..)(..)/;
      my $year = hex $1;
      my $month = hex $2;
      my $unk = hex $3;
      print "q: unknown, after EPO download $w_addr $year/$month/$unk [$3]";
    }
    elsif ($opcode eq "2F") {
     print "a: unknown, after EPO download $data";
    }
    elsif ($opcode eq "16") {
      $data =~ /(?<w_addr>......)(?<w_len>..)(?<w_data>(..)*)/;
      my $w_addr = $+{w_addr};
      my $w_len = hex $+{w_len};
      my $w_data = $+{w_data};
      $w_addr =~ s/(..)(..)(..)/$3$2$1/;
      print "q: write data $w_addr $w_len";
      seek($write_fh, hex $w_addr, 0);
      print $write_fh pack('H*', $w_data);

    }
    elsif ($opcode eq "17") {
      print "a: write data";
    }
    elsif ($opcode eq "24") {
     print "q: clear flash 1";
    }
    elsif ($opcode eq "25") {
      print "a: clear flash 1";
    }
    elsif ($opcode eq "14") {
      my $w_addr = $data;
      $w_addr =~ s/(..)(..)(..)/$3$2$1/;
      print "q: clear flash 2 $w_addr";
    }
    elsif ($opcode eq "15") {
      print "a: clear flash 2";
    }
    else {
      print "unknown: $opcode <$data>";
    }

    my $len = (hex $+{len}) - 1;
    #print " #$opcode [$+{payload}][$len]";
    print "\n";

  }
  else {
    die "failed to decode $entry->{data}";
  }
}

#print Dumper($entries);


