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

#include "qb.h"
#include "qb_translator_pbj.h"

static uint8_t qb_read_pbj_I08(qb_pbj_translator_context *cxt) {
	uint8_t value = 0;
	if(cxt->pbj_data_end - cxt->pbj_data >= 1) { 
		value = *((uint8_t *) cxt->pbj_data);
		cxt->pbj_data += 1;
	}
	return value;
}

static uint16_t qb_read_pbj_I16(qb_pbj_translator_context *cxt) {
	uint16_t value = 0;
	if(cxt->pbj_data_end - cxt->pbj_data >= 2) { 
		value = SWAP_LE_I16(*((uint16_t *) cxt->pbj_data));
		cxt->pbj_data += 2;
	}
	return value;
}

static uint32_t qb_read_pbj_I32(qb_pbj_translator_context *cxt) {
	uint32_t value = 0;
	if(cxt->pbj_data_end - cxt->pbj_data >= 4) { 
		value = SWAP_LE_I32(*((uint32_t *) cxt->pbj_data));
		cxt->pbj_data += 4;
	}
	return value;
}

static float32_t qb_read_pbj_F32(qb_pbj_translator_context *cxt) {
	float32_t value = 0.0f;
	if(cxt->pbj_data_end - cxt->pbj_data >= 4) { 
		uint32_t int_value = SWAP_BE_I32(*((uint32_t *) cxt->pbj_data));
		value = *((float32_t *) &int_value);
		cxt->pbj_data += 4;
	}
	return value;
}

static const char * qb_read_pbj_string(qb_pbj_translator_context *cxt, uint32_t *p_length) {
	const char *value = "";
	if(p_length) {
		uint16_t len = qb_read_pbj_I16(cxt);
		if(cxt->pbj_data_end - cxt->pbj_data >= len) { 
			value = (const char *) cxt->pbj_data;
			*p_length = len;
		} else {
			*p_length = 0;
		}
		cxt->pbj_data += len;
	} else {
		const char *start = (const char *) cxt->pbj_data;
		while(cxt->pbj_data < cxt->pbj_data_end) {
			if(*cxt->pbj_data == '\0') {
				value = start;
				cxt->pbj_data++;
				break;
			}
			cxt->pbj_data++; 
		}
	}
	return value;
}

static qb_pbj_parameter * qb_find_pbj_parameter_by_address(qb_pbj_translator_context *cxt, qb_pbj_address *address) {
	uint32_t i;
	for(i = 0; i < cxt->parameter_count; i++) {
		qb_pbj_parameter *parameter = &cxt->parameters[i];
		qb_pbj_address *destination = &parameter->destination;
		if(destination->register_id == address->register_id) {
			if(destination->dimension > 1) {
				return parameter;
			} else {
				// see if the address is pointing to a channel used by the parameter
				uint32_t j, k;
				for(j = 0; j < address->channel_count; j++) {
					uint32_t channel = address->channels[j];
					for(k = 0; k < destination->channel_count; k++) {
						if(channel == destination->channels[k]) {
							return parameter;
						}
					}
				}
			}
		}
	}
	return NULL;
}

static qb_pbj_texture * qb_find_pbj_texture_by_id(qb_pbj_translator_context *cxt, uint32_t image_id) {
	uint32_t i;
	for(i = 0; i < cxt->texture_count; i++) {
		qb_pbj_texture *texture = &cxt->textures[i];
		if(texture->image_id == image_id) {
			return texture;
		}
	}
	return NULL;
}

static void qb_read_pbj_value(qb_pbj_translator_context *cxt, uint32_t type, qb_pbj_value *value) {
	uint32_t i;
	value->type = type;
	switch(type) {
		case PBJ_TYPE_FLOAT: {
			value->float1 = qb_read_pbj_F32(cxt);
		}	break;
		case PBJ_TYPE_FLOAT2: {
			for(i = 0; i < 2; i++) {
				value->float2[i] = qb_read_pbj_F32(cxt);
			}
		}	break;
		case PBJ_TYPE_FLOAT3: {
			for(i = 0; i < 3; i++) {
				value->float3[i] = qb_read_pbj_F32(cxt);
			}
		}	break;
		case PBJ_TYPE_FLOAT4: {
			for(i = 0; i < 4; i++) {
				value->float4[i] = qb_read_pbj_F32(cxt);
			}
		}	break;
		case PBJ_TYPE_FLOAT2X2: {
			for(i = 0; i < 4; i++) {
				value->float2x2[i] = qb_read_pbj_F32(cxt);
			}
		}	break;
		case PBJ_TYPE_FLOAT3X3: {
			for(i = 0; i < 9; i++) {
				value->float3x3[i] = qb_read_pbj_F32(cxt);
			}
		}	break;
		case PBJ_TYPE_FLOAT4X4: {
			for(i = 0; i < 16; i++) {
				value->float4x4[i] = qb_read_pbj_F32(cxt);
			}
		}	break;
		case PBJ_TYPE_BOOL:
		case PBJ_TYPE_INT: {
			value->int1 = qb_read_pbj_I16(cxt);
		}	break;
		case PBJ_TYPE_BOOL2:
		case PBJ_TYPE_INT2: {
			for(i = 0; i < 2; i++) {
				value->int2[i] = qb_read_pbj_I16(cxt);
			}
		}	break;
		case PBJ_TYPE_BOOL3:
		case PBJ_TYPE_INT3: {
			for(i = 0; i < 3; i++) {
				value->int3[i] = qb_read_pbj_I16(cxt);
			}
		}	break;
		case PBJ_TYPE_BOOL4:
		case PBJ_TYPE_INT4: {
			for(i = 0; i < 4; i++) {
				value->int4[i] = qb_read_pbj_I16(cxt);
			}
		}	break;
		case PBJ_TYPE_STRING: {
			value->string = qb_read_pbj_string(cxt, NULL);
		}	break;
	}
}

static qb_pbj_texture * qb_find_pbj_texture(qb_pbj_translator_context *cxt, const char *name) {
	uint32_t i;
	for(i = 0; i < cxt->texture_count; i++) {
		qb_pbj_texture *texture = &cxt->textures[i];
		if(strcmp(texture->name, name) == 0) {
			return texture;
		}
	}
	return NULL;
}

static void qb_set_pbj_destination_channels(qb_pbj_translator_context *cxt, qb_pbj_address *address, uint32_t channel_mask, uint32_t dimension) {
	// dimension is used only if no destination channels are selected 
	// otherwise dimension is pertinent only to the source
	if(channel_mask) {
		uint32_t channel, bit;
		address->dimension = 1;
		address->channel_count = 0;
		address->all_channels = 0;
		for(bit = 0x08, channel = 0; channel < 4; channel++, bit >>= 1) {
			if(channel_mask & bit) {
				address->channels[address->channel_count++] = channel;
			}
		}
	} else {
		address->dimension = dimension;
		address->channel_count = 0;
	}
}

static void qb_set_pbj_source_channels(qb_pbj_translator_context *cxt, qb_pbj_address *address, uint32_t channel_swizzle, uint32_t channel_count, uint32_t dimension) {
	if(dimension == 1) {
		uint32_t channel, i;
		address->dimension = 1;
		address->channel_count = channel_count;
		address->all_channels = 0;
		for(i = 0; i < channel_count; i++) {
			channel = (channel_swizzle >> ((3 - i) * 2)) & 0x03;
			address->channels[i] = channel;
		}
	} else {
		address->dimension = dimension;
		address->channel_count = 0;
	}
}

