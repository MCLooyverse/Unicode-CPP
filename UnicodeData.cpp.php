<?php
$in_fields = array(
	'point', 'name',
	'category',
	'combining', 'bidi',
	'decomp',
	'decimal', 'digit', 'numeric',
	'mirrored',
	'alias', 'comment',
	'toupper', 'tolower', 'totitle'
);

$Categories = array(
	'Lu' => 'kLetterUppercase',
	'Ll' => 'kLetterLowercase',
	'Lt' => 'kLetterTitlecase',
	'Lm' => 'kLetterModifier',
	'Lo' => 'kLetterOther',
	'Mn' => 'kMarkNonspacing',
	'Mc' => 'kMarkSpacingCombining',
	'Me' => 'kMarkEnclosing',
	'Nd' => 'kNumberDecimalDigit',
	'Nl' => 'kNumberLetter',
	'No' => 'kNumberOther',
	'Pc' => 'kPunctuationConnector',
	'Pd' => 'kPunctuationDash',
	'Ps' => 'kPunctuationOpen',
	'Pe' => 'kPunctuationClose',
	'Pi' => 'kPunctuationInitialQuote',
	'Pf' => 'kPunctuationFinalQuote',
	'Po' => 'kPunctuationOther',
	'Sm' => 'kSymbolMath',
	'Sc' => 'kSymbolCurrency',
	'Sk' => 'kSymbolModifier',
	'So' => 'kSymbolOther',
	'Zs' => 'kSeparatorSpace',
	'Zl' => 'kSeparatorLine',
	'Zp' => 'kSeparatorParagraph',
	'Cc' => 'kOtherControl',
	'Cf' => 'kOtherFormat',
	'Cs' => 'kOtherSurrogate',
	'Co' => 'kOtherPrivateUse',
	'Cn' => 'kOtherNotAssigned'
);

$DecompTypes = array(
	'' => 'canonical',
	'<font>' => 'font',
	'<noBreak>' => 'noBreak',
	'<initial>' => 'initialForm',
	'<medial>' => 'medialForm',
	'<final>' => 'finalForm',
	'<isolated>' => 'isolatedForm',
	'<circle>' => 'circle',
	'<super>' => 'super',
	'<sub>' => 'sub',
	'<vertical>' => 'vertical',
	'<wide>' => 'wide',
	'<narrow>' => 'narrow',
	'<small>' => 'small',
	'<square>' => 'square',
	'<fraction>' => 'fraction',
	'<compat>' => 'compat'
);

$unicodeData = fopen("/home/mcl/Documents/Unicode/UnicodeData-15-0-0.txt", 'r');

if (!$unicodeData) { ?>
#error "UnicodeData.txt not found"

<?php
	exit(1);
}
?>
#include "UnicodeData.h"

using std::literals::string_literals::operator""s;
const mcl::Point mcl::unicodeData[UNICODE_DATA_LENGTH] = {
<?php

// $DEBUG_LIMIT = 0;
while (($l = fgets($unicodeData)) !== false /* && $DEBUG_LIMIT++ < 5 */):
	$l = array_combine($in_fields, explode(';', trim($l)));
?>
	{
		.point = 0x<?php echo $l['point']; ?>,
		.name  = "<?php echo $l['name']; ?>"s,
		.category = mcl::Point::<?php echo $Categories[$l['category']]; ?>,
		.combining = <?php echo $l['combining']; ?>,
		.bidi = "<?php echo $l['bidi']; ?>"s,
<?php
	$val = strlen($l['decomp']) ? explode(' ', $l['decomp']) : array();
	$type = 'canonical';
	if (count($val) && str_starts_with($val[0], '<'))
	{
		$type = $DecompTypes[array_shift($val)];
	}

	$val = join(", ", array_map((fn($x) => "0x{$x}"), $val));
?>
		.decomp = {
			.type = mcl::Point::Decomp::Type::<?php echo $type; ?>,
			.seq = { <?php echo $val; ?> }
		},
<?php if (strlen($l['decimal'])): ?>
		.numeric = {
			mcl::Point::Numeric::Type::decimal,
			<?php echo $l['decimal']; ?>
		},
<?php elseif (strlen($l['digit'])): ?>
		.numeric = {
			mcl::Point::Numeric::Type::digit,
			<?php echo $l['digit']; ?>
		},
<?php elseif (strlen($l['numeric'])): ?>
		.numeric = {
			mcl::Point::Numeric::Type::numeric,
			<?php echo join(", ", explode("/", $l['numeric'])); ?>
		},
<?php else: ?>
		.numeric = { mcl::Point::Numeric::Type::none },
<?php endif; ?>
		.mirrored = <?php echo $l['mirrored'] === 'Y' ? 'true' : 'false'; ?>,
		.alias = "<?php echo $l['alias']; ?>"s,
		.comment = "<?php echo $l['comment']; ?>"s,
		.toupper = <?php echo strlen($l['toupper']) ? "0x{$l['toupper']}" : 0; ?>,
		.tolower = <?php echo strlen($l['tolower']) ? "0x{$l['tolower']}" : 0; ?>,
		.totitle = <?php echo strlen($l['totitle']) ? "0x{$l['totitle']}" : 0; ?>,
	},
<?php endwhile; ?>
};
