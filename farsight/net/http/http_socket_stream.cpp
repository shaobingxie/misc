/*
 * File:    http_socket_stream.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   http socket stream
 *
 * Copyright (c) 2009  Li XianJing <xianjimli@hotmail.com>
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
 * 2009-05-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include "http_socket_stream.h"

CHttpSocketStream::CHttpSocketStream(int socket)
{
	m_socket = socket;

	return;
}

CHttpSocketStream::~CHttpSocketStream(void)
{
	close(m_socket);

	return;
}

int CHttpSocketStream::Read(char* buffer, size_t length)
{
	return recv(m_socket, buffer, length, 0);
}

int CHttpSocketStream::Write(const char* buffer, size_t length)
{
	return send(m_socket, buffer, length, 0);
}