void qb_decode_pbj_binary(qb_pbj_translator_context *cxt) {
	uint32_t opcode, prev_opcode, i;
	cxt->pbj_data = (uint8_t *) cxt->compiler_context->external_code;
	cxt->pbj_data_end = cxt->pbj_data + cxt->compiler_context->external_code_length;

	while(cxt->pbj_data < cxt->pbj_data_end) {
		opcode = qb_read_pbj_I08(cxt);
		if(opcode <= PBJ_ALL) {
			uint32_t bit_fields, channel_mask, channel_swizzle, channel_count, dimension;
			qb_pbj_op *pop;
			uint32_t pop_index;

			if(!cxt->pbj_ops) {				
				// each op is 8 bytes long 
				uint32_t bytes_remaining = (cxt->pbj_data_end - cxt->pbj_data) + 1;
				uint32_t op_count = bytes_remaining / 8;
				if(op_count == 0) {
					return;
				}
				qb_attach_new_array(cxt->pool, (void **) &cxt->pbj_ops, &cxt->pbj_op_count, sizeof(qb_pbj_op), op_count);
			}
			pop_index = cxt->pbj_op_count;
			pop = qb_enlarge_array((void **) &cxt->pbj_ops, 1);
			pop->opcode = opcode;

			// read the destination address
			pop->destination.register_id = qb_read_pbj_I16(cxt);
			bit_fields = qb_read_pbj_I08(cxt);
			channel_mask = bit_fields >> 4;
			channel_count = (bit_fields & 0x03) + 1;
			dimension = ((bit_fields >> 2) & 0x03) + 1;
			qb_set_pbj_destination_channels(cxt, &pop->destination, channel_mask, dimension);

			if(opcode == PBJ_LOAD_CONSTANT) {
				pop->source.channel_count = pop->source.dimension = 0;
				pop->image_id = 0;
				if(pop->destination.register_id & PBJ_REGISTER_INT) {
					pop->constant.int_value = qb_read_pbj_I32(cxt);
					pop->constant.type = PBJ_TYPE_INT;
				} else {			
					pop->constant.float_value = qb_read_pbj_F32(cxt);
					pop->constant.type = PBJ_TYPE_FLOAT;
				}
				pop->flags = PBJ_DESTINATION_IN_USE;
			} else {
				// read the source
				pop->source.register_id = qb_read_pbj_I16(cxt);
				channel_swizzle = qb_read_pbj_I08(cxt);
				qb_set_pbj_source_channels(cxt, &pop->source, channel_swizzle, channel_count, dimension);

				// read the image id (only used for sampling functions)
				pop->image_id = qb_read_pbj_I08(cxt);

				if(opcode == PBJ_IF) {
					// push the instruction onto the conditional stack
					qb_pbj_op **p_conditional_pop = qb_enlarge_array((void **) &cxt->conditionals, 1);
					*p_conditional_pop = pop;
					pop->destination.dimension = 0;
					pop->branch_target_index = 0;
					pop->flags = PBJ_SOURCE_IN_USE;
				} else if(opcode == PBJ_ELSE || opcode == PBJ_END_IF) {
					// update the branch target index of the op at the top of the stack
					qb_pbj_op *related_pop;
					if(cxt->conditional_count == 0) {
						qb_abort("unexpected opcode: %02x", opcode);
					}
					related_pop = cxt->conditionals[cxt->conditional_count - 1];
					related_pop->branch_target_index = pop_index;
					if(opcode == PBJ_ELSE) {
						// replace the if with the else op
						cxt->conditionals[cxt->conditional_count - 1] = pop;
					} else {
						// pop the item off
						cxt->conditional_count--;
					}
					pop->source.dimension = 0;
					pop->destination.dimension = 0;
					pop->branch_target_index = 0;
					pop->flags = 0;
				} else if(opcode == PBJ_SELECT) {
					// put the on-true and on-false sources in the next op
					qb_pbj_op *data_pop = qb_enlarge_array((void **) &cxt->pbj_ops, 1);
					data_pop->opcode = PBJ_OP_DATA;
					data_pop->source2.register_id = qb_read_pbj_I16(cxt);
					channel_swizzle = qb_read_pbj_I08(cxt);
					bit_fields = qb_read_pbj_I08(cxt);
					channel_count = (bit_fields >> 6) + 1;
					qb_set_pbj_source_channels(cxt, &data_pop->source2, channel_swizzle, channel_count, dimension);

					data_pop->source3.register_id = qb_read_pbj_I16(cxt);
					channel_swizzle = qb_read_pbj_I08(cxt);
					bit_fields = qb_read_pbj_I08(cxt);
					channel_count = (bit_fields >> 6) + 1;
					qb_set_pbj_source_channels(cxt, &data_pop->source3, channel_swizzle, channel_count, dimension);
					data_pop->flags = PBJ_SOURCE_IN_USE | PBJ_DESTINATION_IN_USE;
					pop->flags = PBJ_SOURCE_IN_USE | PBJ_DESTINATION_IN_USE;
				} else if(opcode == PBJ_SAMPLE_BILINEAR || opcode == PBJ_SAMPLE_NEAREST) {
					pop->flags = PBJ_IMAGE_ID_IN_USE | PBJ_SOURCE_IN_USE | PBJ_DESTINATION_IN_USE; 
				} else {
					pop->flags = PBJ_SOURCE_IN_USE | PBJ_DESTINATION_IN_USE;
				}
			}
		} else if(opcode == PBJ_KERNEL_METADATA || opcode == PBJ_PARAMETER_METADATA) {
			uint8_t value_type;
			const char *value_name;
			qb_pbj_parameter *last_parameter;
			qb_pbj_value *value = NULL, tmp;
			value_type = qb_read_pbj_I08(cxt);
			value_name = qb_read_pbj_string(cxt, NULL);
			if(opcode == PBJ_PARAMETER_METADATA && cxt->parameter_count > 0) {
				last_parameter = &cxt->parameters[cxt->parameter_count - 1];
				if(strcmp(value_name, "minValue") == 0) {
					value = &last_parameter->min_value;
				} else if(strcmp(value_name, "maxValue") == 0) {
					value = &last_parameter->max_value;
				} else if(strcmp(value_name, "defaultValue") == 0) {
					value = &last_parameter->default_value;
				}
			}
			if(!value) {
				value = &tmp;
			}
			qb_read_pbj_value(cxt, value_type, value);
			if(opcode == PBJ_PARAMETER_METADATA && value_type == PBJ_TYPE_STRING) {
				if(strcmp(value_name, "parameterType") == 0) {
					last_parameter->parameter_type = value->string;
				} else if(strcmp(value_name, "inputSizeName") == 0) {
					last_parameter->input_size_name = value->string;
				} else if(strcmp(value_name, "description") == 0) {
					last_parameter->description = value->string;
				} else if(strcmp(value_name, "displayName") == 0) {
					last_parameter->display_name = value->string;
				} else if(strcmp(value_name, "parameterType") == 0) {
					last_parameter->parameter_type = value->string;
				} 
			} else if(opcode == PBJ_KERNEL_METADATA) {
				if(strcmp(value_name, "description") == 0) {
					cxt->description = value->string;
				} else if(strcmp(value_name, "vendor") == 0) {
					cxt->vendor = value->string;
				} else if(strcmp(value_name, "displayName") == 0) {
					cxt->display_name = value->string;
				} else if(strcmp(value_name, "version") == 0) {
					cxt->version = value->int1;
				}
			}
		} else if(opcode == PBJ_PARAMETER_DATA) {
			qb_pbj_parameter *parameter = qb_enlarge_array((void **) &cxt->parameters, 1);
			uint32_t bit_fields, dimension, channel_mask;
			parameter->qualifier = qb_read_pbj_I08(cxt);
			parameter->type = qb_read_pbj_I08(cxt);
			parameter->destination.register_id = qb_read_pbj_I16(cxt);
			bit_fields = qb_read_pbj_I08(cxt);
			switch(parameter->type) {
				case PBJ_TYPE_FLOAT2X2: 
				case PBJ_TYPE_FLOAT3X3: 
				case PBJ_TYPE_FLOAT4X4: 
					dimension = (parameter->type - PBJ_TYPE_FLOAT2X2) + 2;
					channel_mask = 0;
					break;
				default:
					dimension = 1;
					channel_mask = bit_fields & 0x0F;
					break;
			}
			qb_set_pbj_destination_channels(cxt, &parameter->destination, channel_mask, dimension);
			parameter->name = qb_read_pbj_string(cxt, NULL);
			parameter->address = NULL;
			parameter->parameter_type = NULL;
			parameter->description = NULL;
			parameter->input_size_name = NULL;
			memset(&parameter->default_value, 0, sizeof(qb_pbj_value));
			memset(&parameter->min_value, 0, sizeof(qb_pbj_value));
			memset(&parameter->max_value, 0, sizeof(qb_pbj_value));
		} else if(opcode == PBJ_TEXTURE_DATA) {
			qb_pbj_texture *texture = qb_enlarge_array((void **) &cxt->textures, 1);
			texture->image_id = qb_read_pbj_I08(cxt);
			texture->channel_count = qb_read_pbj_I08(cxt);
			texture->name = qb_read_pbj_string(cxt, NULL);
			texture->address = NULL;
			texture->input_size = NULL;
		} else if(opcode == PBJ_KERNEL_NAME) {
			cxt->pbj_name = qb_read_pbj_string(cxt, &cxt->name_length);
		} else if(opcode == PBJ_VERSION_DATA) {
			qb_read_pbj_I32(cxt);
		} else {
			qb_abort("unknown opcode: %02x (previous opcode = %02x)", opcode, prev_opcode);
			break;
		}
		prev_opcode = opcode;
	}
	for(i = 0; i < cxt->parameter_count; i++) {
		qb_pbj_parameter *parameter = &cxt->parameters[i];
		if(strcmp(parameter->name, "_OutCoord") == 0) {
			cxt->out_coord = parameter;
		} else if(parameter->qualifier == PBJ_PARAMETER_OUT) {
			cxt->out_pixel = parameter;
		} else if(parameter->input_size_name) {
			qb_pbj_texture *texture = qb_find_pbj_texture(cxt, parameter->input_size_name);
			if(texture) {
				texture->input_size = parameter;
			}
		}
	}

	if(!cxt->out_pixel) {
		qb_abort("missing output pixel");
	}
	if(!cxt->out_coord) {
		qb_abort("missing output pixel coordinates");
	}
	if(cxt->conditional_count > 0) {
		qb_abort("missing end if");
	}
	if(cxt->out_pixel->type != PBJ_TYPE_FLOAT3 && cxt->out_pixel->type != PBJ_TYPE_FLOAT4) {
		qb_abort("output pixel is not float3 or float4");
	}
}

static void qbj_translate_pbj_basic_op(qb_pbj_translator_context *cxt, qb_pbj_translator *t, qb_operand *operands, uint32_t operand_count, qb_operand *result, qb_result_prototype *result_prototype) {
	qb_produce_op(cxt->compiler_context, t->extra, operands, operand_count, result, NULL, 0, result_prototype);
}

static int32_t qb_is_read_only(qb_pbj_translator_context *cxt, qb_address *address) {
	if(!READ_ONLY(address)) {
		return FALSE;
	}
	if(address->source_address) {
		return qb_is_read_only(cxt, address->source_address);
	}
	return TRUE;
}

static void qb_perform_assignment(qb_pbj_translator_context *cxt, qb_address *dst_address, qb_address *src_address);
static void qb_perform_branch(qb_pbj_translator_context *cxt, qb_address *condition_address, uint32_t jump_target_index);
static void qb_perform_jump(qb_pbj_translator_context *cxt, uint32_t jump_target_index);
static void qb_mark_next_op_as_jump_target(qb_pbj_translator_context *cxt, uint32_t src_index);

static void qb_translate_pbj_load_constant(qb_pbj_translator_context *cxt, qb_pbj_translator *t, qb_operand *operands, uint32_t operand_count, qb_operand *result, qb_result_prototype *result_prototype) {
	qb_operand *value = &operands[0];
	if(cxt->compiler_context->stage == QB_STAGE_RESULT_TYPE_RESOLUTION) {
		qb_primitive_type expr_type;
		switch(value->pbj_constant->type) {
			case PBJ_TYPE_INT: expr_type = QB_TYPE_S32; break;
			case PBJ_TYPE_FLOAT: expr_type = QB_TYPE_F32; break;
			default: break;
		}
		result_prototype->preliminary_type = result_prototype->final_type = expr_type;
		if(result->type != QB_OPERAND_ADDRESS) {
			result->result_prototype = result_prototype;
			result->type = QB_OPERAND_RESULT_PROTOTYPE;
			result_prototype->address_flags = QB_ADDRESS_TEMPORARY;
		}
	} else {
		if(result->type == QB_OPERAND_ADDRESS && qb_is_read_only(cxt, result->address)) {
			// since we've clear the read-only flags of registers that get written to
			// we know that the destination can be treated as a constant
			qb_mark_as_constant(cxt->compiler_context, result->address);
			if(result->address->segment_selector == QB_SELECTOR_INVALID) {
				qb_allocate_storage_space(cxt->compiler_context, result->address, TRUE);
			}
			switch(value->pbj_constant->type) {
				case PBJ_TYPE_INT: {
					VALUE(I32, result->address) = value->pbj_constant->int_value;
				}	break;
				case PBJ_TYPE_FLOAT: {
					VALUE(F32, result->address) = value->pbj_constant->float_value;
				}	break;
				default: {
				}	break;
			}
		} else {
			qb_address *base_address, *constant_address, *dst_address = result->address;
			uint32_t constant_count = 1;
			uint32_t expected_constant_count = 1;

			// see if we're writing into an array
			if(result->type == QB_OPERAND_ADDRESS) {
				base_address = result->address;
				while(base_address->source_address) {
					if(base_address->array_index_address == cxt->compiler_context->zero_address) {
						base_address = base_address->source_address;
					} else {
						break;
					}
				}
				if(!SCALAR(base_address)) {
					uint32_t expected_constant_count = ARRAY_SIZE(base_address);
					uint32_t remaining = expected_constant_count;
					uint32_t expected_reg_id = cxt->pbj_op->destination.register_id;
					uint32_t expected_channel = cxt->pbj_op->destination.channels[0];
					qb_pbj_op *pop = cxt->pbj_op;
			
					// see if other load-constant ops follow that write into the same location
					do {
						pop++;
						remaining--;
						if(expected_channel < 3) {
							expected_channel++;
						} else {
							expected_channel = 0;
							expected_reg_id++;
						}
						if(pop->opcode != PBJ_LOAD_CONSTANT || pop->destination.register_id != expected_reg_id || pop->destination.channels[0] != expected_channel) {
							break;
						}
					} while(remaining > 0);

					if(remaining == 0) {
						constant_count = expected_constant_count;
						dst_address = base_address;
					}
				}
			}
			if(constant_count == 1) {
				switch(value->pbj_constant->type) {
					case PBJ_TYPE_INT: {
						constant_address = qb_obtain_constant_S32(cxt->compiler_context, value->pbj_constant->int_value);
					}	break;
					case PBJ_TYPE_FLOAT: {
						constant_address = qb_obtain_constant_F32(cxt->compiler_context, value->pbj_constant->float_value);
					}	break;
					default: {
					}	break;
				}
			} else {
				qb_pbj_op *pop = cxt->pbj_op;
				uint32_t i;
				constant_address = qb_create_constant_array(cxt->compiler_context, dst_address->type, &constant_count, 1);
				for(i = 0; i < constant_count; i++) {
					switch(dst_address->type) {
						case QB_TYPE_S32: {
							ARRAY(S32, constant_address)[i] = pop->constant.int_value;
						}	break;
						case QB_TYPE_F32: {	
							ARRAY(F32, constant_address)[i] = pop->constant.float_value;
						}	break;
					}
					pop++;
				}

				// jump over additional ops that are processed
				cxt->pbj_op_index += (constant_count - 1);
			}
			if((result_prototype->address_flags & QB_ADDRESS_TEMPORARY)) {
				result->address = constant_address;
				result->type = QB_OPERAND_ADDRESS;
			} else {
				qb_perform_assignment(cxt, result->address, constant_address);
			}
		}
	}
}

