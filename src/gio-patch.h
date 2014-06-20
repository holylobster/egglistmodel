/*
 * Copyright © 2014 Lars Uebernickel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Lars Uebernickel <lars@uebernic.de>
 */

#ifndef __PATCHED_GIO_H__
#define __PATCHED_GIO_H__

#include <gio/gio.h>

/* Patch GListModel into gio until it lands */

#ifdef __G_LIST_MODEL_H__
  #warning "GListModel seems to have landed in gio. This file can be removed now."
#else
  #define __GIO_GIO_H_INSIDE__
  #define GIO_COMPILATION

  typedef struct _GListModel GListModel;

  #include "glistmodel.h"

  #undef __GIO_GIO_H_INSIDE__
  #undef GIO_COMPILATION
#endif

#endif
