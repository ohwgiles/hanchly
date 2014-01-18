#!/usr/bin/perl
##
## Copyright (c) 2014 Oliver Giles
## All rights reserved.
##
## This file is part of Hanchly (汉查利).
## Hanchly is free software; resdistribution and use are
## permitted under conditions stipulated in the file
## COPYING, which is distributed with Hanchly.
##

if($#ARGV < 3) {
	print STDERR "Usage: create_dict_tables.pl char_table out_index out_table input_files\n";
	exit 1;
}

my %table = ();
open FREQ_TABLE, "<:encoding(UTF-8)", $ARGV[0];
my $i = 0;
for(split //, <FREQ_TABLE>) {
	$table{$_} = $i++ if(/\p{Han}/);
}
close FREQ_TABLE;

my $l = 0;
my %out = ();
print STDERR "processing";
for($i = 3; $i <= $#ARGV; ++$i) {
	open INPUT, "<:encoding(UTF-8)", $ARGV[$i];
	while(<INPUT>) {
		next if /^#/;
		chomp;
		($codePoint, $key, $value) = split /\t/;
		$hz = chr(hex(substr($codePoint, 2)));
		print STDERR "." if($l++%3000 == 0);
		if(exists $table{$hz}) {
			$out{$table{$hz}}{$key} = $value;
		}
	}
	close INPUT;
}

open INDEX, ">", $ARGV[1];
open UNIHAN, ">:encoding(UTF-8)", $ARGV[2];
my $index = 0;

print STDERR "sorting...";

@sorted_keys = sort { $a <=> $b } keys %out;

print STDERR "writing...";

foreach $k (@sorted_keys) {
	my($char, $hash) = ($k, $out{$k});
	my $cjpad = 5 - length($hash->{'kCangjie'});
	my ($rad, $plus) = split '.', $hash->{'kRSUnicode'};
	$record =
		pack('C', $hash->{'kFrequency'}) .
		pack('C', $hash->{'kTotalStrokes'}) .
		pack('C', $hash->{'kGradeLevel'}) .
		pack('C', $rad) .
		pack('C', $plus) .
		$hash->{'kCangjie'} . pack("C$cjpad",0) .
		$hash->{'kDefinition'} .
		pack('C', 0);
	print UNIHAN $record;
	print INDEX pack('L', $index);
	$index += length($record);
}
print INDEX pack('L',0);
close INDEX;
close UNIHAN;

print STDERR "done\n";