static void qb_translate_pbj_select(qb_pbj_translator_context *cxt, qb_pbj_translator *t, qb_operand *operands, uint32_t operand_count, qb_operand *result, qb_result_prototype *result_prototype) {
	qb_operand *condition = &operands[0], *true_value = &operands[1], *false_value = &operands[2];

	if(cxt->compiler_context->stage == QB_STAGE_RESULT_TYPE_RESOLUTION) {
		qb_primitive_type expr_type = qb_get_operand_type(cxt->compiler_context, true_value, 0);
		result_prototype->preliminary_type = result_prototype->final_type = expr_type;
		if(result->type != QB_OPERAND_ADDRESS) {
			result->result_prototype = result_prototype;
			result->type = QB_OPERAND_RESULT_PROTOTYPE;
			result_prototype->address_flags = QB_ADDRESS_TEMPORARY;
		}
	} else {
		if(result->type != QB_OPERAND_ADDRESS) {
			qb_variable_dimensions dim = { SCALAR(true_value->address) ? 0 : 1, true_value->address->array_size_address };
			result->address = qb_obtain_write_target(cxt->compiler_context, true_value->address->type, &dim, 0, result_prototype, FALSE);
			result->type = QB_OPERAND_ADDRESS;
		}

		// branch to true block if non-zero, else fall into to false block
		// skipping two instructions
		qb_perform_branch(cxt, condition->address, QB_INSTRUCTION_OFFSET + 3);

		// assignment of false value
		qb_perform_assignment(cxt, result->address, false_value->address);

		// jump over the true block (i.e. going to the code generated by the next pbj instruction)
		qb_perform_jump(cxt, cxt->pbj_op_offset + cxt->pbj_op_index + 1);

		// assignment of true value
		qb_mark_next_op_as_jump_target(cxt, QB_INSTRUCTION_OFFSET);
		qb_perform_assignment(cxt, result->address, true_value->address);

		// flags the next op as jump targets
		qb_mark_next_op_as_jump_target(cxt, cxt->pbj_op_offset + cxt->pbj_op_index + 1);
	}
}

static void qb_translate_pbj_if(qb_pbj_translator_context *cxt, qb_pbj_translator *t, qb_operand *operands, uint32_t operand_count, qb_operand *result, qb_result_prototype *result_prototype) {
	// jump to the instruction immediately following the else or end-if if the condition is false
	uint32_t jump_target_indices[2] = { QB_INSTRUCTION_NEXT, cxt->pbj_op_offset + cxt->pbj_op->branch_target_index + 1 };
	qb_produce_op(cxt->compiler_context, t->extra, operands, operand_count, result, jump_target_indices, 2, result_prototype);
}

static void qb_translate_pbj_else(qb_pbj_translator_context *cxt, qb_pbj_translator *t, qb_operand *operands, uint32_t operand_count, qb_operand *result, qb_result_prototype *result_prototype) {
	// jump over the else block
	uint32_t jump_target_index = cxt->pbj_op_offset + cxt->pbj_op->branch_target_index + 1;
	qb_produce_op(cxt->compiler_context, t->extra, operands, operand_count, result, &jump_target_index, 1, result_prototype);
}

static void qb_translate_pbj_end_if(qb_pbj_translator_context *cxt, qb_pbj_translator *t, qb_operand *operands, uint32_t operand_count, qb_operand *result, qb_result_prototype *result_prototype) {
	// do nothing
}

static int32_t qb_match_pbj_addresses(qb_pbj_translator_context *cxt, qb_pbj_address *reg1_address, qb_pbj_address *reg2_address) {
	return (reg1_address->register_id == reg2_address->register_id && reg1_address->dimension == reg2_address->dimension 
		 && reg1_address->channel_count == reg2_address->channel_count && reg1_address->all_channels == reg2_address->all_channels);
}

typedef struct qb_pbj_op_info {
	uint32_t opcode;
	qb_pbj_address **src_ptr;
	qb_pbj_address **dst_ptr;
} qb_pbj_op_info;

static uint32_t qb_find_op_sequence(qb_pbj_translator_context *cxt, qb_pbj_op_info *seq, uint32_t seq_len) {
	// use Boyer-Moore-Horspool to locate the opcode sequence
	uint32_t i, j, last = seq_len - 1, skip;
    uint32_t bad_opcode_skip[PBJ_MAX_OPCODE + 1];
	for(i = 0; i <= PBJ_MAX_OPCODE; i++) {
        bad_opcode_skip[i] = seq_len;
	}
	for(i = 0; i < last; i++) {
		bad_opcode_skip[seq[i].opcode] = last - i;
	}
	for(j = 0; j + last < cxt->pbj_op_count; j += skip) {
		for(i = last; cxt->pbj_ops[j + i].opcode == seq[i].opcode; i--) {
			if(i == 0) {
				// found the opcode sequence--now verify that the addresses are correct
				int32_t correct = TRUE;
				for(i = 0; i < seq_len && correct; i++) {
					qb_pbj_op *pop = &cxt->pbj_ops[j + i];
					if(pop->opcode == PBJ_LOAD_CONSTANT) {
						// check the constant
						if(pop->constant.type == PBJ_TYPE_INT) {
							if(pop->constant.int_value != *((int32_t *) seq[i].src_ptr)) {
								correct = FALSE;
							}
						} else {
							if(pop->constant.float_value != *((float32_t *) seq[i].src_ptr)) {
								correct = FALSE;
							}
						}
					} else {
						if(seq[i].src_ptr) {
							// check the source:
							// if the pointer is null, set it
							// if not, check that the address match
							if(!*seq[i].src_ptr) {
								*seq[i].src_ptr = &pop->source;
							} else {
								if(!qb_match_pbj_addresses(cxt, &pop->source, *seq[i].src_ptr)) {
									correct = FALSE;
								}
							}
						}
					}
					if(seq[i].dst_ptr) {
						// check the destination--same logic
						if(!*seq[i].dst_ptr) {
							*seq[i].dst_ptr = &pop->destination;
						} else {
							if(!qb_match_pbj_addresses(cxt, &pop->destination, *seq[i].dst_ptr)) {
								correct = FALSE;
							}
						}
					}
				}
				return (correct) ? j : (uint32_t) -1;
			}
		}
		skip = bad_opcode_skip[cxt->pbj_ops[j + last].opcode];
    }
    return (uint32_t) -1;
}

static void qb_substitute_ops(qb_pbj_translator_context *cxt) {
	// look for smoothStep sequence
	qb_pbj_address *x = NULL, *edge0 = NULL, *edge1 = NULL, *result = NULL;
	qb_pbj_address *zero = NULL, *one = NULL, *var1 = NULL, *var2 = NULL, *var3 = NULL; 
	float32_t constants[4] = { 0.0, 1.0, 2.0, 3.0 }; 
	qb_pbj_op_info smooth_step_sequence[18] = { 
		{	PBJ_LOAD_CONSTANT,	(qb_pbj_address **)	&constants[0],	&zero	},
		{	PBJ_LOAD_CONSTANT,	(qb_pbj_address **)	&constants[1],	&one	},
		{	PBJ_COPY,			&x,									&var1	},
		{	PBJ_SUBTRACT,		&edge0,								&var1	},
		{	PBJ_COPY,			&edge1,								&var2	},
		{	PBJ_SUBTRACT,		&edge0,								&var2	},
		{	PBJ_RECIPROCAL,		&var2,								&var3	},
		{	PBJ_MULTIPLY,		&var3,								&var1	},
		{	PBJ_MAX,			&zero,								&var1	},
		{	PBJ_MIN,			&one,								&var1	},
		{	PBJ_LOAD_CONSTANT,	(qb_pbj_address **)	&constants[2],	&var2	},
		{	PBJ_MULTIPLY,		&var1,								&var2	},
		{	PBJ_LOAD_CONSTANT,	(qb_pbj_address **)	&constants[3],	&var3	},
		{	PBJ_SUBTRACT,		&var2,								&var3	},
		{	PBJ_COPY,			&var1,								&var2	},
		{	PBJ_MULTIPLY,		&var1,								&var2	},
		{	PBJ_MULTIPLY,		&var3,								&var2	},
		{	PBJ_COPY,			&var2,								&result	},
	};
	uint32_t start_index, seq_len = sizeof(smooth_step_sequence) / sizeof(qb_pbj_op_info);
	start_index = qb_find_op_sequence(cxt, smooth_step_sequence, seq_len);
	if(start_index != (uint32_t) -1) {
		qb_pbj_op *ss_pop = &cxt->pbj_ops[start_index];
		qb_pbj_op *ss_data_pop = &cxt->pbj_ops[start_index + 1];
		uint32_t i;

		// change the first and second ops to smoothStep
		ss_pop->opcode = PBJ_SMOOTH_STEP;
		memcpy(&ss_pop->source, x, sizeof(qb_pbj_address));
		memcpy(&ss_pop->destination, result, sizeof(qb_pbj_address));
		ss_data_pop->opcode = PBJ_OP_DATA;
		memcpy(&ss_data_pop->source2, edge0, sizeof(qb_pbj_address));
		memcpy(&ss_data_pop->source3, edge1, sizeof(qb_pbj_address));

		// make the other NOP
		for(i = 2; i < seq_len; i++) {
			qb_pbj_op *nop = &cxt->pbj_ops[start_index + i];
			nop->opcode = PBJ_NOP;
		}
	}
}

static int32_t qb_is_image(qb_pbj_translator_context *cxt, qb_address *address, uint32_t channel_count) {
	if(address && address->type == QB_TYPE_F32 && address->dimension_count == 3) {
		if(CONSTANT_DIMENSION(address, -1) && DIMENSION(address, -1) == channel_count) {
			return TRUE;
		}
	}
	return FALSE;
}

static qb_variable * qb_find_argument(qb_pbj_translator_context *cxt, const char *name) {
	uint32_t i;
	for(i = 0; i < cxt->compiler_context->argument_count; i++) {
		qb_variable *qvar = cxt->compiler_context->variables[i];
		if(strcmp(qvar->name, name) == 0) {
			return qvar;
		}
	}
	return NULL;
}

static qb_variable * qb_find_input(qb_pbj_translator_context *cxt) {
	qb_variable *input_var = NULL;
	uint32_t i;
	for(i = 0; i < cxt->compiler_context->argument_count; i++) {
		qb_variable *qvar = cxt->compiler_context->variables[i];
		if(!(qvar->flags & QB_VARIABLE_BY_REF)) {
			if(qb_is_image(cxt, qvar->address, 4) || qb_is_image(cxt, qvar->address, 3)) {
				if(!input_var) {
					input_var = qvar;
				} else {
					// if there's more than one then the name ought to match
				}
			}
		}
	}
	return input_var;
}

static qb_variable * qb_find_output(qb_pbj_translator_context *cxt) {
	uint32_t i;
	for(i = 0; i < cxt->compiler_context->argument_count; i++) {
		qb_variable *qvar = cxt->compiler_context->variables[i];
		if(qvar->flags & QB_VARIABLE_BY_REF) {
			if(qb_is_image(cxt, qvar->address, 4) || qb_is_image(cxt, qvar->address, 3)) {
				return qvar;
			}
		}
	}
	return NULL;
}

