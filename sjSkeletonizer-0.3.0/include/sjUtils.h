/**
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The Original Code is Copyright (C) 2007-2010 by Alexander Pinzon Fernandez.
 * Bogota - Colombia
 * All rights reserved.
 *
 * Author(s): Alexander Pinz�n Fern�ndez.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** 
 * @file sjLaplacianSmoothing.h
 * @brief sjSkeletonizer San Jeronimo Viewer Application.
 * @author Alexander Pinzon Fernandez.
 * @date 21/01/2011 10:07 a.m.
 */

#ifndef __SJUTILS__H__
#define __SJUTILS__H__

#include "sjDataType.h"

double angle3(sjPoint_3 p1, sjPoint_3 p2, sjPoint_3 p3);
double area3(sjPoint_3 a, sjPoint_3 b, sjPoint_3 c);

void printVector(std::vector<double> x);

void printMatrix(std::vector<std::vector<double>> y);
#endif
