#!/usr/bin/perl -w
##
## Copyright (c) 2014 Oliver Giles
## All rights reserved.
##
## This file is part of Hanchly (汉查利).
## Hanchly is free software; resdistribution and use are
## permitted under conditions stipulated in the file
## COPYING, which is distributed with Hanchly.
##

if($#ARGV != 2) {
	print STDERR "Usage: create_dict_tables.pl char_table cedict output\n";
	exit 1;
}

open FREQ_TABLE, "<:encoding(UTF-8)", $ARGV[0];
open CEDICT, "<:encoding(UTF-8)", $ARGV[1];

$freq = <FREQ_TABLE>;
my @entries;

$|++;
$l = 0;

print STDERR "processing";

while(<CEDICT>) {
	next if /^#/;
	($simp, $py, $def) = /^\S+\s(\S+)\s\[([^\]]+)\]\s\/(.*)\//;
	my $nc = 0;
	my $sum = 0;
	for $c ($simp =~ m/(\p{Han})/g) {
		next if(not $freq =~ /$c/);
		$sum += index $freq, $c;
		$nc++;
	}
	next if($nc == 0);
	print STDERR "." if($l++%1000 == 0);
	$def =~ s/\p{Han}+\|(\p{Han}+)/$1/g;
	push @entries, {freq=>($sum/$nc), simp=>$simp, py=>lc($py), def=>$def};
}

print STDERR "sorting...";

@sorted = sort { $a->{freq} <=> $b->{freq} } @entries;

print STDERR "writing...";

open DICT, ">:encoding(UTF-8)", $ARGV[2];

for $e (@sorted) {
	print DICT "\x1e$e->{simp}\x1f$e->{py}\x1f$e->{def}\x1f";
}

print STDERR "done.\n";