static qb_pbj_register * qb_create_pbj_register(qb_pbj_translator_context *cxt, qb_pbj_address *reg_address) {
	qb_pbj_register **p_regs, *reg;
	qb_pbj_register_slot **p_slots;
	uint32_t id, reg_required, end, *p_count;

	if(reg_address->register_id & PBJ_REGISTER_INT) {
		id = reg_address->register_id & ~PBJ_REGISTER_INT;
		p_regs = &cxt->int_registers;
		p_slots = &cxt->int_register_slots;
		p_count = &cxt->int_register_count;
	} else {
		id = reg_address->register_id;
		p_regs = &cxt->float_registers;
		p_slots = &cxt->float_register_slots;
		p_count = &cxt->float_register_count;
	}

	// matrices could span more than one register
	if(reg_address->dimension > 1) {
		switch(reg_address->dimension) {
			case 2: reg_required = 1; break;
			case 3: reg_required = 3; break;
			case 4: reg_required = 4; break;
		}
	} else {
		reg_required = 1;
	}
	end = id + reg_required - 1;

	if(end >= *p_count) {
		uint32_t original_count = *p_count;
		uint32_t addition = end - *p_count + 1;
		qb_enlarge_array((void **) p_regs, addition);
		qb_enlarge_array((void **) p_slots, addition);
	}
	reg = &(*p_regs)[id];
	reg->span = reg_required;
	return reg;
}

static void qb_propagate_pbj_matrix_span(qb_pbj_translator_context *cxt, qb_pbj_register *reg) {
	if(reg->span == 1) {
		qb_address *rgba_address = qb_create_address_alias(cxt->compiler_context, reg->matrix_address);
		rgba_address->dimension_count = 1;
		rgba_address->array_size_addresses = rgba_address->dimension_addresses = &rgba_address->array_size_address;
		reg->channel_addresses[PBJ_CHANNEL_RGBA] = rgba_address;
	} else {
		// copy vector addresses into this and subsequent registers
		uint32_t i;
		for(i = 0; i < reg->span; i++) {
			qb_address *index_address = qb_obtain_constant_U32(cxt->compiler_context, i);
			reg[i].channel_addresses[PBJ_CHANNEL_RGBA] = qb_obtain_array_element(cxt->compiler_context, reg->matrix_address, index_address, QB_ARRAY_BOUND_CHECK_NONE);
		}
	}
}

static qb_pbj_channel_id qb_get_pbj_channel_id(qb_pbj_translator_context *cxt, qb_pbj_address *reg_address) {
	uint32_t i;
	switch(reg_address->channel_count) {
		case 1: return reg_address->channels[0];
		case 2: {
			if(reg_address->channels[0] == PBJ_CHANNEL_R && reg_address->channels[1] == PBJ_CHANNEL_G) {
				return PBJ_CHANNEL_RG;
			} else if(reg_address->channels[0] == PBJ_CHANNEL_G && reg_address->channels[1] == PBJ_CHANNEL_B) {
				return PBJ_CHANNEL_GB;
			} else if(reg_address->channels[0] == PBJ_CHANNEL_B && reg_address->channels[1] == PBJ_CHANNEL_A) {
				return PBJ_CHANNEL_BA;
			}
		}	break;
		case 3: {
			if(reg_address->channels[0] == PBJ_CHANNEL_R && reg_address->channels[1] == PBJ_CHANNEL_G && reg_address->channels[2] == PBJ_CHANNEL_B) {
				return PBJ_CHANNEL_RGB;
			} else if(reg_address->channels[0] == PBJ_CHANNEL_G && reg_address->channels[1] == PBJ_CHANNEL_B && reg_address->channels[2] == PBJ_CHANNEL_A) {
				return PBJ_CHANNEL_GBA;
			}
		}	break;
		case 4: {
			if(reg_address->channels[0] == PBJ_CHANNEL_R && reg_address->channels[1] == PBJ_CHANNEL_G && reg_address->channels[2] == PBJ_CHANNEL_B && reg_address->channels[3] == PBJ_CHANNEL_A) {
				return PBJ_CHANNEL_RGBA;
			}
		}	break;
	}
	for(i = 1; i < reg_address->channel_count; i++) {
		if(reg_address->channels[i] != reg_address->channels[0]) {
			return PBJ_CHANNEL_NOT_CONTINUOUS;
		}
	}
	// the channels are all the same
	return reg_address->channels[0];
}

static qb_address * qb_create_pbj_constant(qb_compiler_context *cxt, qb_pbj_value *value) {
	qb_address *address = NULL;
	int32_t *int_values = NULL;
	float32_t *float_values;
	uint32_t dimensions[2];
	uint32_t dimension_count;
	uint32_t i, j;

	switch(value->type) {
		case PBJ_TYPE_BOOL:
		case PBJ_TYPE_INT: return qb_obtain_constant_S32(cxt, value->int1);
		case PBJ_TYPE_FLOAT: return qb_obtain_constant_F32(cxt, value->float1);
		case PBJ_TYPE_INT2: {
			dimensions[0] = 2;
			dimension_count = 1;
			int_values = value->int2;
		}	break;
		case PBJ_TYPE_INT3: {
			dimensions[0] = 3;
			dimension_count = 1;
			int_values = value->int3;
		}	break;
		case PBJ_TYPE_INT4: {
			dimensions[0] = 4;
			dimension_count = 1;
			int_values = value->int4;
		}	break;
		case PBJ_TYPE_FLOAT2: {
			dimensions[0] = 2;
			dimension_count = 1;
			float_values = value->float2;
		}	break;
		case PBJ_TYPE_FLOAT3: {
			dimensions[0] = 3;
			dimension_count = 1;
			float_values = value->float3;
		}	break;
		case PBJ_TYPE_FLOAT4: {
			dimensions[0] = 4;
			dimension_count = 1;
			float_values = value->float4;
		}	break;
		case PBJ_TYPE_FLOAT2X2: {
			dimensions[0] = 2;
			dimensions[1] = 2;
			dimension_count = 2;
			float_values = value->float2x2;
		}	break;
		case PBJ_TYPE_FLOAT3X3: {
			dimensions[0] = 3;
			dimensions[1] = 4;
			dimension_count = 2;
			float_values = value->float3x3;
		}	break;
		case PBJ_TYPE_FLOAT4X4: {
			dimensions[0] = 4;
			dimensions[1] = 4;
			dimension_count = 2;
			float_values = value->float4x4;
		}	break;
		default:
			qb_abort("invalid type id: %d", value->type);
	}

	if(int_values) {
		address = qb_create_constant_array(cxt, QB_TYPE_S32, dimensions, dimension_count);
		for(i = 0; i < dimensions[0]; i++) {
			ARRAY(S32, address)[i] = int_values[i];
		}
	} else if(float_values) {
		uint32_t value_count = (dimension_count == 1) ? dimensions[0] : dimensions[1] * dimensions[0];
		address = qb_create_constant_array(cxt, QB_TYPE_F32, dimensions, dimension_count);
		if(value_count == 9) {
			// 3x3 matrices are padded
			for(i = 0, j = 0; i < value_count; i++, j++) {
				ARRAY(F32, address)[j] = float_values[i];
				if(i == 2 || i == 5 || i == 8) {
					ARRAY(F32, address)[++j] = 0;
				}
			}
		} else {
			for(i = 0; i < value_count; i++) {
				ARRAY(F32, address)[i] = float_values[i];
			}
		}
	}
	return address;
}

static void qb_map_pbj_variables(qb_pbj_translator_context *cxt) {
	uint32_t i, dimension;
	qb_variable *qvar;

	// find the output image
	qvar = qb_find_output(cxt);
	if(!qvar) {
		qb_abort("a parameter must be passed by reference to the function to receive the result");
	} 
	cxt->output_image_address = qvar->address;
	cxt->output_image_width_address = cxt->output_image_address->dimension_addresses[1];
	cxt->output_image_height_address = cxt->output_image_address->dimension_addresses[0];
	cxt->output_image_channel_count_address = cxt->output_image_address->dimension_addresses[2];

	// variables for looping 
	cxt->x_address = qb_create_writable_scalar(cxt->compiler_context, QB_TYPE_U32);
	cxt->y_address = qb_create_writable_scalar(cxt->compiler_context, QB_TYPE_U32);

	// the current coordinate--basically x and y in float 
	dimension = 2;
	cxt->out_coord_address = qb_create_writable_array(cxt->compiler_context, QB_TYPE_F32, &dimension, 1);
	cxt->out_coord_x_address = qb_obtain_array_element(cxt->compiler_context, cxt->out_coord_address, cxt->compiler_context->zero_address, QB_ARRAY_BOUND_CHECK_NONE);
	cxt->out_coord_y_address = qb_obtain_array_element(cxt->compiler_context, cxt->out_coord_address, cxt->compiler_context->one_address, QB_ARRAY_BOUND_CHECK_NONE);

	// sub-array representing the pixel
	cxt->output_image_scanline_address = qb_obtain_array_element(cxt->compiler_context, cxt->output_image_address, cxt->y_address, QB_ARRAY_BOUND_CHECK_NONE);
	cxt->output_image_pixel_address = qb_obtain_array_element(cxt->compiler_context, cxt->output_image_scanline_address, cxt->x_address, QB_ARRAY_BOUND_CHECK_NONE);

	// a temporary array holding the pixel being worked on
	dimension = DIMENSION(cxt->output_image_address, -1);
	cxt->active_pixel_address = qb_create_writable_array(cxt->compiler_context, QB_TYPE_F32, &dimension, 1);

	// hook input images to the texture parameters
	for(i = 0; i < cxt->texture_count; i++) {
		qb_pbj_texture *texture = &cxt->textures[i];
		qvar = qb_find_argument(cxt, texture->name);
		if(qvar) {
			if(qb_is_image(cxt, qvar->address, texture->channel_count)) {
				texture->address = qvar->address;
			} else {
				qb_abort("parameter '%s' is not of the correct type", qvar->name);
			} 
		} else {
			// if there's only one input image, then use that one
			qvar = qb_find_input(cxt);
			if(qvar) {
				texture->address = qvar->address;
			} else {
				qb_abort("input image '%s' must be passed to the function", texture->name);
			}
		}
		// premultiplication is applied to input images with alpha channel
		if(DIMENSION(texture->address, -1) == 4) {
			qb_mark_as_writable(cxt->compiler_context, texture->address);
		}
	}

	for(i = 0; i < cxt->parameter_count; i++) {
		qb_pbj_parameter *parameter = &cxt->parameters[i];
		qb_pbj_register *reg = qb_create_pbj_register(cxt, &parameter->destination);

		if(parameter == cxt->out_pixel) {
			parameter->address = cxt->active_pixel_address;
		} else if(parameter == cxt->out_coord) {
			parameter->address = cxt->out_coord_address;
		} else {
			qvar = qb_find_argument(cxt, parameter->name);
			if(qvar) {
				if(!parameter->input_size_name) {
					uint32_t parameter_type = (parameter->destination.register_id & PBJ_REGISTER_INT) ? QB_TYPE_I32 : QB_TYPE_F32;
					if(qvar->address->type == parameter_type) {
						uint32_t parameter_size, element_count;
						if(parameter->destination.dimension > 1) {
							parameter_size = reg->span * 4;
						} else {
							parameter_size = parameter->destination.channel_count;
						}
						if(SCALAR(qvar->address)) {
							element_count = 1;
						} else {
							if(FIXED_LENGTH(qvar->address)) {
								element_count = ARRAY_SIZE(qvar->address);
							} else {
								element_count = 0;
							}
						}
						if(element_count == parameter_size) {
							parameter->address = qvar->address;
						} else {
							qb_abort("parameter '%s' is not of the correct size", qvar->name);
						}
					} else {
						qb_abort("parameter '%s' is not of the correct type", qvar->name);
					}
				} else {
					qb_abort("parameter '%s' is obtained from the input image and cannot be passed separately", parameter->name);
				}

			} else {
				// see if it has a default value
				if(parameter->default_value.type) {
					parameter->address = qb_create_pbj_constant(cxt->compiler_context, &parameter->default_value);
				} else {
					qb_abort("parameter '%s' does not have a default value and must be passed to the function", parameter->name);
				}
			}
		}

		// hook the address up to the register
		if(parameter->destination.dimension > 1) {
			reg->matrix_address = parameter->address;
			qb_propagate_pbj_matrix_span(cxt, reg);
		} else {
			qb_pbj_channel_id channel_id = qb_get_pbj_channel_id(cxt, &parameter->destination);
			if(channel_id != PBJ_CHANNEL_NOT_CONTINUOUS) {
				// should always be the case for parameters
				reg->channel_addresses[channel_id] = parameter->address;
			}
		}
	}
}

