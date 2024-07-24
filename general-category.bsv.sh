#!/bin/sh

jq -r '.[] | "\(.code)|\(.cat)"' wikipedia-general-category.json |
	raku -pe 'my @m = m:g/\w+/; $_ = "@m[0]|k" ~ (@m[1..*]».wordcase).join()' \
	> "${0%.*}"
