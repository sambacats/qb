--TEST--
Complex number hyperbolic cosine test
--FILE--
<?php 

/**
 * A test function
 *
 * @engine	qb 
 * @local	float32[2]		$a
 * @local	float32[3][2]	$c
 *
 * @return	void
 *
 */
function test_function() {
	$a = array(2, 4);
	$c[0] = array(3, -9);
	$c[1] = array(-13, 1);
	$c[2] = array(3.5, -2);
	
	$c = ccosh($c);
	echo ccosh($a), "\n";
	echo "$c\n";
}

test_function();

?>
--EXPECT--
[-2.459135, -2.744817]
[[-9.172952, -4.128551], [119518.5, -186139], [-6.896729, -15.04217]]