#define PBJ_RS				PBJ_READ_SOURCE
#define PBJ_WD				PBJ_WRITE_DESTINATION
#define PBJ_RI_RS_WD		(PBJ_READ_IMAGE | PBJ_READ_SOURCE | PBJ_WRITE_DESTINATION)
#define PBJ_RS_WD			(PBJ_READ_SOURCE | PBJ_WRITE_DESTINATION)
#define PBJ_RS_RD1_WD		(PBJ_READ_SOURCE | PBJ_READ_DESTINATION_FIRST | PBJ_WRITE_DESTINATION)
#define PBJ_RS_RD1_WDS		(PBJ_READ_SOURCE | PBJ_READ_DESTINATION | PBJ_WRITE_DESTINATION | PBJ_WRITE_SCALAR)
#define PBJ_RS_RD2_WD		(PBJ_READ_SOURCE | PBJ_READ_DESTINATION | PBJ_WRITE_DESTINATION)
#define PBJ_RS_RD1_WB		(PBJ_READ_SOURCE | PBJ_READ_DESTINATION | PBJ_WRITE_BOOL)
#define PBJ_RS_RS2_RS3_WD	(PBJ_READ_SOURCE | PBJ_READ_SOURCE2 | PBJ_READ_SOURCE3 | PBJ_WRITE_DESTINATION)
#define PBJ_RS_RD1_WD_WS	(PBJ_READ_SOURCE | PBJ_READ_DESTINATION_FIRST | PBJ_WRITE_DESTINATION | PBJ_WRITE_SOURCE)

static qb_pbj_translator pbj_op_translators[] = {
	{	NULL,										0,					NULL							},	// PBJ_NOP
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_add					},	// PBJ_ADD
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_subtract				},	// PBJ_SUBTRACT
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_multiply				},	// PBJ_MULTIPLY
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_reciprocal				},	// PBJ_RECIPROCAL
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_divide					},	// PBJ_DIVIDE
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_atan2					},	// PBJ_ATAN2
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_pow					},	// PBJ_POW
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_floor_modulo			},	// PBJ_MOD
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_min					},	// PBJ_MIN
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_max					},	// PBJ_MAX
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_step					},	// PBJ_STEP
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_sin					},	// PBJ_SIN
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_cos					},	// PBJ_COS
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_tan					},	// PBJ_TAN
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_asin					},	// PBJ_ASIN
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_acos					},	// PBJ_ACOS
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_atan					},	// PBJ_ATAN
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_exp					},	// PBJ_EXP
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_exp2					},	// PBJ_EXP2
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_log					},	// PBJ_LOG
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_log2					},	// PBJ_LOG2
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_sqrt					},	// PBJ_SQRT
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_rsqrt					},	// PBJ_RSQRT
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_abs					},	// PBJ_ABS
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_sign					},	// PBJ_SIGN
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_floor					},	// PBJ_FLOOR
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_ceil					},	// PBJ_CEIL
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_fract					},	// PBJ_FRACT
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_assign					},	// PBJ_COPY
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_assign					},	// PBJ_FLOAT_TO_INT
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_assign					},	// PBJ_INT_TO_FLOAT
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD2_WD,		&factory_mm_mult_cm				},	// PBJ_MATRIX_MATRIX_MULTIPLY
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_vm_mult_cm				},	// PBJ_VECTOR_MATRIX_MULTIPLY
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD2_WD,		&factory_mv_mult_cm				},	// PBJ_MATRIX_VECTOR_MULTIPLY
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_normalize				},	// PBJ_NORMALIZE
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_length					},	// PBJ_LENGTH
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WDS,		&factory_distance				},	// PBJ_DISTANCE
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WDS,		&factory_dot_product			},	// PBJ_DOT_PRODUCT
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_cross_product			},	// PBJ_CROSS_PRODUCT
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WB,		&factory_equal					},	// PBJ_EQUAL
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WB,		&factory_not_equal				},	// PBJ_NOT_EQUAL
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WB,		&factory_less_than				},	// PBJ_LESS_THAN
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WB,		&factory_less_equal				},	// PBJ_LESS_THAN_EQUAL
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_logical_not			},	// PBJ_LOGICAL_NOT
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_logical_and			},	// PBJ_LOGICAL_AND
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_logical_or				},	// PBJ_LOGICAL_OR
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD,		&factory_logical_xor			},	// PBJ_LOGICAL_XOR
	{	qbj_translate_pbj_basic_op,					PBJ_RI_RS_WD,		&factory_sample_nearest_vector	},	// PBJ_SAMPLE_NEAREST
	{	qbj_translate_pbj_basic_op,					PBJ_RI_RS_WD,		&factory_sample_bilinear_vector	},	// PBJ_SAMPLE_BILINEAR
	{	qb_translate_pbj_load_constant,				PBJ_WD,				&factory_assign,				},	// PBJ_LOAD_CONSTANT
	{	qb_translate_pbj_select,					PBJ_RS_RS2_RS3_WD,	NULL,							},	// PBJ_SELECT
	{	qb_translate_pbj_if,						PBJ_RS,				&factory_branch_on_false,		},	// PBJ_IF
	{	qb_translate_pbj_else,						0,					&factory_jump,					},	// PBJ_ELSE
	{	qb_translate_pbj_end_if,					0,					NULL							},	// PBJ_END_IF
	{	qbj_translate_pbj_basic_op,					0,					&factory_not_equal				},	// PBJ_FLOAT_TO_BOOL
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WD_WS,	&factory_assign					},	// PBJ_BOOL_TO_FLOAT
	{	qbj_translate_pbj_basic_op,					0,					&factory_not_equal				},	// PBJ_INT_TO_BOOL
	{	NULL,										PBJ_RS_RD1_WD_WS,	&factory_assign					},	// PBJ_BOOL_TO_INT
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WB,		&factory_set_equal				},	// PBJ_VECTOR_EQUAL
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RD1_WB,		&factory_set_not_equal			},	// PBJ_VECTOR_NOT_EQUAL
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_any					},	// PBJ_ANY
	{	qbj_translate_pbj_basic_op,					PBJ_RS_WD,			&factory_all					},	// PBJ_ALL
	{	qbj_translate_pbj_basic_op,					PBJ_RS_RS2_RS3_WD,	&factory_smooth_step			},	// PBJ_SMOOTH_STEP
};

static qb_pbj_register_slot * qb_get_pbj_register_slot(qb_pbj_translator_context *cxt, qb_pbj_address *reg_address) {
	if(reg_address->register_id & PBJ_REGISTER_INT) {
		return &cxt->int_register_slots[reg_address->register_id & ~PBJ_REGISTER_INT];
	} else {
		return &cxt->float_register_slots[reg_address->register_id];
	}
}

static qb_pbj_register * qb_get_pbj_register(qb_pbj_translator_context *cxt, qb_pbj_address *reg_address) {
	if(reg_address->register_id & PBJ_REGISTER_INT) {
		return &cxt->int_registers[reg_address->register_id & ~PBJ_REGISTER_INT];
	} else {
		return &cxt->float_registers[reg_address->register_id];
	}
}

static void qb_clear_pbj_register_slot(qb_pbj_translator_context *cxt, qb_pbj_register_slot *slot) {
	uint32_t i;
	slot->matrix.type = QB_OPERAND_EMPTY;
	slot->matrix.generic_pointer = NULL;
	for(i = 0; i <= PBJ_CHANNEL_RGBA; i++) {
		slot->channels[i].type = QB_OPERAND_EMPTY;
		slot->channels[i].generic_pointer = NULL;
	}
}

static void qb_discard_unused_register(qb_pbj_translator_context *cxt, qb_pbj_register *reg) {
	if(!reg->loaded) {
		if(reg->matrix_address) {
			qb_mark_as_temporary(cxt->compiler_context, reg->matrix_address);
		} else {
			qb_address *rgba_address = reg->channel_addresses[PBJ_CHANNEL_RGBA];
			qb_mark_as_temporary(cxt->compiler_context, rgba_address);
		}
	}
}

static void qb_invalidate_result_prototype(qb_pbj_translator_context *cxt, qb_operand *operand) {
	if(operand->type == QB_OPERAND_RESULT_PROTOTYPE) {
		operand->result_prototype->destination = NULL;

		// clear the temporary flag to indicate that the given address should be used
		operand->result_prototype->address_flags &= ~QB_ADDRESS_TEMPORARY;
		operand->type = QB_OPERAND_EMPTY;
		operand->generic_pointer = NULL;
	}
}

static void qb_load_pbj_matrix(qb_pbj_translator_context *cxt, qb_pbj_register *reg, qb_pbj_register_slot *slot) {
	uint32_t i, j;

	// invalidate all temporaries in the slot
	for(i = 0; i < reg->span; i++) {
		for(j = 0; j <= PBJ_CHANNEL_RGBA; j++) {
			qb_invalidate_result_prototype(cxt, &slot[i].channels[j]);
		}
	}

	reg->loaded = TRUE;
	slot->matrix.address = reg->matrix_address;
	slot->matrix.type = QB_OPERAND_ADDRESS;
}

