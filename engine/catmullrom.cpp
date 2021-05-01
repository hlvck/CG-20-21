#include "catmullrom.h"

bool visibleCurves = true;

void buildRotMatrix(float *x, float *y, float *z, float *m) {

    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float *a, float *b, float *res) {

    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {

    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}


float length(float *v) {

    float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    return res;

}

void multMatrixVector(float *m, float *v, float *res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }
}

void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {

    // catmull-rom matrix
    float m[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
                         { 1.0f, -2.5f,  2.0f, -0.5f},
                         {-0.5f,  0.0f,  0.5f,  0.0f},
                         { 0.0f,  1.0f,  0.0f,  0.0f}};


    float pm[4][3] = { { p0[0],  p0[1], p0[2]  },
                       { p1[0], p1[1],  p1[2]  },
                       { p2[0],  p2[1],  p2[2] },
                       { p3[0],  p3[1],  p3[2]}};

    float tv[4] = {t*t*t, t*t, t, 1};
    float tdv[4] = {3*t*t, 2*t, 1, 0};

    // Compute A = M * P
    float a[4][3] = {0};

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                a[i][k] += m[i][j] * pm[j][k];
            }
        }
    }

    // Compute pos = T * A
    float npos[3] = {0};
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            npos[j] += a[i][j] * tv[i];
        }
    }

    // compute deriv = T' * A
    float nder[3] = {0};
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            nder[j] += a[i][j] * tdv[i];
        }
    }

    for(int i = 0; i < 3; i++)
    {
        pos[i] = npos[i];
        deriv[i] = nder[i];
    }
}

// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv, float *points, int size) {

    float t = gt * size; // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    int indices[4];
    indices[0] = (index + size-1)%size;
    indices[1] = (indices[0]+1)%size;
    indices[2] = (indices[1]+1)%size;
    indices[3] = (indices[2]+1)%size;

    getCatmullRomPoint(t, &points[indices[0]*3], &points[indices[1]*3], &points[indices[2]*3], &points[indices[3]*3], pos, deriv);
}

void renderCatmullRomCurve(float* points, int size) {

// draw curve using line segments with GL_LINE_LOOP
    float pos[3] = { 0.0f, 0.0f, 0.0f };
    float deriv[3] = { 0.0f, 0.0f, 0.0f };
    float gt = 0;

    glBegin(GL_LINE_LOOP);
    while(gt<1) {
        getGlobalCatmullRomPoint(gt, pos, deriv, points, size);
        glVertex3f(pos[0], pos[1], pos[2]);
        gt += 0.01;
    }
    glEnd();
}

void toggleCurve()
{
    visibleCurves = !visibleCurves;
}
float catmullRom(float t, float tTotal, float* points, int size, float* prevY)
{
    float pos[3] = {0};
    float deriv[3] = {0};
    float y[3] = {0};
    float z[3] = {0};

    if(visibleCurves) renderCatmullRomCurve(points, size);
    getGlobalCatmullRomPoint(t, pos, deriv, points, size);
    glTranslatef(pos[0], pos[1], pos[2]);

    float x[3] = { deriv[0], deriv[1], deriv[2] };
    normalize(x);
    cross(x, prevY, z);
    normalize(z);
    cross(z, x, y);
    normalize(y);
    for(int i = 0; i < 3; i++) prevY[i] = y[i];
    float m[16];
    buildRotMatrix(x, y, z, m);
    glMultMatrixf((float*)m);

    float spf = 0;
    if(fps) spf = 1/fps;
    t = t < 1 ? t+(spf/tTotal) : 0;
    return t;
}