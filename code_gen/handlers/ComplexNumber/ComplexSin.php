<?php

class ComplexSin extends Handler {

	use ArrayAddressMode, UnaryOperator, FloatingPointOnly, FixedOperandSize, Slow, Multithreaded;
	
	protected function getActionOnUnitData() {
		$type = $this->getOperandType(2);
		$cType = $this->getOperandCType(2);
		$f = ($type == 'F32') ? 'f' : '';
		$lines = array();
		$lines[] = "$cType r = sin$f(op1_ptr[0]) * cosh$f(op1_ptr[1]);";
		$lines[] = "$cType i = cos$f(op1_ptr[0]) * sinh$f(op1_ptr[1]);";
		$lines[] = "res_ptr[0] = r;";
		$lines[] = "res_ptr[1] = i;";
		return $lines;
	}
}

?>