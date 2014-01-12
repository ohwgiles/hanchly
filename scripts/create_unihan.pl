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

if($#ARGV < 3) {
	print STDERR "Usage: create_dict_tables.pl char_table out_index out_table input_files\n";
	exit 1;
}

my %table = ();
open FREQ_TABLE, "<", $ARGV[0];
my $i = 0;
for(split //, <FREQ_TABLE>) {
	$table{$_} = $i++ if(/\p{Han}/);
}
close FREQ_TABLE;

my %out = ();
for($i = 3; $i <= $#ARGV; ++$i) {
	open INPUT, "<", $ARGV[$i];
	while(<INPUT>) {
		next if /^#/;
		chomp;
		($codePoint, $key, $value) = split /\t/;
		$hz = chr(hex(substr($codePoint, 2)));
		if(exists $table{$hz}) {
			$out{$table{$hz}}{$key} = $value;
		}
	}
	close INPUT;
}

open INDEX, ">", $ARGV[1];
open UNIHAN, ">", $ARGV[2];
my $index = 0;
while(my($char, $hash) = each(%out)) {
	print INDEX pack('S', $index);
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
	$index += length($record);
}
close INDEX;
close UNIHAN;

