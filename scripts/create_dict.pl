#!/usr/bin/perl -w

if($#ARGV != 2) {
	print STDERR "Usage: create_dict_tables.pl char_table cedict output\n";
	exit 1;
}

open FREQ_TABLE, "<", $ARGV[0];
open CEDICT, "<", $ARGV[1];

$freq = <FREQ_TABLE>;
my @entries;

$|++;
$l = 0;

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
	if($nc == 0) {
		print STDERR "skip[$simp]";
		next;
	}
	print STDERR "." if($l++%1000 == 0);
	$def =~ s/\p{Han}+\|(\p{Han}+)/$1/g;
	push @entries, {freq=>($sum/$nc), simp=>$simp, py=>lc($py), def=>$def};
}

print STDERR "sorting...";

@sorted = sort { $a->{freq} <=> $b->{freq} } @entries;

print STDERR "writing...";

open DICT, ">", $ARGV[2];

for $e (@sorted) {
	print DICT "\x1e$e->{simp}\0$e->{py}\0$e->{def}\0";
}

print STDERR "done.\n";

