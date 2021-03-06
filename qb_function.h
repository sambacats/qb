/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Chung Leong <cleong@cal.berkeley.edu>                        |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef QB_FUNCTION_H_
#define QB_FUNCTION_H_

typedef struct qb_variable					qb_variable;
typedef struct qb_native_code_bundle		qb_native_code_bundle;

typedef enum qb_external_symbol_type		qb_external_symbol_type;

enum {
	QB_VARIABLE_LOCAL				= 0x00000001,
	QB_VARIABLE_ARGUMENT			= 0x00000002,
	QB_VARIABLE_STATIC				= 0x00000004,
	QB_VARIABLE_GLOBAL				= 0x00000008,
	QB_VARIABLE_CLASS				= 0x00000010,
	QB_VARIABLE_CLASS_INSTANCE		= 0x00000020,
	QB_VARIABLE_RETURN_VALUE		= 0x00000040,
	QB_VARIABLE_SHARED				= 0x00000080,
	QB_VARIABLE_CLASS_CONSTANT		= 0x00000100,
	QB_VARIABLE_RETURN_KEY_VALUE	= 0x00000200,
	QB_VARIABLE_SENT_VALUE			= 0x00000400,
	QB_VARIABLE_THIS				= 0x00000800,
	QB_VARIABLE_LEXICAL				= 0x00001000,
	QB_VARIABLE_TYPES				= 0x0000FFFF,

	QB_VARIABLE_BY_REF				= 0x00010000,
	QB_VARIABLE_IMPORTED			= 0x00020000,
};

struct qb_variable {
	uint32_t flags;
	qb_address *address;
	const char *name;
	uint32_t name_length;
	ulong hash_value;
	zend_class_entry *zend_class;
	zval *default_value;
	zval *value;
	zval **value_pointer;
};

enum qb_external_symbol_type {
	QB_EXT_SYM_ZEND_FUNCTION,
	QB_EXT_SYM_STATIC_ZEND_FUNCTION,
	QB_EXT_SYM_ZEND_CLASS,
	QB_EXT_SYM_PCRE,
};

struct qb_external_symbol {
	qb_external_symbol_type type;
	const char *name;
	uint32_t name_length;
	void *pointer;
};

enum {
	QB_NATIVE_SYMBOL_INLINE_FUNCTION			= 0x0001,
	QB_NATIVE_SYMBOL_INTRINSIC_FUNCTION			= 0x0002,
};

struct qb_native_symbol {
	const char *name;
	void *address;
	long hash_value;
	long flags;
};

struct qb_native_proc_record {
	uint64_t crc64;
	void *proc;
};

enum {
	QB_FUNCTION_NATIVE_IF_POSSIBLE	= 0x00000001,
	QB_FUNCTION_NEVER_NATIVE		= 0x00000002,
	QB_FUNCTION_GO_THRU_ZEND		= 0x00000004,
	QB_FUNCTION_INITIALIZED			= 0x00000008,
	QB_FUNCTION_RELOCATED			= 0x00000010,
	QB_FUNCTION_REENTRANCE_COPY		= 0x00000020,
	QB_FUNCTION_FORKED_COPY			= 0x00000040,
	QB_FUNCTION_INLINEABLE			= 0x00000080,
	QB_FUNCTION_GENERATOR			= 0x00000100,
	QB_FUNCTION_NEVER_INLINE		= 0x00000200,
	QB_FUNCTION_INLINE_ALWAYS		= 0x00000400,
	QB_FUNCTION_RUN_IN_MAIN_THREAD	= 0x00000800,
	QB_FUNCTION_NEED_SHADOWS		= 0x00001000,
	QB_FUNCTION_HAS_BREAKPOINTS		= 0x00002000,
	QB_FUNCTION_MULTITHREADED		= 0x00004000,
	QB_FUNCTION_CLOSURE				= 0x00008000,
};

struct qb_function {
	int8_t *instructions;
	int8_t *instruction_start;
	uint64_t instruction_crc64;
	uint32_t instruction_length;
	uint16_t *instruction_opcodes;
	uint32_t instruction_opcode_count;
	uint32_t flags;
	qb_variable *return_variable;
	qb_variable *return_key_variable;
	qb_variable *sent_variable;
	qb_variable **variables;
	uint32_t variable_count;
	uint32_t argument_count;
	qb_storage *local_storage;
	const char *name;
	uint32_t line_id;
	void *native_proc;
	uintptr_t instruction_base_address;
	uintptr_t local_storage_base_address;
	zend_op_array *zend_op_array;
	qb_function *next_reentrance_copy;
	qb_function *next_forked_copy;
	volatile int32_t in_use;
};

struct qb_native_code_bundle {
	void *memory;
	uint32_t size;
};

#if ZEND_ENGINE_2_1 || ZEND_ENGINE_2_2 || ZEND_ENGINE_2_3
	#define QB_GET_FUNCTION(op_array)		((void *) (op_array)->opcodes[0].op2.u.jmp_addr)
	#define QB_SET_FUNCTION(op_array, p)	(op_array)->opcodes[0].op2.u.jmp_addr = (void *) p
#elif ZEND_ENGINE_2_4 || ZEND_ENGINE_2_5 || ZEND_ENGINE_2_6
	#define QB_GET_FUNCTION(op_array)		((op_array)->opcodes[0].op2.ptr)
	#define QB_SET_FUNCTION(op_array, p)	(op_array)->opcodes[0].op2.ptr = (void *) p
#endif

#define QB_IS_COMPILED(op_array)			((op_array)->opcodes[0].opcode == QB_USER_OPCODE)

#define QB_USER_OPCODE						254

#endif
