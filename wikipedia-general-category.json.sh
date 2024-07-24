#!/bin/sh

curl https://en.wikipedia.org/wiki/Unicode_character_property |
	pup -p ':parent-of(#General_Category) + p + table json{}' |
	jq -L ~/.local/include/jq 'include "mcl";
		.[] | collectRows | .rows | map(select(length == 6)) |
		map(map(extractText)) | map({
			code: .[0],
			cat: .[1],
			type: .[2],
			char: .[3],
			count: .[4],
			comment: .[5]
		})' \
	> "${0%.*}"
