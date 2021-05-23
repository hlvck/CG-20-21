#define USE_MATH_DEFINES

#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void multMatrixVector(float *m, float *v, float *res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }
}

void normalize(float *a) {
    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}

void cross(float *a, float *b, float *res) {
    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

void plane(double dlength, char* filename)
{
    double length = dlength/2;
    std::ofstream file;
    file.open(filename);
    file << "6" << std::endl;
    file << length << ",0.0," << length << std::endl;
    file << length << ",0.0," << -length << std::endl;
    file << -length << ",0.0," << -length << std::endl;
    file << -length << ",0.0," << -length << std::endl;
    file << -length << ",0.0," << length << std::endl;
    file << length << ",0.0," << length << std::endl;
    //normal
    file << "0.0, 1.0, 0.0" << std::endl;
    file << "0.0, 1.0, 0.0" << std::endl;
    file << "0.0, 1.0, 0.0" << std::endl;
    file << "0.0, 1.0, 0.0" << std::endl;
    file << "0.0, 1.0, 0.0" << std::endl;
    file << "0.0, 1.0, 0.0" << std::endl;
    //texture
    file << "1.0, 0.0" << std::endl;
    file << "1.0, 1.0" << std::endl;
    file << "0.0, 1.0" << std::endl;
    file << "0.0, 1.0" << std::endl;
    file << "0.0, 0.0" << std::endl;
    file << "1.0, 0.0" << std::endl;
    file.close();
}

void box(double xlen, double ylen, double zlen, int divisions, char* filename)
{
    std::ofstream file;
    file.open(filename);
    file << 6*6*(divisions*divisions) << std::endl;
    std::vector<float> normal, texture;
    for(int i = 0; i<divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            double x1 = (j*xlen)/divisions;
            double x2 = ((j+1)*xlen)/divisions;
            double z1 = (j*zlen)/divisions;
            double z2 = ((j+1)*zlen)/divisions;
            double y1 = (i*ylen)/divisions;
            double y2 = ((i+1)*ylen)/divisions;
            double x1tb = (i*xlen)/divisions;
            double x2tb = ((i+1)*xlen)/divisions;
            double tx1 = j/(double)divisions;
            double tx2 = (j+1)/(double)divisions;
            double ty1 = i/(double)divisions;
            double ty2 = (i+1)/(double)divisions;

            file << x1 << "," << y1 << "," << zlen << std::endl;  // Face by x, z
            file << x2 << "," << y1 << "," << zlen << std::endl;
            file << x1 << "," << y2 << "," << zlen << std::endl;
            file << x1 << "," << y2 << "," << zlen << std::endl;
            file << x2 << "," << y1 << "," << zlen << std::endl;
            file << x2 << "," << y2 << "," << zlen << std::endl;

            normal.emplace_back(0); normal.emplace_back(0); normal.emplace_back(1);
            normal.emplace_back(0); normal.emplace_back(0); normal.emplace_back(1);
            normal.emplace_back(0); normal.emplace_back(0); normal.emplace_back(1);
            normal.emplace_back(0); normal.emplace_back(0); normal.emplace_back(1);
            normal.emplace_back(0); normal.emplace_back(0); normal.emplace_back(1);
            normal.emplace_back(0); normal.emplace_back(0); normal.emplace_back(1);

            texture.emplace_back(tx1/3); texture.emplace_back(ty1/2 + 0.5f);
            texture.emplace_back(tx2/3); texture.emplace_back(ty1/2 + 0.5f);
            texture.emplace_back(tx1/3); texture.emplace_back(ty2/2 + 0.5f);
            texture.emplace_back(tx1/3); texture.emplace_back(ty2/2 + 0.5f);
            texture.emplace_back(tx2/3); texture.emplace_back(ty1/2 + 0.5f);
            texture.emplace_back(tx2/3); texture.emplace_back(ty2/2 + 0.5f);

            file << x1 << "," << y1 << "," << "0.0" << std::endl;  // Face by x, 0
            file << x1 << "," << y2 << "," << "0.0" << std::endl;
            file << x2 << "," << y2 << "," << "0.0" << std::endl;
            file << x2 << "," << y2 << "," << "0.0" << std::endl;
            file << x2 << "," << y1 << "," << "0.0" << std::endl;
            file << x1 << "," << y1 << "," << "0.0" << std::endl;

            normal.emplace_back(0); normal.emplace_back(0); normal.emplace_back(-1);
            normal.emplace_back(0); normal.emplace_back(0); normal.emplace_back(-1);
            normal.emplace_back(0); normal.emplace_back(0); normal.emplace_back(-1);
            normal.emplace_back(0); normal.emplace_back(0); normal.emplace_back(-1);
            normal.emplace_back(0); normal.emplace_back(0); normal.emplace_back(-1);
            normal.emplace_back(0); normal.emplace_back(0); normal.emplace_back(-1);

            texture.emplace_back(((float)divisions/3) - tx1/3 + 0.33f); texture.emplace_back(ty1/2 + 0.5f);
            texture.emplace_back(((float)divisions/3) - tx1/3 + 0.33f); texture.emplace_back(ty2/2 + 0.5f);
            texture.emplace_back(((float)divisions/3) - tx2/3 + 0.33f); texture.emplace_back(ty2/2 + 0.5f);
            texture.emplace_back(((float)divisions/3) - tx2/3 + 0.33f); texture.emplace_back(ty2/2 + 0.5f);
            texture.emplace_back(((float)divisions/3) - tx2/3 + 0.33f); texture.emplace_back(ty1/2 + 0.5f);
            texture.emplace_back(((float)divisions/3) - tx1/3 + 0.33f); texture.emplace_back(ty1/2 + 0.5f);

            file << "0.0" << "," << y1 << "," << z1 << std::endl;  // Face by z, 0
            file << "0.0" << "," << y1 << "," << z2 << std::endl;
            file << "0.0" << "," << y2 << "," << z1 << std::endl;
            file << "0.0" << "," << y2 << "," << z1 << std::endl;
            file << "0.0" << "," << y1 << "," << z2 << std::endl;
            file << "0.0" << "," << y2 << "," << z2 << std::endl;

            normal.emplace_back(-1); normal.emplace_back(0); normal.emplace_back(0);
            normal.emplace_back(-1); normal.emplace_back(0); normal.emplace_back(0);
            normal.emplace_back(-1); normal.emplace_back(0); normal.emplace_back(0);
            normal.emplace_back(-1); normal.emplace_back(0); normal.emplace_back(0);
            normal.emplace_back(-1); normal.emplace_back(0); normal.emplace_back(0);
            normal.emplace_back(-1); normal.emplace_back(0); normal.emplace_back(0);

            texture.emplace_back(tx1/3 + 0.66f); texture.emplace_back(ty1/2 + 0.5f);
            texture.emplace_back(tx2/3 + 0.66f); texture.emplace_back(ty1/2 + 0.5f);
            texture.emplace_back(tx1/3 + 0.66f); texture.emplace_back(ty2/2 + 0.5f);
            texture.emplace_back(tx1/3 + 0.66f); texture.emplace_back(ty2/2 + 0.5f);
            texture.emplace_back(tx2/3 + 0.66f); texture.emplace_back(ty1/2 + 0.5f);
            texture.emplace_back(tx2/3 + 0.66f); texture.emplace_back(ty2/2 + 0.5f);

            file << xlen << "," << y1 << "," << z1 << std::endl;  // Face by z, x
            file << xlen << "," << y2 << "," << z1 << std::endl;
            file << xlen << "," << y2 << "," << z2 << std::endl;
            file << xlen << "," << y2 << "," << z2 << std::endl;
            file << xlen << "," << y1 << "," << z2 << std::endl;
            file << xlen << "," << y1 << "," << z1 << std::endl;

            normal.emplace_back(1); normal.emplace_back(0); normal.emplace_back(0);
            normal.emplace_back(1); normal.emplace_back(0); normal.emplace_back(0);
            normal.emplace_back(1); normal.emplace_back(0); normal.emplace_back(0);
            normal.emplace_back(1); normal.emplace_back(0); normal.emplace_back(0);
            normal.emplace_back(1); normal.emplace_back(0); normal.emplace_back(0);
            normal.emplace_back(1); normal.emplace_back(0); normal.emplace_back(0);

            texture.emplace_back(((float)divisions/3) - tx1/3); texture.emplace_back(ty1/2);
            texture.emplace_back(((float)divisions/3) - tx1/3); texture.emplace_back(ty2/2);
            texture.emplace_back(((float)divisions/3) - tx2/3); texture.emplace_back(ty2/2);
            texture.emplace_back(((float)divisions/3) - tx2/3); texture.emplace_back(ty2/2);
            texture.emplace_back(((float)divisions/3) - tx2/3); texture.emplace_back(ty1/2);
            texture.emplace_back(((float)divisions/3) - tx1/3); texture.emplace_back(ty1/2);

            file << x1tb << "," << ylen << "," << z1 << std::endl;  // Top of box
            file << x1tb << "," << ylen << "," << z2 << std::endl;
            file << x2tb << "," << ylen << "," << z2 << std::endl;
            file << x2tb << "," << ylen << "," << z2 << std::endl;
            file << x2tb << "," << ylen << "," << z1 << std::endl;
            file << x1tb << "," << ylen << "," << z1 << std::endl;

            normal.emplace_back(0); normal.emplace_back(1); normal.emplace_back(0);
            normal.emplace_back(0); normal.emplace_back(1); normal.emplace_back(0);
            normal.emplace_back(0); normal.emplace_back(1); normal.emplace_back(0);
            normal.emplace_back(0); normal.emplace_back(1); normal.emplace_back(0);
            normal.emplace_back(0); normal.emplace_back(1); normal.emplace_back(0);
            normal.emplace_back(0); normal.emplace_back(1); normal.emplace_back(0);

            texture.emplace_back(tx1/3 + 0.33f); texture.emplace_back(ty1/2);
            texture.emplace_back(tx2/3 + 0.33f); texture.emplace_back(ty1/2);
            texture.emplace_back(tx2/3 + 0.33f); texture.emplace_back(ty2/2);
            texture.emplace_back(tx2/3 + 0.33f); texture.emplace_back(ty2/2);
            texture.emplace_back(tx1/3 + 0.33f); texture.emplace_back(ty2/2);
            texture.emplace_back(tx1/3 + 0.33f); texture.emplace_back(ty1/2);

            file << x1tb << "," << "0.0" << "," << z1 << std::endl;  // Bottom of box
            file << x2tb << "," << "0.0" << "," << z1 << std::endl;
            file << x1tb << "," << "0.0" << "," << z2 << std::endl;
            file << x1tb << "," << "0.0" << "," << z2 << std::endl;
            file << x2tb << "," << "0.0" << "," << z1 << std::endl;
            file << x2tb << "," << "0.0" << "," << z2 << std::endl;

            normal.emplace_back(0); normal.emplace_back(-1); normal.emplace_back(0);
            normal.emplace_back(0); normal.emplace_back(-1); normal.emplace_back(0);
            normal.emplace_back(0); normal.emplace_back(-1); normal.emplace_back(0);
            normal.emplace_back(0); normal.emplace_back(-1); normal.emplace_back(0);
            normal.emplace_back(0); normal.emplace_back(-1); normal.emplace_back(0);
            normal.emplace_back(0); normal.emplace_back(-1); normal.emplace_back(0);

            texture.emplace_back(((float)divisions/3) - tx1/3 + 0.66f); texture.emplace_back(ty1/2);
            texture.emplace_back(((float)divisions/3) - tx1/3 + 0.66f); texture.emplace_back(ty2/2);
            texture.emplace_back(((float)divisions/3) - tx2/3 + 0.66f); texture.emplace_back(ty1/2);
            texture.emplace_back(((float)divisions/3) - tx2/3 + 0.66f); texture.emplace_back(ty1/2);
            texture.emplace_back(((float)divisions/3) - tx1/3 + 0.66f); texture.emplace_back(ty2/2);
            texture.emplace_back(((float)divisions/3) - tx2/3 + 0.66f); texture.emplace_back(ty2/2);
        }
    }

    for(int i = 0; i < normal.size(); i+=3) {
        file << normal[i] << "," << normal[i+1] << "," << normal[i+2] << std::endl;
    }

    for(int i = 0; i < texture.size(); i+=3) {
        file << texture[i] << "," << texture[i+1] << "," << texture[i+2] << std::endl;
    }

    file.close();
}

