#!/bin/bash
##
## Copyright (c) 2014 Oliver Giles
## All rights reserved.
##
## This file is part of Hanchly (汉查利).
## Hanchly is free software; resdistribution and use are
## permitted under conditions stipulated in the file
## COPYING, which is distributed with Hanchly.
##

export LC_ALL=C

# convert to UTF-8
	iconv -f GB18030 -t UTF-8 |
# strip comments and leave columns hanzi and frequency
	cut -s -f2,3 |
# sort in descending frequency order
	sort -k 2nr |
# stable sort on character only
	sort -s -k 1,1 |
# strip the least common of duplicates
	uniq -w 3 |
# re-sort in descending frequency
	sort -k 2nr |
# truncate. 6000 hanzi should be enough for anyone..
	head -n 6000 |
# only output the characters
	cut -f1 |
# convert EOL to nulls, so each can be a C string
	tr '\n' '\0'
