#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <pthread.h>

#ifdef GUI
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "./headers/physics.h"
#include "./headers/logger.h"

int n_thd; // number of threads

int n_body;
int n_iteration;

// double* m = new double[n_body];
// double* x = new double[n_body];
// double* y = new double[n_body];
// double* vx = new double[n_body];
// double* vy = new double[n_body];


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



void update_position(double *x, double *y, double *vx, double *vy, int n, int index) {
    //TODO: update position 
    x[index] = x[index] + vx[index] * dt;
    y[index] = y[index] + vy[index] * dt;
}

void update_velocity(double *m, double *x, double *y, double *vx, double *vy, int n, int index) {
    //TODO: calculate force and acceleration, update velocity
    double acceleration = 0;
    double acceleration_x = 0;
    double acceleration_y = 0;
    // double origin_vx, origin_vy;
    double x_proj, y_proj, xy_distance_pow;
    // printf("This is pthread %d\n", n);
    if (x[index] > bound_x || x[index] < 0. || y[index] > bound_y || y[index] < 0.)
    {
        vx[index] = -vx[index];
        vy[index] = -vy[index];
    }
    for (int j = 0; j < n; j++)
    {
        if (index != j)
        {
            xy_distance_pow = pow(x[index] - x[j], 2.) + pow(y[index] - y[j], 2.);
            // xy_distance_pow = (x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]);
            // printf("ans111:%f,\n", pow(x[i] - x[j], 2.));
            // printf("ans2:%f,\n", pow(y[i] - y[j], 2.));
            // printf("xi:%f, xj:%f, yi:%f, yj:%f\n", x[i], x[j], y[i], y[j]);
            // printf("xy_dis:%f,\n", xy_distance_pow);
            if (xy_distance_pow < 1000 * radius2)
            {
                vx[index] = -vx[index];
                vy[index] = -vy[index];
                acceleration_x = 0;
                acceleration_y = 0;
                break;
            }
            else
            {
                x_proj = pow(pow(x[index] - x[j], 2.) / xy_distance_pow, 0.5);
                y_proj = pow(pow(y[index] - y[j], 2.) / xy_distance_pow, 0.5);
                acceleration = gravity_const * m[j] / (xy_distance_pow + err);
                // printf("acc:%f, xP:%f, yP:%f\n", acceleration, x_proj, y_proj);
                if (x[index] < x[j])
                {
                    acceleration_x = acceleration_x + acceleration * x_proj;
                } else {
                    acceleration_x = acceleration_x - acceleration * x_proj;
                }
                if (y[index] < y[j])
                {
                    acceleration_y = acceleration_y + acceleration * y_proj;
                } else {
                    acceleration_y = acceleration_y - acceleration * y_proj;
                }
            }
        }
    }
    // printf("acc_X:%f, acc_Y%f\n", acceleration_x, acceleration_y);
    vx[index] = vx[index] + acceleration_x * dt;
    vy[index] = vy[index] + acceleration_y * dt;
    // printf("v_X:%f, v_Y%f\n\n", vx[i], vy[i]);
}


typedef struct {
    //TODO: specify your arguments for threads
    int a;
    int b;
    double* m_s = new double[n_body];
    double* x_s = new double[n_body];
    double* y_s = new double[n_body];
    double* vx_s = new double[n_body];
    double* vy_s = new double[n_body];
    //TODO END
} Args;


void* worker(void* args) {
    //TODO: procedure in each threads

    Args* my_arg = (Args*) args;
    int a = my_arg->a; //现在所处的thread
    int b = my_arg->b; //thread的数量
    double* m = my_arg->m_s;
    double* x = my_arg->x_s;
    double* y = my_arg->y_s;
    double* vx = my_arg->vx_s;
    double* vy = my_arg->vy_s;
    // printf("This is pthread %d/%d\n", a, b);
    for (int i = a; i < n_body; i = i + b)
    {
        // printf("This is pthread %d\n", i);
        // printf("This is pthread %d/%d, current body:%d/%d\n", a, b, i, n_body);
        update_velocity(m, x, y, vx, vy, n_body, i);
        update_position(x, y, vx, vy, n_body, i);
    }
    

    // TODO END
}


void master(){
    double* m = new double[n_body];
    double* x = new double[n_body];
    double* y = new double[n_body];
    double* vx = new double[n_body];
    double* vy = new double[n_body];

    // printf("Generate is start\n!");
    generate_data(m, x, y, vx, vy, n_body);
    // printf("Generate is over\n!");
    // printf("logger is start\n!");
    Logger l = Logger("sequential", n_body, bound_x, bound_y);
    // printf("logger is over\n!");
    for (int i = 0; i < n_iteration; i++){
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        //TODO: assign jobs
        //TODO: assign jobs
        // printf("Assign is start\n!");
        pthread_t thds[n_thd]; // thread pool
        Args args[n_thd]; // arguments for all threads
        for (int thd = 0; thd < n_thd; thd++){
            args[thd].a = thd;
            args[thd].b = n_thd;
            args[thd].m_s = m;
            args[thd].x_s = x;
            args[thd].y_s = y;
            args[thd].vx_s = vx;
            args[thd].vy_s = vy;
        }
        // printf("Assign is over\n!");
        // printf("Work is start\n!");
        for (int thd = 0; thd < n_thd; thd++) pthread_create(&thds[thd], NULL, worker, &args[thd]);
        // printf("Join is start\n!");
        for (int thd = 0; thd < n_thd; thd++) pthread_join(thds[thd], NULL);
        // printf("Work is over\n!");
        //TODO END
        
        //TODO End

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = t2 - t1;

        printf("Iteration %d, elapsed time: %.8f\n", i, time_span);

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
            // xi = 2000.;
            yi = y[i];
            // yi = 2000.;
            glVertex2f(xi, yi);
            // printf("xi:%f, yi:%f, body:%d\n", xi, yi, i);
        }
        glEnd();
        glFlush();
        glutSwapBuffers();
        #else

        #endif
    }

    // delete[] m;
    // delete[] x;
    // delete[] y;
    // delete[] vx;
    // delete[] vy;


}


int main(int argc, char *argv[]) {
    n_body = atoi(argv[1]);
    n_iteration = atoi(argv[2]);
    n_thd = atoi(argv[3]);
    // printf("TASK is start\n!");

    #ifdef GUI
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Pthread");
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, bound_x, 0, bound_y);
    #endif
    master();
    printf("TASK is over!");

	return 0;
}