void sphere (double radius, int slices, int stacks, char* filename)
{
    std::ofstream file;
    file.open(filename);
    file << slices * stacks * 6 << std::endl;
    std::vector<float> normal, texture;
    for(int i = 0; i < slices; i++)
    {
        double t1x = i/(double)slices;
        double t2x = (i+1)/(double)slices;
        for(int j = 0; j < stacks; j++)
        {
            double a1 = (i*2*M_PI)/slices;
            double a2 = ((i+1)*2*M_PI)/slices;
            double b1 = (M_PI/2)-(j*M_PI/stacks);
            double b2 = (M_PI/2)-((j+1)*M_PI/stacks);
            double y1 = radius*sin(b1), ny1 = sin(b1);
            double y2 = radius*sin(b2), ny2 = sin(b2);
            double x1 = radius*cos(b1)*sin(a1), nx1 = cos(b1)*sin(a1);
            double x2 = radius*cos(b2)*sin(a1), nx2 = cos(b2)*sin(a1);
            double x3 = radius*cos(b2)*sin(a2), nx3 = cos(b2)*sin(a2);
            double x4 = radius*cos(b1)*sin(a2), nx4 = cos(b1)*sin(a2);
            double z1 = radius*cos(b1)*cos(a1), nz1 = cos(b1)*cos(a1);
            double z2 = radius*cos(b2)*cos(a1), nz2 = cos(b2)*cos(a1);
            double z3 = radius*cos(b2)*cos(a2), nz3 = cos(b2)*cos(a2);
            double z4 = radius*cos(b1)*cos(a2), nz4 = cos(b1)*cos(a2);
            double t1y = 1 - (j/(double)stacks);
            double t2y = 1 - ((j+1)/(double)stacks);

            file << x1 << "," << y1 << "," << z1 << std::endl;
            file << x2 << "," << y2 << "," << z2 << std::endl;
            file << x3 << "," << y2 << "," << z3 << std::endl;
            file << x4 << "," << y1 << "," << z4 << std::endl;
            file << x1 << "," << y1 << "," << z1 << std::endl;
            file << x3 << "," << y2 << "," << z3 << std::endl;

            normal.emplace_back(nx1); normal.emplace_back(ny1); normal.emplace_back(nz1);
            normal.emplace_back(nx2); normal.emplace_back(ny2); normal.emplace_back(nz2);
            normal.emplace_back(nx3); normal.emplace_back(ny2); normal.emplace_back(nz3);
            normal.emplace_back(nx4); normal.emplace_back(ny1); normal.emplace_back(nz4);
            normal.emplace_back(nx1); normal.emplace_back(ny1); normal.emplace_back(nz1);
            normal.emplace_back(nx3); normal.emplace_back(ny2); normal.emplace_back(nz3);

            texture.emplace_back(t1x); texture.emplace_back(t1y);
            texture.emplace_back(t1x); texture.emplace_back(t2y);
            texture.emplace_back(t2x); texture.emplace_back(t2y);
            texture.emplace_back(t2x); texture.emplace_back(t1y);
            texture.emplace_back(t1x); texture.emplace_back(t1y);
            texture.emplace_back(t2x); texture.emplace_back(t2y);
        }
    }
    for(int i = 0; i < normal.size(); i+=3) {
        file << normal[i] << "," << normal[i+1] << "," << normal[i+2] << std::endl;
    }
    for(int i = 0; i < texture.size(); i+=2) {
        file << texture[i] << "," << texture[i + 1] << std::endl;
    }
    file.close();
}

