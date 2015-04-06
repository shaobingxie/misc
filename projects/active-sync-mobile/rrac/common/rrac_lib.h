/*
 * File:    rrac_lib.h
 * Author:  Li XianJing <lixianjing@broncho.org>
 * Brief:   
 *
 * Copyright (c) 2008 topwise, Inc.
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2008-01-28 Li XianJing <lixianjing@topwisesz.com> created
 *
 */

#ifndef _RRAC_LIB_H
#define _RRAC_LIB_H

#include <asm-common.h>
#include <asm-stream.h>
#include <asm-input-buffer.h>
#include <asm-output-buffer.h>

#include <rrac_macros.h>
#include <rrac_structs.h>
#include <rrac_enums.h>

AsmRet rrac_read_command(AsmStream* stream, RracCommandHeader** command);
AsmRet rrac_write_command(AsmStream* stream, const RracCommandHeader* command);

AsmRet rrac_read_object(AsmStream* stream,  RracObject** object);
AsmRet rrac_write_object(AsmStream* stream, const RracObject* object);

#endif/*_RRAC_LIB_H*/