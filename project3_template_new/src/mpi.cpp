#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <mpi.h>

#ifdef GUI
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "./headers/physics.h"
#include "./headers/logger.h"


int n_body;
int n_iteration;


int my_rank;
int world_size;

std::chrono::high_resolution_clock::time_point t1,t2;


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


void update_position(double *x, double *y, double *vx, double *vy, int n) {
    //TODO: update position 
    for (int i = 0; i < n; i++)
    {
        x[i] = x[i] + vx[i] * dt;
        y[i] = y[i] + vy[i] * dt;
    }
}


void update_velocity(double *m, double *x, double *y,double *x_total, double *y_total, double *vx, double *vy, int n) {
    //TODO: calculate force and acceleration, update velocity
    for (int i = 0; i < n; i++)
    {
        double acceleration = 0;
        double acceleration_x = 0;
        double acceleration_y = 0;
        double x_proj, y_proj, xy_distance_pow;
        if (x[i] > bound_x || x[i] < 0. || y[i] > bound_y || y[i] < 0.)
        {
            vx[i] = -vx[i];
            vy[i] = -vy[i];
        }
        for (int j = 0; j < n*world_size; j++)
        {
            if (i != j)
            {
                xy_distance_pow = pow(x[i] - x_total[j], 2.) + pow(y[i] - y_total[j], 2.);
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
                    x_proj = pow(pow(x[i] - x_total[j], 2.) / xy_distance_pow, 0.5);
                    y_proj = pow(pow(y[i] - y_total[j], 2.) / xy_distance_pow, 0.5);
                    acceleration = gravity_const * m[j] / (xy_distance_pow + err);
                    if (x[i] < x_total[j])
                    {
                        acceleration_x = acceleration_x + acceleration * x_proj;
                    } else {
                        acceleration_x = acceleration_x - acceleration * x_proj;
                    }
                    if (y[i] < y_total[j])
                    {
                        acceleration_y = acceleration_y + acceleration * y_proj;
                    } else {
                        acceleration_y = acceleration_y - acceleration * y_proj;
                    }
                }
            }
        }
        vx[i] = vx[i] + acceleration_x * dt;
        vy[i] = vy[i] + acceleration_y * dt;
    }
}


int main(int argc, char *argv[]) {
    n_body = atoi(argv[1]);
    n_iteration = atoi(argv[2]);

	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int n_ture;
    if (n_body % world_size != 0)
    {
        n_ture = n_body + world_size - (n_body % world_size);
    }
    else {
        n_ture = n_body;
    }
    int point_per_process = n_ture / world_size;

    double* m = new double[n_ture];
    double* x = new double[n_ture];
    double* y = new double[n_ture];
    double* vx = new double[n_ture];
    double* vy = new double[n_ture];
    Logger l = Logger("mpi", n_body, bound_x, bound_y);
    if (my_rank == 0)
    {
        generate_data(m, x, y, vx, vy, n_ture);
    }
    double* local_m = new double[point_per_process];
    double* local_x = new double[point_per_process];
    double* local_y = new double[point_per_process];
    double* local_vx = new double[point_per_process];
    double* local_vy = new double[point_per_process];
    MPI_Scatter(vx, point_per_process, MPI_DOUBLE, local_vx, point_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(vy, point_per_process, MPI_DOUBLE, local_vy, point_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(x, point_per_process, MPI_DOUBLE, local_x, point_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(y, point_per_process, MPI_DOUBLE, local_y, point_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(m, point_per_process, MPI_DOUBLE, local_m, point_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(m, n_ture, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    for (int i = 0; i < n_iteration; i++)
    {
        if (my_rank == 0)
        {
            t1 = std::chrono::high_resolution_clock::now();

            // std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> time_span = t2 - t1;
        }

        MPI_Bcast(x, n_ture, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(y, n_ture, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        update_velocity(m, local_x, local_y, x, y, local_vx, local_vy, point_per_process);
        MPI_Barrier(MPI_COMM_WORLD);
        update_position(local_x, local_y, local_vx, local_vy, point_per_process);
        MPI_Barrier(MPI_COMM_WORLD);
        
        MPI_Gather(local_x, point_per_process, MPI_DOUBLE, x, point_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Gather(local_y, point_per_process, MPI_DOUBLE, y, point_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);


        if (my_rank == 0)
        {
            t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time_span = t2 - t1;
            printf("Iteration %d, elapsed time: %.8f\n", i, time_span);
            l.save_frame(x, y);
        }
        
        MPI_Barrier(MPI_COMM_WORLD);
    }
    



    

    

	if (my_rank == 0) {
		#ifdef GUI
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(500, 500); 
		glutInitWindowPosition(0, 0);
		glutCreateWindow("N Body Simulation MPI Implementation");
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glMatrixMode(GL_PROJECTION);
		gluOrtho2D(0, bound_x, 0, bound_y);
		#endif
        // master();
	} else {
        // slave();
    }

	if (my_rank == 0){
		printf("Student ID: 119010434\n"); // replace it with your student id
		printf("Name: Zhang Qihang\n"); // replace it with your name
		printf("Assignment 3: N Body Simulation MPI Implementation\n");
	}

	MPI_Finalize();

	return 0;
}

