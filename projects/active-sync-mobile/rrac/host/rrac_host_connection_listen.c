/*
 * File:    rrac_host_connection_listen.c
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

#include <asm-server-socket.h>
#include <asm-input-buffer.h>
#include <asm-output-buffer.h>
#include <rrac_host_lib.h>
#include <rrac_host_connection_listen.h>
#include <rrac_host_connection_cmd.h>
#include <rrac_host_connection_data.h>

typedef struct _PrivInfo
{
	AsmServer* server;

	AsmHookFunc on_destroy;
	void* on_destroy_ctx;

	RapiStartReplicationFunc start_replication;
	void* start_replication_ctx;
}PrivInfo;

static int     rrac_host_connection_listen_get_fd(AsmConnection* thiz)
{
	asm_return_val_if_fail(thiz != NULL, -1);
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return asm_server_get_fd(priv->server);
}

static AsmRet rrac_host_connection_listen_process_event(AsmConnection* thiz)
{
	AsmRet ret = ASM_RET_FAIL;
	asm_return_val_if_fail(thiz != NULL, ASM_RET_FAIL);

	return ret;
}

static AsmRet rrac_host_connection_listen_destroy(AsmConnection* thiz)
{
	asm_return_val_if_fail(thiz != NULL, ASM_RET_FAIL);
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	if(priv->on_destroy != NULL)
	{
		priv->on_destroy(priv->on_destroy_ctx, thiz);
	}

	asm_server_destroy(priv->server);
	ASM_FREE(thiz);

	return ASM_RET_OK;
}

AsmRet rrac_host_connection_listen_hook_destroy(AsmConnection* thiz, AsmHookFunc on_destroy, void* on_destroy_ctx)
{
	asm_return_val_if_fail(thiz != NULL, ASM_RET_FAIL);
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	
	priv->on_destroy = on_destroy;
	priv->on_destroy_ctx = on_destroy_ctx;

	return ASM_RET_OK;
}

AsmConnection* rrac_host_connection_listen_create(RapiStartReplicationFunc start_replication, void* ctx)
{
	AsmServer* server = asm_server_socket_create(NULL, RRAC_PORT);

	asm_return_val_if_fail(server != NULL, NULL);

	AsmConnection* thiz = ASM_ALLOC(AsmConnection, sizeof(PrivInfo));

	PrivInfo* priv = (PrivInfo*)thiz->priv;
	priv->server = server;
	priv->start_replication = start_replication;
	priv->start_replication_ctx = ctx;

	thiz->get_fd        = rrac_host_connection_listen_get_fd;
	thiz->process_event = rrac_host_connection_listen_process_event;
	thiz->hook_destroy  = rrac_host_connection_listen_hook_destroy;
	thiz->destroy       = rrac_host_connection_listen_destroy;

	return thiz;
}

AsmRet         rrac_host_connection_request_sync(AsmConnection* thiz, AsmConnection** cmd, AsmConnection** data)
{
	AsmRet ret = ASM_RET_FAIL;
	asm_return_val_if_fail(thiz != NULL && cmd != NULL && data != NULL, ASM_RET_FAIL);
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	if(priv->start_replication != NULL)
	{
		ret = priv->start_replication(priv->start_replication_ctx);
	}
	else
	{
		ret = ASM_RET_OK;
	}
	asm_return_val_if_fail(ret == ASM_RET_OK, ret);

	AsmStream* stream = NULL;
	if(asm_server_accept(priv->server, &stream) == ASM_RET_OK)
	{
		*cmd = rrac_host_connection_cmd_create(stream);
	
		if(asm_server_accept(priv->server, &stream) == ASM_RET_OK)
		{
			*data = rrac_host_connection_data_create(stream);

			ret = ASM_RET_OK;
		}
	}

	printf("%s:%d cmd=%p data=%p ret=%d\n", __func__, __LINE__, cmd, data, ret);

	return ret;
}


