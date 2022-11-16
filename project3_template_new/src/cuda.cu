#include <cuda.h>
#include <cuda_runtime.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#ifdef GUI
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "./headers/physics.h"
#include "./headers/logger.h"


int block_size = 512;


int n_body;
int n_iteration;


__global__ void update_position(double *x, double *y, double *vx, double *vy, int n) {
    //TODO: update position 
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < n) {
        x[i] = x[i] + vx[i] * dt;
        y[i] = y[i] + vy[i] * dt;
    }
}

__global__ void update_velocity(double *m, double *x, double *y, double *vx, double *vy, int n) {
    //TODO: calculate force and acceleration, update velocity
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < n) {  
        double acceleration = 0;
        double acceleration_x = 0;
        double acceleration_y = 0;
        // double origin_vx, origin_vy;
        double x_proj, y_proj, xy_distance_pow;
        if (x[i] > bound_x || x[i] < 0. || y[i] > bound_y || y[i] < 0.)
        {
            vx[i] = -vx[i];
            vy[i] = -vy[i];
            // printf("OVER HERE #1\n");
        }
        // printf("OVER HERE #1\n");
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                xy_distance_pow = pow(x[i] - x[j], 2.) + pow(y[i] - y[j], 2.);
                // xy_distance_pow = (x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]);
                // printf("ans111:%f,\n", pow(x[i] - x[j], 2.));
                // printf("ans2:%f,\n", pow(y[i] - y[j], 2.));
                // printf("xi:%f, xj:%f, yi:%f, yj:%f\n", x[i], x[j], y[i], y[j]);
                // printf("xy_dis:%f,\n", xy_distance_pow);
                if (xy_distance_pow < 1000 * radius2)
                {
                    vx[i] = -vx[i];
                    vy[i] = -vy[i];
                    acceleration_x = 0;
                    acceleration_y = 0;
                    break;
                }
                else
                {
                    x_proj = pow(pow(x[i] - x[j], 2.) / xy_distance_pow, 0.5);
                    y_proj = pow(pow(y[i] - y[j], 2.) / xy_distance_pow, 0.5);
                    acceleration = gravity_const * m[j] / (xy_distance_pow + err);
                    // printf("acc:%f, xP:%f, yP:%f\n", acceleration, x_proj, y_proj);
                    if (x[i] < x[j])
                    {
                        acceleration_x = acceleration_x + acceleration * x_proj;
                    } else {
                        acceleration_x = acceleration_x - acceleration * x_proj;
                    }
                    if (y[i] < y[j])
                    {
                        acceleration_y = acceleration_y + acceleration * y_proj;
                    } else {
                        acceleration_y = acceleration_y - acceleration * y_proj;
                    }
                }
            }
        }
        // printf("acc_X:%f, acc_Y%f\n", acceleration_x, acceleration_y);
        vx[i] = vx[i] + acceleration_x * dt;
        vy[i] = vy[i] + acceleration_y * dt;
        // printf("v_X:%f, v_Y%f\n\n", vx[i], vy[i]);
    }
}


void generate_data(double *m, double *x,double *y,double *vx,double *vy, int n) {
    // TODO: Generate proper initial position and mass for better visualization
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) {
        m[i] = rand() % max_mass + 1.0f;
        x[i] = 2000.0f + rand() % (bound_x / 4);
        y[i] = 2000.0f + rand() % (bound_y / 4);
        vx[i] = 0.0f;
        vy[i] = 0.0f;
    }
}



void master() {
    double* m = new double[n_body];
    double* x = new double[n_body];
    double* y = new double[n_body];
    double* vx = new double[n_body];
    double* vy = new double[n_body];

    generate_data(m, x, y, vx, vy, n_body);

    Logger l = Logger("cuda", n_body, bound_x, bound_y);

    double *device_m;
    double *device_x;
    double *device_y;
    double *device_vx;
    double *device_vy;

    cudaMalloc(&device_m, n_body * sizeof(double));
    cudaMalloc(&device_x, n_body * sizeof(double));
    cudaMalloc(&device_y, n_body * sizeof(double));
    cudaMalloc(&device_vx, n_body * sizeof(double));
    cudaMalloc(&device_vy, n_body * sizeof(double));

    cudaMemcpy(device_m, m, n_body * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(device_x, x, n_body * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(device_y, y, n_body * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(device_vx, vx, n_body * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(device_vy, vy, n_body * sizeof(double), cudaMemcpyHostToDevice);

    int n_block = n_body / block_size + 1;

    for (int i = 0; i < n_iteration; i++){
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        update_velocity<<<n_block, block_size>>>(device_m, device_x, device_y, device_vx, device_vy, n_body);
        update_position<<<n_block, block_size>>>(device_x, device_y, device_vx, device_vy, n_body);

        cudaMemcpy(x, device_x, n_body * sizeof(double), cudaMemcpyDeviceToHost);
        cudaMemcpy(y, device_y, n_body * sizeof(double), cudaMemcpyDeviceToHost);

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = t2 - t1;
        
        printf("Iteration %d, elapsed time: %.3f\n", i, time_span);

        l.save_frame(x, y);

        #ifdef GUI
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(2.0f);
        glBegin(GL_POINTS);
        double xi;
        double yi;
        for (int i = 0; i < n_body; i++){
            xi = x[i];
            yi = y[i];
            glVertex2f(xi, yi);
        }
        glEnd();
        glFlush();
        glutSwapBuffers();
        #else

        #endif

    }

    cudaFree(device_m);
    cudaFree(device_x);
    cudaFree(device_y);
    cudaFree(device_vx);
    cudaFree(device_vy);

    delete[] m;
    delete[] x;
    delete[] y;
    delete[] vx;
    delete[] vy;
    
}


int main(int argc, char *argv[]){
    
    n_body = atoi(argv[1]);
    n_iteration = atoi(argv[2]);

    #ifdef GUI
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500, 500);
    glutCreateWindow("N Body Simulation CUDA Implementation");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gluOrtho2D(0, bound_x, 0, bound_y);
    #endif

    master();

    printf("Student ID: 119010001\n"); // replace it with your student id
    printf("Name: Your Name\n"); // replace it with your name
    printf("Assignment 2: N Body Simulation CUDA Implementation\n");

    return 0;

}


