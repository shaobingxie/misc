/*
 * File: ftk_window.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   window
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

#include "ftk_log.h"
#include "ftk_window.h"
#include "ftk_globals.h"

typedef struct _PrivInfo
{
	char* title;
	FtkCanvas*  canvas;
	FtkDisplay* display;
	FtkWidget*  focus_widget;
	FtkWndManager* wnd_manager;
}PrivInfo;

Ret ftk_window_set_focus(FtkWidget* thiz, FtkWidget* focus_widget)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL && thiz != focus_widget, RET_FAIL);

	if(priv->focus_widget == focus_widget)
	{
		return RET_OK;
	}

	if(priv->focus_widget != NULL)
	{
		ftk_widget_set_focused(priv->focus_widget, 0);
	}

	priv->focus_widget = focus_widget;
	
	if(priv->focus_widget != NULL)
	{
		ftk_widget_set_focused(priv->focus_widget, 1);
	}

	return RET_OK;
}

FtkWidget* ftk_window_get_focus(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, NULL);

	return priv->focus_widget;
}

static Ret ftk_window_on_key_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_FAIL;
	FtkWidget* iter = NULL;
	DECL_PRIV0(thiz, priv);

	if(priv->focus_widget == NULL)
	{
		return RET_OK;
	}

	ret = ftk_widget_event(priv->focus_widget, event);
	
	if(ret == RET_REMOVE || event->type == FTK_EVT_KEY_DOWN)
	{
		return RET_OK;
	}

	switch(event->u.key.code)
	{
		case FTK_KEY_LEFT:
		case FTK_KEY_UP:
		{
			iter = priv->focus_widget;
			while((iter = ftk_widget_prev(iter)) != NULL)
			{
				if(!ftk_widget_is_insensitive(iter) && ftk_widget_is_visible(iter))
				{
					ftk_window_set_focus(thiz, iter);
					break;
				}
			}
			break;
		}
		case FTK_KEY_DOWN:
		case FTK_KEY_RIGHT:
		case FTK_KEY_TAB:
		{
			iter = priv->focus_widget;
			while((iter = ftk_widget_next(iter)) != NULL)
			{
				if(!ftk_widget_is_insensitive(iter) && ftk_widget_is_visible(iter))
				{
					ftk_window_set_focus(thiz, iter);
					break;
				}
			}
			break;
		}
		default:break;
	}

	return RET_OK;
}

static Ret ftk_window_on_mouse_event(FtkWidget* thiz, FtkEvent* event)
{
	Ret ret = RET_FAIL;
	FtkWidget* target = NULL;
	
	if((target = ftk_widget_find_target(thiz, event->u.mouse.x, event->u.mouse.y)) != NULL && target != thiz)
	{
		if(event->type == FTK_EVT_MOUSE_DOWN && !ftk_widget_is_insensitive(target))
		{
			ftk_window_set_focus(thiz, target);
		}

		if(!ftk_widget_is_insensitive(target))
		{
			ret = ftk_widget_event(target, event);
		}
	}

	return ret;
}

static Ret ftk_window_on_event(FtkWidget* thiz, FtkEvent* event)
{
	return_val_if_fail(thiz != NULL && event != NULL, RET_FAIL);

	switch(event->type)
	{
		case FTK_EVT_UPDATE:
		{
			ftk_window_update(thiz, &event->u.rect);
			break;
		}
		case FTK_EVT_SHOW:
		{
			ftk_widget_paint(thiz);
			break;
		}
		case FTK_EVT_HIDE:
		{
			/*FIXME*/
			break;
		}
		case FTK_EVT_MOUSE_DOWN:
		case FTK_EVT_MOUSE_UP:
		case FTK_EVT_MOUSE_MOVE:
		{
			ftk_window_on_mouse_event(thiz, event);
			break;
		}
		case FTK_EVT_KEY_DOWN:
		case FTK_EVT_KEY_UP:
		{
			ftk_window_on_key_event(thiz, event);
			break;
		}
		default:
		{
			ftk_logd("%s: type=%d\n", __func__, event->type);
			break;
		}
	}

	return RET_OK;
}