static qb_pbj_channel_id * qb_get_pbj_channel_overlap(qb_pbj_translator_context *cxt, qb_pbj_channel_id channel_id, uint32_t *p_count) {
	qb_pbj_channel_id *overlapping_channels;
	uint32_t overlapping_channel_count;
	switch(channel_id) {
		case PBJ_CHANNEL_R: {
			static qb_pbj_channel_id R[] = { PBJ_CHANNEL_R, PBJ_CHANNEL_RG, PBJ_CHANNEL_RGB, PBJ_CHANNEL_RGBA };
			overlapping_channel_count = 4;
			overlapping_channels = R;
		}	break;
		case PBJ_CHANNEL_G: {
			static qb_pbj_channel_id G[] = { PBJ_CHANNEL_G, PBJ_CHANNEL_RG, PBJ_CHANNEL_GB, PBJ_CHANNEL_RGB, PBJ_CHANNEL_GBA, PBJ_CHANNEL_RGBA };
			overlapping_channel_count = 6;
			overlapping_channels = G;
		}	break;
		case PBJ_CHANNEL_B: {
			static qb_pbj_channel_id B[] = { PBJ_CHANNEL_B, PBJ_CHANNEL_GB, PBJ_CHANNEL_BA, PBJ_CHANNEL_RGB, PBJ_CHANNEL_GBA, PBJ_CHANNEL_RGBA };
			overlapping_channel_count = 6;
			overlapping_channels = B;
		}	break;
		case PBJ_CHANNEL_A: {
			static qb_pbj_channel_id A[] = { PBJ_CHANNEL_A, PBJ_CHANNEL_BA, PBJ_CHANNEL_GBA, PBJ_CHANNEL_RGBA };
			overlapping_channel_count = 4;
			overlapping_channels = A;
		}	break;
		case PBJ_CHANNEL_RG: {
			static qb_pbj_channel_id RG[] = { PBJ_CHANNEL_R, PBJ_CHANNEL_G, PBJ_CHANNEL_RG, PBJ_CHANNEL_GB, PBJ_CHANNEL_RGB, PBJ_CHANNEL_GBA, PBJ_CHANNEL_RGBA };
			overlapping_channel_count = 7;
			overlapping_channels = RG;
		}	break;
		case PBJ_CHANNEL_GB: {
			static qb_pbj_channel_id GB[] = { PBJ_CHANNEL_G, PBJ_CHANNEL_B, PBJ_CHANNEL_RG, PBJ_CHANNEL_GB, PBJ_CHANNEL_BA, PBJ_CHANNEL_RGB, PBJ_CHANNEL_GBA, PBJ_CHANNEL_RGBA };
			overlapping_channel_count = 8;
			overlapping_channels = GB;
		}	break;
		case PBJ_CHANNEL_BA: {
			static qb_pbj_channel_id BA[] = { PBJ_CHANNEL_B, PBJ_CHANNEL_A, PBJ_CHANNEL_GB, PBJ_CHANNEL_BA, PBJ_CHANNEL_RGB, PBJ_CHANNEL_GBA, PBJ_CHANNEL_RGBA };
			overlapping_channel_count = 7;
			overlapping_channels = BA;
		}	break;
		case PBJ_CHANNEL_RGB: {
			static qb_pbj_channel_id RGB[] = { PBJ_CHANNEL_R, PBJ_CHANNEL_G, PBJ_CHANNEL_B, PBJ_CHANNEL_RG, PBJ_CHANNEL_GB, PBJ_CHANNEL_BA, PBJ_CHANNEL_RGB, PBJ_CHANNEL_GBA, PBJ_CHANNEL_RGBA };
			overlapping_channel_count = 9;
			overlapping_channels = RGB;
		}	break;
		case PBJ_CHANNEL_GBA: {
			static qb_pbj_channel_id GBA[] = { PBJ_CHANNEL_G, PBJ_CHANNEL_B, PBJ_CHANNEL_A, PBJ_CHANNEL_RG, PBJ_CHANNEL_GB, PBJ_CHANNEL_BA, PBJ_CHANNEL_RGB, PBJ_CHANNEL_GBA, PBJ_CHANNEL_RGBA };
			overlapping_channel_count = 9;
			overlapping_channels = GBA;
		}	break;
		case PBJ_CHANNEL_RGBA: {
			static qb_pbj_channel_id RGBA[] = { PBJ_CHANNEL_R, PBJ_CHANNEL_G, PBJ_CHANNEL_B, PBJ_CHANNEL_A, PBJ_CHANNEL_RG, PBJ_CHANNEL_GB, PBJ_CHANNEL_BA, PBJ_CHANNEL_RGB, PBJ_CHANNEL_GBA, PBJ_CHANNEL_RGBA };
			overlapping_channel_count = 10;
			overlapping_channels = RGBA;
		}	break;
		default: {
			overlapping_channel_count = 0;
		}	break;
	}
	*p_count = overlapping_channel_count;
	return overlapping_channels;
}

static void qb_load_pbj_channel(qb_pbj_translator_context *cxt, qb_pbj_register *reg, qb_pbj_channel_id channel_id, qb_pbj_register_slot *slot) {
	uint32_t overlapping_channel_count, i;
	qb_pbj_channel_id *overlapping_channels = qb_get_pbj_channel_overlap(cxt, channel_id, &overlapping_channel_count);
	
	for(i = 0; i < overlapping_channel_count; i++) {
		qb_pbj_channel_id overlapping_channel = overlapping_channels[i];
		qb_invalidate_result_prototype(cxt, &slot->channels[overlapping_channel]);
	}
	qb_invalidate_result_prototype(cxt, &slot->matrix);

	reg->loaded = TRUE;
	slot->channels[channel_id].address = reg->channel_addresses[channel_id];
	slot->channels[channel_id].type = QB_OPERAND_ADDRESS;
}

enum {
	ALLOW_EMPTY		= 0x0001,
	IGNORE_VALUE	= 0x0002,
};

uint32_t qb_get_pbj_channel_mask(qb_pbj_translator_context *cxt, qb_pbj_address *reg_address) {
	uint32_t mask = 0;
	uint32_t i;
	for(i = 0; i < reg_address->channel_count; i++) {
		uint32_t channel = reg_address->channels[i];
		mask |= channel << (i * 3);
	}
	return mask;
}

static void qb_perform_gather(qb_pbj_translator_context *cxt, qb_address *src_address, qb_address *dst_address, uint32_t mask);
static void qb_perform_scatter(qb_pbj_translator_context *cxt, qb_address *src_address, qb_address *dst_address, uint32_t mask);

void qb_retrieve_operand(qb_pbj_translator_context *cxt, qb_pbj_address *reg_address, qb_operand *operand, uint32_t flags) {
	qb_pbj_register_slot *slot = qb_get_pbj_register_slot(cxt, reg_address);
	qb_pbj_register *reg = qb_get_pbj_register(cxt, reg_address);
	if(reg_address->dimension > 1) {
		if(slot->matrix.type == QB_OPERAND_EMPTY) {
			if(!(flags & ALLOW_EMPTY)) {
				// put the register into the slot
				qb_load_pbj_matrix(cxt, reg, slot);
			}
		}
		*operand = slot->matrix;
	} else {
		qb_pbj_channel_id channel_id = qb_get_pbj_channel_id(cxt, reg_address);
		if(channel_id != PBJ_CHANNEL_NOT_CONTINUOUS) {
			if(slot->channels[channel_id].type == QB_OPERAND_EMPTY) {
				if(!(flags & ALLOW_EMPTY)) {
					qb_load_pbj_channel(cxt, reg, channel_id, slot);
				}
			}
			*operand = slot->channels[channel_id];
		} else {
			if(!(flags & ALLOW_EMPTY)) {
				qb_pbj_register *reg = qb_get_pbj_register(cxt, reg_address);
				// TODO: load the channels
				qb_address *rgba_address = reg->channel_addresses[PBJ_CHANNEL_RGBA];
				if(cxt->compiler_context->stage == QB_STAGE_RESULT_TYPE_RESOLUTION) {
					// put the full address in there for now
					operand->address = rgba_address;
					operand->type = QB_OPERAND_ADDRESS;
				} else if(cxt->compiler_context->stage == QB_STAGE_OPCODE_TRANSLATION) {
					qb_variable_dimensions dim = { 1, qb_obtain_constant_U32(cxt->compiler_context, reg_address->channel_count) };
					qb_address *gather_address = qb_obtain_temporary_variable(cxt->compiler_context, rgba_address->type, &dim);
					uint32_t mask = qb_get_pbj_channel_mask(cxt, reg_address);

					qb_perform_gather(cxt, rgba_address, gather_address, mask);
					qb_lock_address(cxt->compiler_context, gather_address);
					operand->address = gather_address;
					operand->type = QB_OPERAND_ADDRESS;
				}
			} else {
				operand->type = QB_OPERAND_EMPTY;
				operand->generic_pointer = NULL;
			}
		}
	}
	if(operand->type == QB_OPERAND_RESULT_PROTOTYPE) {
		if(operand->result_prototype->destination) {
			// loading a value slated to be eliminated
			// clear the destination so the assignment occurs
			if(!(IGNORE_VALUE)) {				
				operand->result_prototype->destination = NULL;
			}
		}
	}
}

void qb_retire_operand(qb_pbj_translator_context *cxt, qb_pbj_address *reg_address, qb_operand *operand) {
	qb_pbj_register_slot *slot = qb_get_pbj_register_slot(cxt, reg_address);
	qb_operand *slot_operand = NULL;
	if(cxt->compiler_context->stage == QB_STAGE_RESULT_TYPE_RESOLUTION) {
		if(operand->type == QB_OPERAND_RESULT_PROTOTYPE) {
			if(reg_address->dimension > 1) {
				slot->matrix = *operand;
			} else {
				qb_pbj_register *reg = qb_get_pbj_register(cxt, reg_address);
				qb_pbj_channel_id channel_id = qb_get_pbj_channel_id(cxt, reg_address);
				if(channel_id != PBJ_CHANNEL_NOT_CONTINUOUS) {
					uint32_t overlapping_channel_count, i;
					qb_pbj_channel_id *overlapping_channels = qb_get_pbj_channel_overlap(cxt, channel_id, &overlapping_channel_count);

					// clear slots for overlapping addresses, so that access through them
					// would trigger qb_load_pbj_register(), which invalidate this result prototype
					// basically, it's a case where a temporary variable cannot substitute for the given address
					// because what is being modified is one part of a larger structure (e.g. one coordinate of a vector)
					for(i = 0; i < overlapping_channel_count; i++) {
						uint32_t overlapping_channel = overlapping_channels[i];
						slot->channels[overlapping_channel].type = QB_OPERAND_EMPTY;
						slot->channels[overlapping_channel].generic_pointer = NULL;
					}
					slot->matrix.type = QB_OPERAND_EMPTY;
					slot->matrix.generic_pointer = NULL;
					slot->channels[channel_id] = *operand;
				}
			}
		}
	} else if(cxt->compiler_context->stage == QB_STAGE_OPCODE_TRANSLATION) {
		if(reg_address->dimension > 1) {
			slot->matrix = *operand;
		} else {
			qb_pbj_channel_id channel_id = qb_get_pbj_channel_id(cxt, reg_address);
			if(channel_id != PBJ_CHANNEL_NOT_CONTINUOUS) {
				qb_unlock_operand(cxt->compiler_context, &slot->channels[channel_id]);
				slot->channels[channel_id] = *operand;
			} else {
				if(cxt->compiler_context->stage == QB_STAGE_OPCODE_TRANSLATION) {
					qb_pbj_register *reg = qb_get_pbj_register(cxt, reg_address);
					qb_address *rgba_address = reg->channel_addresses[PBJ_CHANNEL_RGBA];
					uint32_t mask = qb_get_pbj_channel_mask(cxt, reg_address);
					qb_perform_gather(cxt, operand->address, rgba_address, mask);
				}
			}
		}
	}
}

static void qb_lock_temporary_variables_in_register_slots(qb_pbj_translator_context *cxt, qb_pbj_register_slot *slots, uint32_t count) {
	uint32_t i, j;
	for(i = 0; i < count; i++) {
		qb_pbj_register_slot *slot = &slots[i];
		qb_lock_operand(cxt->compiler_context, &slot->matrix);
		for(j = PBJ_CHANNEL_R; j <= PBJ_CHANNEL_RGBA; j++) {
			qb_lock_operand(cxt->compiler_context, &slot->channels[j]);
		}
	}
}