void cone(double radius, double height, int slices, int stacks, char* filename)
{
    std::ofstream file;
    file.open(filename);
    std::vector<float> normal; float norm[3];
    file << 6*slices*stacks + 3*slices << std::endl;

    for(int i = 0; i < stacks; i++)
    {
        double r1 = radius - ((i*radius)/stacks);
        double r2 = radius - (((i+1)*radius)/stacks);
        double y1 = (i*height)/stacks;
        double y2 = ((i+1)*height)/stacks;

        for(int j = 0; j < slices; j++)
        {
            double a1 = (j*2*M_PI)/slices;
            double a2 = ((j+1)*2*M_PI)/slices;
            double x1 = r1*sin(a1), nx1 = sin(a1);
            double x2 = r1*sin(a2), nx2 = sin(a2);
            double x3 = r2*sin(a1), nx3 = sin(a1);
            double x4 = r2*sin(a2), nx4 = sin(a2);
            double z1 = r1*cos(a1), nz1 = cos(a1);
            double z2 = r1*cos(a2), nz2 = cos(a2);
            double z3 = r2*cos(a1), nz3 = cos(a1);
            double z4 = r2*cos(a2), nz4 = cos(a2);
            double ny = radius/height;

            file << x1 << "," << y1 << "," << z1 << std::endl;
            file << x2 << "," << y1 << "," << z2 << std::endl;
            file << x3 << "," << y2 << "," << z3 << std::endl;
            file << x2 << "," << y1 << "," << z2 << std::endl;
            file << x4 << "," << y2 << "," << z4 << std::endl;
            file << x3 << "," << y2 << "," << z3 << std::endl;

            norm[0] = nx1; norm[1] = ny; norm[2] = nz1; normalize(norm);
            normal.emplace_back(norm[0]); normal.emplace_back(norm[1]); normal.emplace_back(norm[2]);
            norm[0] = nx2; norm[1] = ny; norm[2] = nz2; normalize(norm);
            normal.emplace_back(norm[0]); normal.emplace_back(norm[1]); normal.emplace_back(norm[2]);
            norm[0] = nx3; norm[1] = ny; norm[2] = nz3; normalize(norm);
            normal.emplace_back(norm[0]); normal.emplace_back(norm[1]); normal.emplace_back(norm[2]);
            norm[0] = nx2; norm[1] = ny; norm[2] = nz2; normalize(norm);
            normal.emplace_back(norm[0]); normal.emplace_back(norm[1]); normal.emplace_back(norm[2]);
            norm[0] = nx4; norm[1] = ny; norm[2] = nz4; normalize(norm);
            normal.emplace_back(norm[0]); normal.emplace_back(norm[1]); normal.emplace_back(norm[2]);
            norm[0] = nx3; norm[1] = ny; norm[2] = nz3; normalize(norm);
            normal.emplace_back(norm[0]); normal.emplace_back(norm[1]); normal.emplace_back(norm[2]);

            if(i==0) //base
            {
                file << x1 << "," << y1 << "," << z1 << std::endl;
                file << "0.0" << "," << y1 << "," << "0.0" << std::endl;
                file << x2 << "," << y1 << "," << z2 << std::endl;

                normal.emplace_back(0); normal.emplace_back(-1); normal.emplace_back(0);
                normal.emplace_back(0); normal.emplace_back(-1); normal.emplace_back(0);
                normal.emplace_back(0); normal.emplace_back(-1); normal.emplace_back(0);
            }
        }
    }

    for(int i = 0; i < normal.size(); i+=3) {
        file << normal[i] << "," << normal[i+1] << "," << normal[i+2] << std::endl;
    }
    file.close();
}