static Ret ftk_window_on_paint(FtkWidget* thiz)
{
	FtkRect rect = {0};
	DECL_PRIV0(thiz, priv);

	rect.width  = ftk_widget_width(thiz);
	rect.height = ftk_widget_height(thiz);

	ftk_display_update(priv->display, 
		ftk_canvas_bitmap(priv->canvas), &rect, 
		ftk_widget_left(thiz), ftk_widget_top(thiz));

	return RET_OK;
}

static void ftk_window_destroy(FtkWidget* thiz)
{
	if(thiz != NULL)
	{
		FtkEvent event = {0};
		DECL_PRIV0(thiz, priv);

		event.type = FTK_EVT_WND_DESTROY;
		event.widget = thiz;

		ftk_wnd_manager_dispatch(ftk_default_wnd_manager(), &event);

		FTK_FREE(priv->title);
		ftk_canvas_destroy(priv->canvas);
		FTK_ZFREE(priv, sizeof(PrivInfo));
	}

	return;
}

Ret        ftk_window_set_title(FtkWidget* thiz, const char* title)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL && title != NULL, RET_FAIL);

	FTK_FREE(priv->title);
	priv->title = strdup(title);

	return RET_OK;
}

const char*ftk_window_get_title(FtkWidget* thiz)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, NULL);

	return priv->title;
}

Ret        ftk_window_update(FtkWidget* thiz, FtkRect* rect)
{
	int xoffset = 0;
	int yoffset = 0;
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL, RET_FAIL);

	xoffset = ftk_widget_left(thiz) + rect->x;
	yoffset = ftk_widget_top(thiz) + rect->y;

	return ftk_display_update(priv->display, ftk_canvas_bitmap(priv->canvas), rect, xoffset, yoffset);
}

FtkWidget* ftk_window_create(int x, int y, int width, int height)
{
	FtkCanvas* canvas = NULL;
	FtkWidget* thiz = (FtkWidget*)FTK_ZALLOC(sizeof(FtkWidget));
	
	if(thiz != NULL)
	{
		FtkColor color = {0};
		thiz->priv_subclass[0] = FTK_ZALLOC(sizeof(PrivInfo));
		do
		{
			DECL_PRIV0(thiz, priv);	
			if(priv == NULL)
			{
				break;
			}
			canvas = ftk_canvas_create(width, height, color);
			if(canvas == NULL)
			{
				break;
			}
			priv->canvas  = canvas;
			priv->display = ftk_default_display();

			ftk_widget_init(thiz, 0, 0);
			ftk_widget_set_canvas(thiz, canvas);
			ftk_widget_move(thiz, x, y);
			ftk_widget_resize(thiz, width, height);

			thiz->on_event = ftk_window_on_event;
			thiz->on_paint = ftk_window_on_paint;
			thiz->destroy  = ftk_window_destroy;

			ftk_wnd_manager_add(ftk_default_wnd_manager(), thiz);
		}while(0);

		if(canvas == NULL)
		{
			FTK_FREE(thiz->priv_subclass[0]);
			FTK_FREE(thiz);
		}
	}

	return thiz;
}

Ret ftk_window_attach(FtkWidget* thiz, FtkWndManager* wnd_manager)
{
	DECL_PRIV0(thiz, priv);
	return_val_if_fail(thiz != NULL && wnd_manager != NULL, RET_FAIL);

	priv->wnd_manager = wnd_manager;
	ftk_wnd_manager_add(wnd_manager, thiz);

	return RET_OK;
}

#ifdef FTK_WINDOW_TEST
#include "ftk_display_fb.h"

int main(int argc, char* argv[])
{
	FtkGc gc = {0};
	gc.mask = FTK_GC_FONT;
	gc.font = ftk_font_create("./unicode.fnt", 16);
	FtkDisplay* display = ftk_display_fb_create(FTK_FB_NAME);
	FtkWidget* thiz = ftk_window_create(display, 100, 0, 0, 320, 480);
	ftk_window_set_focus(thiz, NULL);
	assert(ftk_window_get_focus(thiz) == NULL);

	ftk_window_set_title(thiz, "HelloWorld.");
	assert(strcmp(ftk_window_get_title(thiz), "HelloWorld.") == 0);
	ftk_widget_set_gc(thiz, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_show(thiz, 1);
	ftk_widget_paint(thiz);
	ftk_widget_unref(thiz);
	ftk_font_destroy(gc.font);
	ftk_display_destroy(display);

	return 0;
}
#endif/*FTK_WINDOW_TEST*/

