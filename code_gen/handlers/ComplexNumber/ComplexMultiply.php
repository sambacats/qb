<?php

class ComplexMultiply extends Handler {

	use ArrayAddressMode, BinaryOperator, FloatingPointOnly, FixedOperandSize, Slow, Multithreaded;

	protected function getActionOnUnitData() {
		$type = $this->getOperandType(3);
		$cType = $this->getOperandCType(3);
		$f = ($type == 'F32') ? 'f' : '';
		$lines = array();
		$lines[] = "$cType r = (op1_ptr[0] * op2_ptr[0]) - (op1_ptr[1] * op2_ptr[1]);";
		$lines[] = "$cType i = (op1_ptr[0] * op2_ptr[1]) + (op1_ptr[1] * op2_ptr[0]);";
		$lines[] = "res_ptr[0] = r;";
		$lines[] = "res_ptr[1] = i;";
		return $lines;
	}
}

?>