float m[4][4] = {	{-1.0f,  3.0f, -3.0f,  1.0f},
                     { 3.0f, -6.0f,  3.0f, 0.0f},
                     {-3.0f,  3.0f,  0.0f,  0.0f},
                     { 1.0f,  0.0f,  0.0f,  0.0f}};

void genNormals(float u, float v, float cpoints[4][4][3], float result[3])
{
    float um[4] = {u*u*u, u*u, u, 1};
    float vm[4] = {v*v*v, v*v, v, 1};
    float ud[4] = {3*u*u, 2*u, u, 1};
    float vd[4] = {3*v*v, 2*v, v, 1};

    float resU[4];
    multMatrixVector((float*)m, um, resU);
    float resV[4];
    multMatrixVector((float*)m, vm, resV);
    float resdu[4];
    multMatrixVector((float*)m, ud, resdu);
    float resdv[4];
    multMatrixVector((float*)m, vd, resdv);

    float pnttu[4][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    float pnttv[4][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    // vec tu
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                pnttu[i][k] += cpoints[j][i][k] * resdu[j];
            }
        }
    }
    float resulttu[3] = {0,0,0};
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            resulttu[j] += pnttu[i][j] * resV[i];
        }
    }
    //vec tv
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                pnttv[i][k] += cpoints[j][i][k] * resU[j];
            }
        }
    }
    float resulttv[3] = {0,0,0};
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            resulttv[j] += pnttv[i][j] * resdv[i];
        }
    }
    //normal: normalized cross product of tangent vectors
    cross(resulttv, resulttu, result);
    normalize(result);
}

