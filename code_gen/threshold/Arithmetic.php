<?php

class Add_I08 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int8[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a += M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Decrement_I08 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int8[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a--;
		$e = time();
		return ($e - $s);
	}
}

class Increment_I08 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int8[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a++;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_S08 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int8[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_U08 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param uint8[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Negate_I08 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int8[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Subtract_I08 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int8[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Add_I16 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int16[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a += M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Decrement_I16 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int16[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a--;
		$e = time();
		return ($e - $s);
	}
}

class Increment_I16 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int16[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a++;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_S16 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int16[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_U16 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param uint16[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Negate_I16 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int16[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Subtract_I16 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int16[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Add_I32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a += M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Decrement_I32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a--;
		$e = time();
		return ($e - $s);
	}
}

class Increment_I32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a++;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_S32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_U32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param uint32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class MultiplyAccumulate_S32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int32[] $a
	 * @local int32[] $b
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$b = $a + 9;
		$s = time();
		$a += $b * M_PI;
		$e = time();
		return ($e - $s);
	}
}

class MultiplyAccumulate_U32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param uint32[] $a
	 * @local int32[] $b
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$b = $a + 9;
		$s = time();
		$a += $b * M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Negate_I32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Subtract_I32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Add_I64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a += M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Decrement_I64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a--;
		$e = time();
		return ($e - $s);
	}
}

class Increment_I64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a++;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_S64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_U64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param uint64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class MultiplyAccumulate_S64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int64[] $a
	 * @local int64[] $b
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$b = $a + 9;
		$s = time();
		$a += $b * M_PI;
		$e = time();
		return ($e - $s);
	}
}

class MultiplyAccumulate_U64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param uint64[] $a
	 * @local int64[] $b
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$b = $a + 9;
		$s = time();
		$a += $b * M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Negate_I64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Subtract_I64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param int64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Add_F32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a += M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Add_F32_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float32[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a += M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Add_F32_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float32[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a += M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Add_F32_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float32[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a += M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Decrement_F32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a--;
		$e = time();
		return ($e - $s);
	}
}

class Decrement_F32_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float32[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a--;
		$e = time();
		return ($e - $s);
	}
}

class Decrement_F32_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float32[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a--;
		$e = time();
		return ($e - $s);
	}
}

class Decrement_F32_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float32[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a--;
		$e = time();
		return ($e - $s);
	}
}

class Divide_F32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a /= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Divide_F32_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float32[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a /= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Divide_F32_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float32[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a /= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Divide_F32_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float32[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a /= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class FlooredDivisionModulo_F32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = mod($a, M_PI);
		$e = time();
		return ($e - $s);
	}
}

class Increment_F32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a++;
		$e = time();
		return ($e - $s);
	}
}

class Increment_F32_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float32[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a++;
		$e = time();
		return ($e - $s);
	}
}

class Increment_F32_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float32[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a++;
		$e = time();
		return ($e - $s);
	}
}

class Increment_F32_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float32[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a++;
		$e = time();
		return ($e - $s);
	}
}

class Modulo_F32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a %= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Modulo_F32_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float32[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a %= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Modulo_F32_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float32[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a %= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Modulo_F32_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float32[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a %= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_F32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_F32_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float32[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_F32_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float32[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_F32_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float32[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class MultiplyAccumulate_F32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float32[] $a
	 * @local float32[] $b
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$b = $a + 9;
		$s = time();
		$a += $b * M_PI;
		$e = time();
		return ($e - $s);
	}
}

class MultiplyAccumulate_F32_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float32[][2] $a
	 * @local float32[] $b
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$b = $a + 9;
		$s = time();
		$a += $b * M_PI;
		$e = time();
		return ($e - $s);
	}
}

class MultiplyAccumulate_F32_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float32[][3] $a
	 * @local float32[] $b
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$b = $a + 9;
		$s = time();
		$a += $b * M_PI;
		$e = time();
		return ($e - $s);
	}
}

class MultiplyAccumulate_F32_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float32[][4] $a
	 * @local float32[] $b
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$b = $a + 9;
		$s = time();
		$a += $b * M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Negate_F32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Negate_F32_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float32[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Negate_F32_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float32[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Negate_F32_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float32[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Subtract_F32 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float32[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Subtract_F32_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float32[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Subtract_F32_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float32[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Subtract_F32_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float32[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Add_F64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a += M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Add_F64_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float64[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a += M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Add_F64_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float64[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a += M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Add_F64_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float64[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a += M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Decrement_F64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a--;
		$e = time();
		return ($e - $s);
	}
}

class Decrement_F64_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float64[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a--;
		$e = time();
		return ($e - $s);
	}
}

class Decrement_F64_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float64[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a--;
		$e = time();
		return ($e - $s);
	}
}

class Decrement_F64_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float64[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a--;
		$e = time();
		return ($e - $s);
	}
}

class Divide_F64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a /= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Divide_F64_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float64[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a /= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Divide_F64_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float64[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a /= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Divide_F64_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float64[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a /= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class FlooredDivisionModulo_F64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = mod($a, M_PI);
		$e = time();
		return ($e - $s);
	}
}

class Increment_F64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a++;
		$e = time();
		return ($e - $s);
	}
}

class Increment_F64_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float64[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a++;
		$e = time();
		return ($e - $s);
	}
}

class Increment_F64_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float64[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a++;
		$e = time();
		return ($e - $s);
	}
}

class Increment_F64_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float64[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a++;
		$e = time();
		return ($e - $s);
	}
}

class Modulo_F64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a %= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Modulo_F64_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float64[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a %= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Modulo_F64_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float64[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a %= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Modulo_F64_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float64[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a %= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_F64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_F64_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float64[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_F64_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float64[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Multiply_F64_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float64[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a *= M_PI;
		$e = time();
		return ($e - $s);
	}
}

class MultiplyAccumulate_F64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float64[] $a
	 * @local float64[] $b
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$b = $a + 9;
		$s = time();
		$a += $b * M_PI;
		$e = time();
		return ($e - $s);
	}
}

class MultiplyAccumulate_F64_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float64[][2] $a
	 * @local float64[] $b
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$b = $a + 9;
		$s = time();
		$a += $b * M_PI;
		$e = time();
		return ($e - $s);
	}
}

class MultiplyAccumulate_F64_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float64[][3] $a
	 * @local float64[] $b
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$b = $a + 9;
		$s = time();
		$a += $b * M_PI;
		$e = time();
		return ($e - $s);
	}
}

class MultiplyAccumulate_F64_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float64[][4] $a
	 * @local float64[] $b
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$b = $a + 9;
		$s = time();
		$a += $b * M_PI;
		$e = time();
		return ($e - $s);
	}
}

class Negate_F64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Negate_F64_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float64[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Negate_F64_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float64[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Negate_F64_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float64[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Subtract_F64 {
	use SequentialNumbers;

	/**
	 * @engine qb
	 * @param float64[] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Subtract_F64_X2 {
	use SequentialNumbers;

	var $vector_size = 2;

	/**
	 * @engine qb
	 * @param float64[][2] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Subtract_F64_X3 {
	use SequentialNumbers;

	var $vector_size = 3;

	/**
	 * @engine qb
	 * @param float64[][3] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

class Subtract_F64_X4 {
	use SequentialNumbers;

	var $vector_size = 4;

	/**
	 * @engine qb
	 * @param float64[][4] $a
	 * @local float64 $(s|e)
	 * @return float64
	 */
	function test($a) {
		$s = time();
		$a = -$a;
		$e = time();
		return ($e - $s);
	}
}

?>
