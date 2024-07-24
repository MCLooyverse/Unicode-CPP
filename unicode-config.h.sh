#!/bin/sh

source ./config.sh

lineCount=$(wc -l "$UNICODE_DATA" | cut -f1 -d' ')

raku -pe "s/ '\$[line-count]' /${lineCount}UL/" unicode-config.h.template \
	> unicode-config.h