void genPoints(float u, float v, float cpoints[4][4][3], float result[3])
{
    float um[4] = {u*u*u, u*u, u, 1};
    float vm[4] = {v*v*v, v*v, v, 1};

    float resU[4];
    multMatrixVector((float*)m, um, resU);
    float resV[4];
    multMatrixVector((float*)m, vm, resV);
    float pnt[4][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                pnt[i][k] += cpoints[j][i][k] * resU[j];
            }
        }
    }
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            result[j] += pnt[i][j] * resV[i];
        }
    }

}

void bezier(int tessellation, char* bezfile, char* outfile) {
    std::ifstream file(bezfile);
    std::string line;

    if (!file.is_open()) {
        printf("Error opening Bezier patches file.\n");
        return;
    }

    std::stringstream ss;
    getline(file, line);
    int numPatches = stoi(line);
    auto indices = new int[numPatches][16];

    for(int i = 0; i < numPatches; i++)
    {
        int j = 0;
        getline(file, line);
        ss.clear();
        ss.str(line);
        while(ss.good()) {
            std::string substr;
            getline(ss, substr, ',');
            indices[i][j] = stoi(substr);
            j++;
        }
    }

    getline(file, line);
    int numPoints = stoi(line);
    auto controlPoints = new float[numPoints][3];

    for(int i = 0; i < numPoints; i++)
    {
        int j = 0;
        getline(file, line);
        ss.clear();
        ss.str(line);
        while(ss.good()) {
            std::string substr;
            getline(ss, substr, ',');
            controlPoints[i][j] = stof(substr);
            j++;
        }
    }

    auto bezierPoints = new float[numPatches][16][3];
    for(int i = 0; i < numPatches; i++) {
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 3; k++)
            {
                bezierPoints[i][j][k] = controlPoints[indices[i][j]][k];
            }
        }
    }

    std::ofstream output(outfile);
    output << (numPatches * tessellation * (tessellation) *6) << std::endl;

    float change = 1.0f/tessellation;
    std::vector<float> normals;

    for(int p = 0; p < numPatches; p++)
    {
        float fromPatches[4][4][3];
        for (int z = 0; z < 4; z++) {
            for (int x = 0; x < 4; x++) {
                for (int w = 0; w < 3; w++) {
                    fromPatches[z][x][w] = bezierPoints[p][z * 4 + x][w];
                }
            }
        }
        for (int j = 0; j < tessellation; j++)
        {
            float u = j*change;
            float u2 = (j+1)*change;

            for(int k = 0; k < tessellation; k++)
            {
                float v = k*change;
                float v2 = (k+1)*change;

                float p1[3] = {0,0,0}, p2[3] = {0,0,0}, p3[3] = {0,0,0}, p4[3] = {0,0,0};
                float n1[3] = {0,0,0}, n2[3] = {0,0,0}, n3[3] = {0,0,0}, n4[3] = {0,0,0};
                genPoints(u, v, fromPatches, p1);
                genPoints(u, v2, fromPatches, p2);
                genPoints(u2, v, fromPatches, p3);
                genPoints(u2, v2, fromPatches, p4);
                genNormals(u, v, fromPatches, n1);
                genNormals(u, v2, fromPatches, n2);
                genNormals(u2, v, fromPatches, n3);
                genNormals(u2, v2, fromPatches, n4);

                output << p4[0] << "," << p4[1] << "," << p4[2] << std::endl;
                output << p3[0] << "," << p3[1] << "," << p3[2] << std::endl;
                output << p2[0] << "," << p2[1] << "," << p2[2] << std::endl;
                output << p3[0] << "," << p3[1] << "," << p3[2] << std::endl;
                output << p1[0] << "," << p1[1] << "," << p1[2] << std::endl;
                output << p2[0] << "," << p2[1] << "," << p2[2] << std::endl;

                normals.emplace_back(n4[0]); normals.emplace_back(n4[1]); normals.emplace_back(n4[2]);
                normals.emplace_back(n3[0]); normals.emplace_back(n3[1]); normals.emplace_back(n3[2]);
                normals.emplace_back(n2[0]); normals.emplace_back(n2[1]); normals.emplace_back(n2[2]);
                normals.emplace_back(n3[0]); normals.emplace_back(n3[1]); normals.emplace_back(n3[2]);
                normals.emplace_back(n1[0]); normals.emplace_back(n1[1]); normals.emplace_back(n1[2]);
                normals.emplace_back(n2[0]); normals.emplace_back(n2[1]); normals.emplace_back(n2[2]);

            }
        }
    }

    for(int i = 0; i < normals.size(); i+=3) {
        output << normals[i] << "," << normals[i+1] << "," << normals[i+2] << std::endl;
    }
    output.close();
}

int main(int argc, char** argv)
{
    if (argc == 4 && !strcmp(argv[1], "plane"))
    {
        plane(atof(argv[2]), argv[3]);
    }
    else if(argc == 7 && !strcmp(argv[1], "box"))
    {
        box(atof(argv[2]), atof(argv[3]), atof(argv[4]), atoi(argv[5]), argv[6]);
    }
    else if(argc == 6 && !strcmp(argv[1], "sphere"))
    {
        sphere(atof(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
    }
    else if(argc == 7 && !strcmp(argv[1], "cone"))
    {
        cone(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
    }
    else if(argc == 5 && !strcmp(argv[1], "bezier"))
    {
        bezier(atoi(argv[2]), argv[3], argv[4]);
    }
    else
    {
        printf("Incorrect formatting. Usage: ./generator <primitive type> <parameters> <file name>\n");
    }
}