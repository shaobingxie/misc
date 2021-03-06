/*
 * File: ftk_widget.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   interface for all widget.
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
 * 2009-10-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef FTK_WIDGET_H
#define FTK_WIDGET_H

#include "ftk_key.h"
#include "ftk_event.h"
#include "ftk_canvas.h"
#include "ftk_display.h"

struct _FtkWdiget;
typedef struct _FtkWdiget FtkWidget;

struct _FtkWidgetInfo;
typedef struct _FtkWidgetInfo FtkWidgetInfo;

typedef Ret  (*FtkWidgetOnEvent)(FtkWidget* thiz, FtkEvent* event);
typedef Ret  (*FtkWidgetOnPaint)(FtkWidget* thiz);
typedef void (*FtkWidgetDestroy)(FtkWidget* thiz);

typedef enum _FtkWidgetState
{
	FTK_WIDGET_NORMAL = 0,
	FTK_WIDGET_FOCUSED,
	FTK_WIDGET_INSENSITIVE,
	FTK_WIDGET_STATE_NR
}FtkWidgetState;

struct _FtkWdiget
{
	int ref;
	FtkWidgetOnEvent on_event;
	FtkWidgetOnPaint on_paint;
	FtkWidgetDestroy destroy;
	
	FtkWidget* prev;
	FtkWidget* next;
	FtkWidget* parent;
	FtkWidget* children;
	FtkWidgetInfo* priv;
	/*private data for subclass*/
	void* priv_subclass[5];
};

void ftk_widget_init(FtkWidget* thiz, int type, int id);
int ftk_widget_type(FtkWidget* thiz);
int ftk_widget_top(FtkWidget* thiz);
int ftk_widget_left(FtkWidget* thiz);
int ftk_widget_top_abs(FtkWidget* thiz);
int ftk_widget_left_abs(FtkWidget* thiz);
int ftk_widget_width(FtkWidget* thiz);
int ftk_widget_height(FtkWidget* thiz);
int ftk_widget_is_insensitive(FtkWidget* thiz);
int ftk_widget_is_visible(FtkWidget* thiz);
int ftk_widget_is_focused(FtkWidget* thiz);
int ftk_widget_id(FtkWidget* thiz);
Ret ftk_widget_update(FtkWidget* thiz);
FtkGc* ftk_widget_get_gc(FtkWidget* thiz);
FtkCanvas* ftk_widget_canvas(FtkWidget* thiz);

void ftk_widget_show(FtkWidget* thiz, int visible);
void ftk_widget_move(FtkWidget* thiz, int x, int y);
void ftk_widget_resize(FtkWidget* thiz, int width, int height);
void ftk_widget_set_type(FtkWidget* thiz, int type);
void ftk_widget_set_insensitive(FtkWidget* thiz, int insensitive);
void ftk_widget_set_focused(FtkWidget* thiz, int focused);
void ftk_widget_set_id(FtkWidget* thiz, int id);
void ftk_widget_set_canvas(FtkWidget* thiz, FtkCanvas* canvas);
void ftk_widget_set_parent(FtkWidget* thiz, FtkWidget* parent);
void ftk_widget_append_child(FtkWidget* thiz, FtkWidget* child);
void ftk_widget_append_sibling(FtkWidget* thiz, FtkWidget* next);
void ftk_widget_remove_child(FtkWidget* thiz, FtkWidget* child);

FtkWidget* ftk_widget_toplevel(FtkWidget* thiz);
FtkWidget* ftk_widget_parent(FtkWidget* thiz);
FtkWidget* ftk_widget_prev(FtkWidget* thiz);
FtkWidget* ftk_widget_next(FtkWidget* thiz);
FtkWidget* ftk_widget_child(FtkWidget* thiz);
FtkWidget* ftk_widget_lookup(FtkWidget* thiz, int id);
FtkWidget* ftk_widget_find_target(FtkWidget* thiz, int x, int y);

void    ftk_widget_set_bg_color(FtkWidget* thiz, FtkWidgetState state, FtkColor color);
void    ftk_widget_set_fg_color(FtkWidget* thiz, FtkWidgetState state, FtkColor color);
void    ftk_widget_set_gc(FtkWidget* thiz, FtkWidgetState state, FtkGc* gc);

void ftk_widget_paint(FtkWidget* thiz);
void ftk_widget_destroy(FtkWidget* thiz);

void ftk_widget_ref(FtkWidget* thiz);
void ftk_widget_unref(FtkWidget* thiz);

void ftk_widget_paint_self(FtkWidget* thiz);
void ftk_widget_ref_self(FtkWidget* thiz);
void ftk_widget_unref_self(FtkWidget* thiz);
Ret ftk_widget_event(FtkWidget* thiz, FtkEvent* event);

#define FTK_BEGIN_PAINT(x, y, width, height, canvas) \
	int x = ftk_widget_left_abs(thiz);\
	int y = ftk_widget_top_abs(thiz);\
	int width  = ftk_widget_width(thiz);\
	int height = ftk_widget_height(thiz);\
	FtkCanvas* canvas = ftk_widget_canvas(thiz);

#define FTK_END_PAINT() \
	return ftk_widget_update(thiz);

#endif/*FTK_WIDGET_H*/