static void qb_translate_current_pbj_instruction(qb_pbj_translator_context *cxt) {
	qb_pbj_op *pop = cxt->pbj_op;
	if(pop->opcode != PBJ_OP_DATA && pop->opcode != PBJ_NOP) {
		qb_pbj_translator *t = &pbj_op_translators[pop->opcode];
		if(t->translate) {
			qb_result_prototype *result_prototype = &cxt->result_prototypes[cxt->pbj_op_index];
			qb_operand operands[4] = { { QB_OPERAND_NONE, NULL }, { QB_OPERAND_NONE, NULL }, { QB_OPERAND_NONE, NULL }, { QB_OPERAND_NONE, NULL } }; 
			qb_operand result = { QB_OPERAND_NONE, NULL };
			qb_pbj_address scalar_destination;
			uint32_t operand_count = 0;

			if(t->flags & PBJ_READ_IMAGE) {
				qb_pbj_texture *texture = qb_find_pbj_texture_by_id(cxt, pop->image_id);
				qb_operand *image = &operands[operand_count++];
				image->address = texture->address;
				image->type = QB_OPERAND_ADDRESS;
			}
			if(t->flags & PBJ_READ_DESTINATION_FIRST) {
				uint32_t flags = 0;
				if(t->extra == &factory_assign) {
					flags = IGNORE_VALUE;
					if(cxt->compiler_context->stage == QB_STAGE_RESULT_TYPE_RESOLUTION) {
						flags |= ALLOW_EMPTY;
					} else if(cxt->compiler_context->stage == QB_STAGE_OPCODE_TRANSLATION) {
						if(result_prototype->address_flags & QB_ADDRESS_TEMPORARY) {
							flags |= ALLOW_EMPTY;
						}
					}
				}
				qb_retrieve_operand(cxt, &pop->destination, &operands[operand_count++], flags);
			}
			if(t->flags & PBJ_READ_SOURCE) {
				qb_retrieve_operand(cxt, &pop->source, &operands[operand_count++], 0);
			} else if(pop->opcode == PBJ_LOAD_CONSTANT) {
				qb_operand *constant = &operands[operand_count++];
				constant->pbj_constant = &pop->constant;
				constant->type = QB_OPERAND_PBJ_CONSTANT;
			}
			if(t->flags & PBJ_READ_SOURCE2) {
				qb_pbj_op *data_pop = pop + 1;
				qb_retrieve_operand(cxt, &data_pop->source2, &operands[operand_count++], 0);
				if(t->flags & PBJ_READ_SOURCE3) {
					qb_retrieve_operand(cxt, &data_pop->source3, &operands[operand_count++], 0);
				}
			}
			if(t->flags & PBJ_READ_DESTINATION) {
				qb_retrieve_operand(cxt, &pop->destination, &operands[operand_count++], 0);
			}
			if(t->flags & PBJ_WRITE_DESTINATION) {
				uint32_t flags;
				if(cxt->compiler_context->stage == QB_STAGE_RESULT_TYPE_RESOLUTION) {
					flags |= ALLOW_EMPTY;
				} else if(cxt->compiler_context->stage == QB_STAGE_OPCODE_TRANSLATION) {
					if(result_prototype->address_flags & QB_ADDRESS_TEMPORARY) {
						flags |= ALLOW_EMPTY;
					}
				}
				if(t->flags & PBJ_WRITE_SCALAR) {
					// the op writes to the first channel of the destination only (e.g. dot product)
					scalar_destination = pop->destination;
					scalar_destination.channel_count = 1;
					qb_retrieve_operand(cxt, &scalar_destination, &result, flags);
				} else {
					qb_retrieve_operand(cxt, &pop->destination, &result, flags);
				}
				if(result.type == QB_OPERAND_ADDRESS) {
					if(CONSTANT(result.address)) {
						result.type = QB_OPERAND_EMPTY;
						result.generic_pointer = NULL;
					}
				}
			} else if(t->flags & PBJ_WRITE_BOOL) {
				cxt->comparison_result.channel_count = pop->source.channel_count;
				qb_retrieve_operand(cxt, &cxt->comparison_result, &result, ALLOW_EMPTY);
			}

			t->translate(cxt, t, operands, operand_count, &result, result_prototype);

			if(t->flags & PBJ_WRITE_DESTINATION) {
				if(t->flags & PBJ_WRITE_SCALAR) {
					qb_retire_operand(cxt, &scalar_destination, &result);
				} else {
					qb_retire_operand(cxt, &pop->destination, &result);
				}
			} else if(t->flags & PBJ_WRITE_BOOL) {
				qb_retire_operand(cxt, &cxt->comparison_result, &result);
			}

			qb_lock_temporary_variables_in_register_slots(cxt, cxt->int_register_slots, cxt->int_register_slot_count);
			qb_lock_temporary_variables_in_register_slots(cxt, cxt->float_register_slots, cxt->float_register_slot_count);
		}
	}
}

static void qb_allocate_pbj_register(qb_pbj_translator_context *cxt, qb_pbj_address *reg_address) {
	qb_pbj_register *reg = qb_create_pbj_register(cxt, reg_address);
	qb_primitive_type element_type = (reg_address->register_id & PBJ_REGISTER_INT) ? QB_TYPE_I32 : QB_TYPE_F32;
	if(reg_address->dimension > 1) {
		if(!reg->matrix_address) {
			uint32_t size = reg->span * 4;
			uint32_t dimensions[2] = { reg_address->dimension, size / reg_address->dimension };
			reg->matrix_address = qb_create_writable_array(cxt->compiler_context, element_type, dimensions, 2);
			qb_propagate_pbj_matrix_span(cxt, reg);
		}
	} else {
		qb_address *rgba_address = reg->channel_addresses[PBJ_CHANNEL_RGBA];
		qb_pbj_channel_id channel_id = qb_get_pbj_channel_id(cxt, reg_address);
		if(!rgba_address) {
			uint32_t size = 4;
			rgba_address = qb_create_writable_array(cxt->compiler_context, element_type, &size, 1);
			reg->channel_addresses[PBJ_CHANNEL_RGBA] = rgba_address;
		}
		if(channel_id != PBJ_CHANNEL_NOT_CONTINUOUS) {
			if(!reg->channel_addresses[channel_id]) {
				qb_address *channel_address;
				uint32_t channel = reg_address->channels[0];
				qb_address *index_address = qb_obtain_constant_U32(cxt->compiler_context, channel);
				if(channel_id <= PBJ_CHANNEL_A) {
					channel_address = qb_obtain_array_element(cxt->compiler_context, rgba_address, index_address, QB_ARRAY_BOUND_CHECK_NONE);
				} else {
					qb_address *length_address = qb_obtain_constant_U32(cxt->compiler_context, reg_address->channel_count);
					channel_address = qb_obtain_array_slice(cxt->compiler_context, rgba_address, index_address, length_address, QB_ARRAY_BOUND_CHECK_NONE);
				}
				reg->channel_addresses[channel_id] = channel_address;
			}
		}
	}
}

static void qb_allocate_pbj_registers(qb_pbj_translator_context *cxt) {
	uint32_t i;
	qb_pbj_register *reg;

	// allocate matrices first, since they can spill into multiple registers
	for(i = 0; i < cxt->pbj_op_count; i++) {
		qb_pbj_op *pop = &cxt->pbj_ops[i];
		if(pop->flags & PBJ_SOURCE_IN_USE) {
			if(pop->source.dimension > 1) {
				qb_allocate_pbj_register(cxt, &pop->source);
			}
		}
		if(pop->flags & PBJ_DESTINATION_IN_USE) {
			if(pop->destination.dimension > 1) {
				qb_allocate_pbj_register(cxt, &pop->destination);
			}
		}
	}

	// allocate vectors and scalars
	for(i = 0; i < cxt->pbj_op_count; i++) {
		qb_pbj_op *pop = &cxt->pbj_ops[i];
		if(pop->flags & PBJ_SOURCE_IN_USE) {
			if(pop->source.dimension == 1) {
				qb_allocate_pbj_register(cxt, &pop->source);
			}
		}
		if(pop->flags & PBJ_DESTINATION_IN_USE) {
			if(pop->destination.dimension == 1) {
				qb_allocate_pbj_register(cxt, &pop->destination);
			}
		}
	}

	// allocate return register used by comparison operators
	cxt->comparison_result.dimension = 1;
	cxt->comparison_result.channels[0] = 0;
	cxt->comparison_result.channels[1] = 1;
	cxt->comparison_result.channels[2] = 2;
	cxt->comparison_result.channels[3] = 3;
	cxt->comparison_result.register_id = 0 | PBJ_REGISTER_INT;
	for(i = 1; i <= 4; i++) {
		cxt->comparison_result.channel_count = i;
		qb_allocate_pbj_register(cxt, &cxt->comparison_result);
	}
	reg = qb_get_pbj_register(cxt, &cxt->comparison_result);
	for(i = PBJ_CHANNEL_R; i <= PBJ_CHANNEL_RGBA; i++) {
		if(reg->channel_addresses[i]) {
			qb_mark_as_temporary(cxt->compiler_context, reg->channel_addresses[i]);
		}
	}
	reg = reg + 1;
	for(i = PBJ_CHANNEL_R; i <= PBJ_CHANNEL_RGBA; i++) {
		if(reg->channel_addresses[i]) {
			qb_mark_as_temporary(cxt->compiler_context, reg->channel_addresses[i]);
		}
	}

	// mark addresses destination addresses as writable unless it's load-constant
	for(i = 0; i < cxt->pbj_op_count; i++) {
		qb_pbj_op *pop = &cxt->pbj_ops[i];
		if(pop->opcode != PBJ_LOAD_CONSTANT && pop->opcode != PBJ_NOP && pop->opcode != PBJ_OP_DATA) {
			qb_pbj_translator *t = &pbj_op_translators[pop->opcode];
			if(t->flags & PBJ_WRITE_DESTINATION) {
				qb_pbj_register *reg = qb_get_pbj_register(cxt, &pop->destination);
				qb_address *dst_address;
				if(pop->destination.dimension > 1) {
					dst_address = reg->matrix_address;
				} else {
					qb_pbj_channel_id channel_id = qb_get_pbj_channel_id(cxt, &pop->destination);
					dst_address = reg->channel_addresses[channel_id];
				}
				qb_mark_as_writable(cxt->compiler_context, dst_address);
			}
		}
	}
}

void qb_survey_pbj_instructions(qb_pbj_translator_context *cxt) {
	uint32_t i;

	// look for for sequences of ops representing smoothstep()
	qb_substitute_ops(cxt);

	// map function arguments to PB kernel parameters
	qb_map_pbj_variables(cxt);

	// allocate registers
	qb_allocate_pbj_registers(cxt);

	// clear the register slots
	for(i = 0; i < cxt->int_register_slot_count; i++) {
		qb_clear_pbj_register_slot(cxt, &cxt->int_register_slots[i]);
	}
	for(i = 0; i < cxt->float_register_slot_count; i++) {
		qb_clear_pbj_register_slot(cxt, &cxt->float_register_slots[i]);
	}

	qb_enlarge_array((void **) &cxt->result_prototypes, cxt->pbj_op_count);
	for(i = 0; i < cxt->result_prototype_count; i++) {
		qb_result_prototype *prototype = &cxt->result_prototypes[i];
		prototype->preliminary_type = prototype->final_type = QB_TYPE_UNKNOWN;
	}

	// shift the translation table by two so there's a place to put the inner and outer loop jump targets
	cxt->pbj_op_offset = 2;
	cxt->compiler_context->op_translations = qb_allocate_indices(cxt->pool, cxt->pbj_op_offset + cxt->pbj_op_count + 1); 
	memset(cxt->compiler_context->op_translations, 0xFF, (cxt->pbj_op_offset + cxt->pbj_op_count + 1) * sizeof(uint32_t));

	cxt->compiler_context->stage = QB_STAGE_RESULT_TYPE_RESOLUTION;
	for(cxt->pbj_op_index = 0; cxt->pbj_op_index < cxt->pbj_op_count; cxt->pbj_op_index++) {
		cxt->pbj_op = &cxt->pbj_ops[cxt->pbj_op_index];
		qb_translate_current_pbj_instruction(cxt);
	}

	for(i = 0; i < cxt->int_register_slot_count; i++) {
		qb_discard_unused_register(cxt, &cxt->int_registers[i]);
	}
	for(i = 0; i < cxt->float_register_slot_count; i++) {
		qb_discard_unused_register(cxt, &cxt->float_registers[i]);
	}
}

static void qb_set_jump_target(qb_pbj_translator_context *cxt) {
	if(cxt->jump_target_index != (uint32_t) -1) {
		qb_op *qop = cxt->compiler_context->ops[cxt->jump_target_index];
		qop->flags |= QB_OP_JUMP_TARGET; 
		cxt->jump_target_index = (uint32_t) -1;
	}
}

static void qb_perform_assignment(qb_pbj_translator_context *cxt, qb_address *dst_address, qb_address *src_address) {
	qb_retrieve_binary_op_result(cxt->compiler_context, &factory_assign, dst_address, src_address);
	qb_set_jump_target(cxt);
}

