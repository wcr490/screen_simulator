#include "main.h"
#include "stdio.h"

#define STR_WIDTH 8
#define STR_HEIGHT 16

/*
 * Demo for introducing 'aff_mat.c'
 * Follow those comments with star
 */

int draw_affine_charXXX_buffer(SOCKET s, char c,
                               uint16_t x, uint16_t y,
                               mat3 *mat // *Get a matrix of affine transformation
)
{
    int index = (c - ' ') * 14;
    // *Centre point of the graphics object
    vec_xy ref_xy = {x + 4, y + 7};
    // I assume It is for 8x14
    for (int i = 0; i < 14; i++)
    {
        for (int k = 0; k < 8; k++)
        {

            // *Deal with every points in the graphics
            if (ASCXXX[index + i] & (128 >> k))
            {
                vec_xy xy;
                // *Wrap the point into a vector
                xy[0] = x + k, xy[1] = y + i;
                // *Apply transformation with xy and reference xy
                vec_xy_transformation_apply(mat, xy, ref_xy);
                // *Currently, xy has contains the actual coordinates
                int ax = roundf(xy[0]), ay = roundf(xy[1]);
                if (ax < 0 || ay < 0)
                {
                    continue;
                }
                // TODO: It will actually cause some bias Orz.
                if (draw_point_buffer(s, ax, ay))
                    return 1;
            }
        }
    }
    return 0;
}
int comeup_matrix(void)
{
    float sin = 0.7071;
    mat3 mat;
    // I don't consider it's sign here
    float cos = newton_sqrt((1 - sin * sin), 3);
    mat3_set_angle(&mat, sin, cos);
    return 0;
}