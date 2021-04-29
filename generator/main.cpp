#define USE_MATH_DEFINES

#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

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
    file.close();
}

void box(double xlen, double ylen, double zlen, int divisions, char* filename)
{
    std::ofstream file;
    file.open(filename);
    file << 6*6*(divisions*divisions) << std::endl;
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

            file << x1 << "," << y1 << "," << zlen << std::endl;  // Face by x, z
            file << x2 << "," << y1 << "," << zlen << std::endl;
            file << x1 << "," << y2 << "," << zlen << std::endl;
            file << x1 << "," << y2 << "," << zlen << std::endl;
            file << x2 << "," << y1 << "," << zlen << std::endl;
            file << x2 << "," << y2 << "," << zlen << std::endl;

            file << x1 << "," << y1 << "," << "0.0" << std::endl;  // Face by x, 0
            file << x1 << "," << y2 << "," << "0.0" << std::endl;
            file << x2 << "," << y2 << "," << "0.0" << std::endl;
            file << x2 << "," << y2 << "," << "0.0" << std::endl;
            file << x2 << "," << y1 << "," << "0.0" << std::endl;
            file << x1 << "," << y1 << "," << "0.0" << std::endl;

            file << "0.0" << "," << y1 << "," << z1 << std::endl;  // Face by z, 0
            file << "0.0" << "," << y1 << "," << z2 << std::endl;
            file << "0.0" << "," << y2 << "," << z1 << std::endl;
            file << "0.0" << "," << y2 << "," << z1 << std::endl;
            file << "0.0" << "," << y1 << "," << z2 << std::endl;
            file << "0.0" << "," << y2 << "," << z2 << std::endl;

            file << xlen << "," << y1 << "," << z1 << std::endl;  // Face by z, x
            file << xlen << "," << y2 << "," << z1 << std::endl;
            file << xlen << "," << y2 << "," << z2 << std::endl;
            file << xlen << "," << y2 << "," << z2 << std::endl;
            file << xlen << "," << y1 << "," << z2 << std::endl;
            file << xlen << "," << y1 << "," << z1 << std::endl;

            file << x1tb << "," << ylen << "," << z1 << std::endl;  // Top of box
            file << x1tb << "," << ylen << "," << z2 << std::endl;
            file << x2tb << "," << ylen << "," << z2 << std::endl;
            file << x2tb << "," << ylen << "," << z2 << std::endl;
            file << x2tb << "," << ylen << "," << z1 << std::endl;
            file << x1tb << "," << ylen << "," << z1 << std::endl;

            file << x1tb << "," << "0.0" << "," << z1 << std::endl;  // Bottom of box
            file << x2tb << "," << "0.0" << "," << z1 << std::endl;
            file << x1tb << "," << "0.0" << "," << z2 << std::endl;
            file << x1tb << "," << "0.0" << "," << z2 << std::endl;
            file << x2tb << "," << "0.0" << "," << z1 << std::endl;
            file << x2tb << "," << "0.0" << "," << z2 << std::endl;
        }
    }
    file.close();
}

void sphere (double radius, int slices, int stacks, char* filename)
{
    std::ofstream file;
    file.open(filename);
    file << slices * stacks * 6 << std::endl;
    for(int i = 0; i < slices; i++)
    {
        for(int j = 0; j < stacks; j++)
        {
            double a1 = (i*2*M_PI)/slices;
            double a2 = ((i+1)*2*M_PI)/slices;
            double b1 = (M_PI/2)-(j*M_PI/stacks);
            double b2 = (M_PI/2)-((j+1)*M_PI/stacks);
            double y1 = radius*sin(b1);
            double y2 = radius*sin(b2);
            double x1 = radius*cos(b1)*sin(a1);
            double x2 = radius*cos(b2)*sin(a1);
            double x3 = radius*cos(b2)*sin(a2);
            double x4 = radius*cos(b1)*sin(a2);
            double z1 = radius*cos(b1)*cos(a1);
            double z2 = radius*cos(b2)*cos(a1);
            double z3 = radius*cos(b2)*cos(a2);
            double z4 = radius*cos(b1)*cos(a2);

            file << x1 << "," << y1 << "," << z1 << std::endl;
            file << x2 << "," << y2 << "," << z2 << std::endl;
            file << x3 << "," << y2 << "," << z3 << std::endl;
            file << x4 << "," << y1 << "," << z4 << std::endl;
            file << x1 << "," << y1 << "," << z1 << std::endl;
            file << x3 << "," << y2 << "," << z3 << std::endl;
        }
    }
    file.close();
}

void cone(double radius, double height, int slices, int stacks, char* filename)
{
    std::ofstream file;
    file.open(filename);
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
            double x1 = r1*sin(a1);
            double x2 = r1*sin(a2);
            double x3 = r2*sin(a1);
            double x4 = r2*sin(a2);
            double z1 = r1*cos(a1);
            double z2 = r1*cos(a2);
            double z3 = r2*cos(a1);
            double z4 = r2*cos(a2);

            file << x1 << "," << y1 << "," << z1 << std::endl;
            file << x2 << "," << y1 << "," << z2 << std::endl;
            file << x3 << "," << y2 << "," << z3 << std::endl;
            file << x2 << "," << y1 << "," << z2 << std::endl;
            file << x4 << "," << y2 << "," << z4 << std::endl;
            file << x3 << "," << y2 << "," << z3 << std::endl;

            if(i==0)
            {
                file << x1 << "," << y1 << "," << z1 << std::endl;
                file << "0.0" << "," << y1 << "," << "0.0" << std::endl;
                file << x2 << "," << y1 << "," << z2 << std::endl;
            }
        }
    }
}

void multMatrixVector(float *m, float *v, float *res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }
}

void genPoints(float u, float v, float cpoints[4][4][3], float result[3])
{
    float um[4] = {u*u*u, u*u, u, 1};
    float vm[4] = {v*v*v, v*v, v, 1};
    float m[4][4] = {	{-1.0f,  3.0f, -3.0f,  1.0f},
                         { 3.0f, -6.0f,  3.0f, 0.0f},
                         {-3.0f,  3.0f,  0.0f,  0.0f},
                         { 1.0f,  0.0f,  0.0f,  0.0f}};
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

    for(int p = 0; p < numPatches; p++)
    {
        float from[4][4][3];
        for (int z = 0; z < 4; z++) {
            for (int x = 0; x < 4; x++) {
                for (int w = 0; w < 3; w++) {
                    from[z][x][w] = bezierPoints[p][z*4+x][w];
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
                genPoints(u, v, from, p1);
                genPoints(u, v2, from, p2);
                genPoints(u2, v, from, p3);
                genPoints(u2, v2, from, p4);

                output << p3[0] << "," << p3[1] << "," << p3[2] << std::endl;
                output << p4[0] << "," << p4[1] << "," << p4[2] << std::endl;
                output << p2[0] << "," << p2[1] << "," << p2[2] << std::endl;
                output << p2[0] << "," << p2[1] << "," << p2[2] << std::endl;
                output << p1[0] << "," << p1[1] << "," << p1[2] << std::endl;
                output << p3[0] << "," << p3[1] << "," << p3[2] << std::endl;

            }
        }
    }

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