static void qb_perform_subtraction(qb_pbj_translator_context *cxt, qb_address *dst_address, qb_address *src_address) {
	qb_retrieve_binary_op_result(cxt->compiler_context, factories_subtract_assign[0], dst_address, src_address);
	qb_set_jump_target(cxt);
}

static void qb_perform_increment(qb_pbj_translator_context *cxt, qb_address *dst_address) {
	qb_retrieve_unary_op_result(cxt->compiler_context, &factory_increment_pre, dst_address);
	qb_set_jump_target(cxt);
}

static void qb_perform_alpha_premultication(qb_pbj_translator_context *cxt, qb_address *dst_address) {
	qb_operand operand = { QB_OPERAND_ADDRESS, dst_address };
	qb_operand result = { QB_OPERAND_ADDRESS, dst_address };
	qb_create_op(cxt->compiler_context, &factory_apply_premult, QB_TYPE_F32, &operand, 1, &result, NULL, 0, TRUE);
	qb_set_jump_target(cxt);
}

static void qb_perform_alpha_premultiplication_removal(qb_pbj_translator_context *cxt, qb_address *dst_address) {
	qb_operand operand = { QB_OPERAND_ADDRESS, dst_address };
	qb_operand result = { QB_OPERAND_ADDRESS, dst_address };
	qb_create_op(cxt->compiler_context, &factory_remove_premult, QB_TYPE_F32, &operand, 1, &result, NULL, 0, TRUE);
	qb_set_jump_target(cxt);
}

static void qb_perform_gather(qb_pbj_translator_context *cxt, qb_address *src_address, qb_address *dst_address, uint32_t mask) {
	qb_operand operands[3];
	qb_operand result = { QB_OPERAND_EMPTY, NULL };
	operands[0].address = dst_address;
	operands[0].type = QB_OPERAND_ADDRESS;
	operands[1].address = src_address;
	operands[1].type = QB_OPERAND_ADDRESS;
	operands[2].number = mask;
	operands[2].type = QB_OPERAND_NUMBER;
	qb_produce_op(cxt->compiler_context, &factory_gather, operands, 3, &result, NULL, 0, NULL);
	qb_set_jump_target(cxt);
}

static void qb_perform_scatter(qb_pbj_translator_context *cxt, qb_address *src_address, qb_address *dst_address, uint32_t mask) {
	qb_operand operands[3];
	qb_operand result = { QB_OPERAND_EMPTY, NULL };
	operands[0].address = dst_address;
	operands[0].type = QB_OPERAND_ADDRESS;
	operands[1].address = src_address;
	operands[1].type = QB_OPERAND_ADDRESS;
	operands[2].number = mask;
	operands[2].type = QB_OPERAND_NUMBER;
	qb_produce_op(cxt->compiler_context, &factory_scatter, operands, 3, &result, NULL, 0, NULL);
	qb_set_jump_target(cxt);
}

static void qb_perform_return(qb_pbj_translator_context *cxt) {
	qb_operand operand = { QB_OPERAND_NONE, NULL };
	qb_operand result = { QB_OPERAND_EMPTY, NULL };
	qb_produce_op(cxt->compiler_context, &factory_return, &operand, 1, &result, NULL, 0, NULL);
	qb_set_jump_target(cxt);
}

static void qb_perform_loop(qb_pbj_translator_context *cxt, qb_address *index_address, qb_address *limit_address, uint32_t jump_target_index) {
	qb_operand operands[2] = { { QB_OPERAND_ADDRESS, index_address }, { QB_OPERAND_ADDRESS, limit_address } };
	qb_operand result = { QB_OPERAND_EMPTY, NULL };
	uint32_t jump_target_indices[2] = { jump_target_index, QB_INSTRUCTION_NEXT };
	qb_produce_op(cxt->compiler_context, &factory_loop, operands, 2, &result, jump_target_indices, 2, NULL);
	qb_set_jump_target(cxt);
}

static void qb_perform_branch(qb_pbj_translator_context *cxt, qb_address *condition_address, uint32_t jump_target_index) {
	qb_operand operand = { QB_OPERAND_ADDRESS, condition_address };
	qb_operand result = { QB_OPERAND_NONE, NULL };
	uint32_t jump_target_indices[2] = { jump_target_index, QB_INSTRUCTION_NEXT };
	qb_produce_op(cxt->compiler_context, &factory_branch_on_true, &operand, 1, &result, jump_target_indices, 2, NULL);
	qb_set_jump_target(cxt);
}

static void qb_perform_jump(qb_pbj_translator_context *cxt, uint32_t jump_target_index) {
	qb_operand result = { QB_OPERAND_NONE, NULL };
	qb_produce_op(cxt->compiler_context, &factory_jump, NULL, 0, &result, &jump_target_index, 1, NULL);
	qb_set_jump_target(cxt);
}

static void qb_perform_nop(qb_pbj_translator_context *cxt) {
	qb_create_op(cxt->compiler_context, &factory_nop, QB_TYPE_VOID, NULL, 0, NULL, NULL, 0, FALSE);
	qb_set_jump_target(cxt);
}

static void qb_mark_next_op_as_jump_target(qb_pbj_translator_context *cxt, uint32_t src_index) {
	if(!(src_index & QB_INSTRUCTION_OFFSET)) {
		cxt->compiler_context->op_translations[src_index] = cxt->compiler_context->op_count;
	}
	cxt->jump_target_index = cxt->compiler_context->op_count;
}

void qb_translate_pbj_instructions(qb_pbj_translator_context *cxt) {
	qb_address *start_coord_address;
	qb_operand operand;
	uint32_t i;

	// clear the register slots again
	for(i = 0; i < cxt->int_register_slot_count; i++) {
		qb_clear_pbj_register_slot(cxt, &cxt->int_register_slots[i]);
	}
	for(i = 0; i < cxt->float_register_slot_count; i++) {
		qb_clear_pbj_register_slot(cxt, &cxt->float_register_slots[i]);
	}

	// interpret 3x4 matrix as 3x3 with padded element
	cxt->compiler_context->matrix_padding = TRUE;

	cxt->compiler_context->stage = QB_STAGE_OPCODE_TRANSLATION;

	// apply premultiplication to input images
	for(i = 0; i < cxt->texture_count; i++) {
		qb_pbj_texture *texture = &cxt->textures[i];
		if(DIMENSION(texture->address, -1) == 4) {
			qb_perform_alpha_premultication(cxt, texture->address);
		}
	}

#define OUTER_LOOK_JUMP_TARGET_INDEX		0
#define INNER_LOOP_JUMP_TARGET_INDEX		1

	// set y to zero, but _OutCoord.y to 0.5, as that's the center of the grid
	start_coord_address = qb_obtain_constant_F32(cxt->compiler_context, 0.5);
	qb_perform_assignment(cxt, cxt->y_address, cxt->compiler_context->zero_address);
	qb_perform_assignment(cxt, cxt->out_coord_y_address, start_coord_address);

	// set x to zero, but _OutCoord.x to 0.5
	// the outer loop starts here
	qb_mark_next_op_as_jump_target(cxt, OUTER_LOOK_JUMP_TARGET_INDEX);
	qb_perform_assignment(cxt, cxt->x_address, cxt->compiler_context->zero_address);
	qb_perform_assignment(cxt, cxt->out_coord_x_address, start_coord_address);

	// initialize the active pixel to zero--the inner loop starts here
	qb_mark_next_op_as_jump_target(cxt, INNER_LOOP_JUMP_TARGET_INDEX);
	qb_perform_subtraction(cxt, cxt->active_pixel_address, cxt->active_pixel_address);

	// translate the instructions
	cxt->compiler_context->stage = QB_STAGE_OPCODE_TRANSLATION;
	for(cxt->pbj_op_index = 0; cxt->pbj_op_index < cxt->pbj_op_count; cxt->pbj_op_index++) {
		uint32_t current_op_count = cxt->compiler_context->op_count;

		cxt->pbj_op = &cxt->pbj_ops[cxt->pbj_op_index];
		qb_translate_current_pbj_instruction(cxt);

		// add a nop if new one wasn't generated
		if(current_op_count == cxt->compiler_context->op_count) {
			qb_perform_nop(cxt);
		}

		// flag new op as a jump target if there's a placeholder in the position
		if(cxt->compiler_context->op_translations[cxt->pbj_op_offset + cxt->pbj_op_index] == QB_OP_INDEX_JUMP_TARGET) {
			qb_op *first_op = cxt->compiler_context->ops[current_op_count];
			first_op->flags |= QB_OP_JUMP_TARGET;
		}
		cxt->compiler_context->op_translations[cxt->pbj_op_offset + cxt->pbj_op_index] = current_op_count;
	}

	// mark the previous instruction as a jump target in case there's an jump to the end of the PB program
	qb_mark_next_op_as_jump_target(cxt, cxt->pbj_op_offset + cxt->pbj_op_count);

	// copy the output pixel into the image
	qb_retrieve_operand(cxt, &cxt->out_pixel->destination, &operand, 0);
	qb_perform_assignment(cxt, cxt->output_image_pixel_address, operand.address);

	// increment _OutCoord.x
	qb_perform_increment(cxt, cxt->out_coord_x_address);

	// jump to beginning of inner loop if x is less than width
	qb_perform_loop(cxt, cxt->x_address, cxt->output_image_width_address, INNER_LOOP_JUMP_TARGET_INDEX);

	// increment _OutCoord.y
	qb_perform_increment(cxt, cxt->out_coord_y_address);

	// jump to beginning of outer loop if y is less than height
	qb_perform_loop(cxt, cxt->y_address, cxt->output_image_height_address, OUTER_LOOK_JUMP_TARGET_INDEX);

	// remove premultiplication from output image if it has an alpha channel
	if(DIMENSION(cxt->output_image_address, -1) == 4) {
		qb_perform_alpha_premultiplication_removal(cxt, cxt->output_image_address);
	}

	qb_perform_return(cxt);
}


void qb_initialize_pbj_translator_context(qb_pbj_translator_context *cxt, qb_compiler_context *compiler_cxt TSRMLS_DC) {
	memset(cxt, 0, sizeof(qb_pbj_translator_context));
	cxt->compiler_context = compiler_cxt;
	cxt->pool = compiler_cxt->pool;
	cxt->storage = compiler_cxt->storage;

	qb_attach_new_array(cxt->pool, (void **) &cxt->conditionals, &cxt->conditional_count, sizeof(qb_pbj_op *), 8);
	qb_attach_new_array(cxt->pool, (void **) &cxt->parameters, &cxt->parameter_count, sizeof(qb_pbj_parameter), 8);
	qb_attach_new_array(cxt->pool, (void **) &cxt->textures, &cxt->texture_count, sizeof(qb_pbj_texture), 4);

	qb_attach_new_array(cxt->pool, (void **) &cxt->int_registers, &cxt->int_register_count, sizeof(qb_pbj_register), 16);
	qb_attach_new_array(cxt->pool, (void **) &cxt->int_register_slots, &cxt->int_register_slot_count, sizeof(qb_pbj_register_slot), 16);
	qb_attach_new_array(cxt->pool, (void **) &cxt->float_registers, &cxt->float_register_count, sizeof(qb_pbj_register), 16);
	qb_attach_new_array(cxt->pool, (void **) &cxt->float_register_slots, &cxt->float_register_slot_count, sizeof(qb_pbj_register_slot), 16);

	qb_attach_new_array(cxt->pool, (void **) &cxt->result_prototypes, &cxt->result_prototype_count, sizeof(qb_result_prototype), 16);

	SAVE_TSRMLS
}

void qb_free_pbj_translator_context(qb_pbj_translator_context *cxt) {
}


