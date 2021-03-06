--TEST--
Function call array dimension mismatch test (scalar from PHP)
--FILE--
<?php

/**
 * @engine	qb
 * @inline	never
 * @param	int32[4][8]			$a
 * @param	int32[][3][4]		$b
 */
function other_function($a, $b = array()) {
	echo "$a\n";
	echo "$b\n";
}

/**
 * @engine	qb
 * @local	float32				$a
 */
function test_function() {
	$a = 8.5;
	other_function($a);
}

test_function();

?>
--EXPECT--
[[8, 8, 8, 8, 8, 8, 8, 8], [8, 8, 8, 8, 8, 8, 8, 8], [8, 8, 8, 8, 8, 8, 8, 8], [8, 8, 8, 8, 8, 8, 8, 8]]
[]

