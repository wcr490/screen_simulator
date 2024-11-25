#ifndef __AFF_MAT_H__
#define __AFF_MAT_H__

#include "stdint.h"

/* Type Definitions */
typedef struct
{
    float a00, a01, a02;
    float a10, a11, a12;
    float a20, a21, a22;
} mat3;
typedef float vec_xy[2];

/* Matrix */
void mat3_init_identity(mat3 *self);
void mat3_set_angle(mat3 *self, float sin, float cos);
void mat3_set_scale(mat3 *self, float scale);
void mat3_set_translate(mat3 *self, float dx, float dy);
void mat3_transform(mat3 *self, vec_xy xy, float bias);

/* Graph */
// Usually, use the centre point of the graph as reference point.
void vec_xy_transformation_apply(mat3 *self, vec_xy xy, vec_xy ref_xy);

/* Others */
float fabsf(float x);
float newton_sqrt(float x, uint8_t precision_digit_num);
int _roundf(float x);

#endif