#include "stdio.h"
#include "aff_mat.h"

void mat3_init_identity(mat3 *self)
{
    self->a00 = 1,
    self->a01 = 0,
    self->a02 = 0,
    self->a10 = 0,
    self->a11 = 1,
    self->a12 = 0,
    self->a20 = 0,
    self->a21 = 0,
    self->a22 = 1;
}
// Clockwise in the default implementation
void mat3_set_angle(mat3 *self, float sin, float cos)
{
    mat3_init_identity(self);
    self->a00 = cos, self->a01 = -sin, self->a10 = sin, self->a11 = cos;
}
void mat3_set_scale(mat3 *self, float scale)
{
    mat3_init_identity(self);
    self->a00 = scale, self->a11 = scale;
}
void mat3_set_translate(mat3 *self, float dx, float dy)
{
    mat3_init_identity(self);
    self->a02 = dx, self->a12 = dy;
}

void mat3_transform(mat3 *self, vec_xy xy, float z_coor)
{
    float tmpx = xy[0], tmpy = xy[1];
    xy[0] = self->a00 * tmpx + self->a01 * tmpy + self->a02 * z_coor;
    xy[1] = self->a10 * tmpx + self->a11 * tmpy + self->a12 * z_coor;
}
void vec_xy_transformation_apply(mat3 *self, vec_xy xy, vec_xy ref_xy)
{
    xy[0] -= ref_xy[0], xy[1] -= ref_xy[1];
    mat3_transform(self, xy, 1);
    xy[0] += ref_xy[0], xy[1] += ref_xy[1];
}
void mat3_print(mat3 *mat)
{
    printf("----------\n");
    printf("%f %f %f \n", mat->a00, mat->a01, mat->a02);
    printf("%f %f %f \n", mat->a10, mat->a11, mat->a12);
    printf("%f %f %f \n", mat->a20, mat->a21, mat->a22);
    printf("----------\n");
}

float fabsf(float x)
{
    return (x < 0.0f) ? -x : x;
}
float newton_sqrt(float x, uint8_t precision_digit_num)
{
    if (x < 0)
        return -1.0f;
    if (x == 1 || x == 0)
        return x;
    float __init = x;
    float precision = 1;
    for (int i = 0; i < precision_digit_num; i++)
    {
        precision *= 0.1f;
    }
    while (1)
    {
        float next_x = (x + __init / x) * 0.5f;
        if (fabsf(next_x - x) < precision)
            break;
        x = next_x;
    }
    return x;
}
int _roundf(float x)
{
    if (x > 0.0f)
    {
        return (int)(x + 0.5f);
    }
    else
    {
        return (int)(x - 0.5f);
    }
}