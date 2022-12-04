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


int size; // problem size


int my_rank;
int world_size;


void initialize(float *data) {
    // intialize the temperature distribution
    int len = size * size;
    for (int i = 0; i < len; i++) {
        data[i] = wall_temp;
    }
}


void generate_fire_area(bool *fire_area){
    // generate the fire area
    int len = size * size;
    for (int i = 0; i < len; i++) {
        fire_area[i] = 0;
    }

    float fire1_r2 = fire_size * fire_size;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            int a = i - size / 2;
            int b = j - size / 2;
            int r2 = 0.5 * a * a + 0.8 * b * b - 0.5 * a * b;
            if (r2 < fire1_r2) fire_area[i * size + j] = 1;
        }
    }

    float fire2_r2 = (fire_size / 2) * (fire_size / 2);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            int a = i - 1 * size / 3;
            int b = j - 1 * size / 3;
            int r2 = a * a + b * b;
            if (r2 < fire2_r2) fire_area[i * size + j] = 1;
        }
    }
}


void update(float *data, float *new_data, int begin, int end) {
    // TODO: update the temperature of each point, and store the result in `new_data` to avoid data racing
    if (begin == 0)
    {
        begin = 1;
    }
    if (end == size)
    {
        end = size - 1;
    }
    for (int i = begin; i < end; i++){
        for (int j = 1; j < size - 1; j++){
            int idx = i * size + j;
            float up = data[idx - size];
            float down = data[idx + size];
            float left = data[idx - 1];
            float right = data[idx + 1];
            float new_val = (up + down + left + right) / 4;
            new_data[idx] = new_val;
        }
    }
}


void maintain_fire(float *data, bool* fire_area, int begin, int end) {
    // TODO: maintain the temperature of fire
    int len = size * size;
    for (int i = 0; i < len; i++){
        if (fire_area[i]) data[i] = fire_temp;
    }
}


void maintain_wall(float *data, int begin, int end) {
    // TODO: maintain the temperature of the wall
}


#ifdef GUI
void data2pixels(float *data, GLubyte* pixels){
    // convert rawdata (large, size^2) to pixels (small, resolution^2) for faster rendering speed
    float factor_data_pixel = (float) size / resolution;
    float factor_temp_color = (float) 255 / fire_temp;
    for (int x = 0; x < resolution; x++){
        for (int y = 0; y < resolution; y++){
            int idx = x * resolution + y;
            int idx_pixel = idx * 3;
            int x_raw = x * factor_data_pixel;
            int y_raw = y * factor_data_pixel;
            int idx_raw = y_raw * size + x_raw;
            float temp = data[idx_raw];
            int color =  ((int) temp / 5 * 5) * factor_temp_color;
            pixels[idx_pixel] = color;
            pixels[idx_pixel + 1] = 255 - color;
            pixels[idx_pixel + 2] = 255 - color;
        }
    }
}


// void data2pixels(float *data, GLubyte* pixels, int begin, int end){
//     // convert rawdata (large, size^2) to pixels (small, resolution^2) for faster rendering speed
//     float factor_data_pixel = (float) size / resolution;
//     float factor_temp_color = (float) 255 / fire_temp;
//     for (int x = 0; x < resolution; x++){
//         for (int y = 0; y < resolution; y++){
//             int idx = x * resolution + y;
//             int idx_pixel = idx * 3;
//             int x_raw = x * factor_data_pixel;
//             int y_raw = y * factor_data_pixel;
//             int idx_raw = y_raw * size + x_raw;
//             float temp = data[idx_raw];
//             int color =  ((int) temp / 5 * 5) * factor_temp_color;
//             pixels[idx_pixel] = color;
//             pixels[idx_pixel + 1] = 255 - color;
//             pixels[idx_pixel + 2] = 255 - color;
//         }
//     }
// }

void plot(GLubyte* pixels){
    // visualize temprature distribution
    #ifdef GUI
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(resolution, resolution, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glutSwapBuffers();
    #endif
}
#endif



void slave(){
    // TODO: MPI routine (one possible solution, you can use another partition method)
    int my_begin_row_id = size * my_rank / (world_size);
    // size: 1000 rank:20 begin_id:0, 50, 100, ..., 950
    int my_end_row_id = size * (my_rank + 1) / world_size;
    int sub_problem_num = size / world_size;
    // size: 1000 rank:20 end_id:50, 100, 150, ..., 1000

    // TODO: Initialize a storage for temperature distribution of this area
    float* local_data_odd;
    float* local_data_even;
    float* sub_local_data;
    sub_local_data = new float[sub_problem_num * size];
    local_data_odd = new float[size * size];
    local_data_even = new float[size * size];
    bool* fire_area = new bool[size * size];

    MPI_Bcast(local_data_odd, size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(local_data_even, size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    // MPI_Bcast(fire_area, size*size, MPI_BOOL, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    
    // TODO: Receive initial temperature distribution of this area from master


    // TODO: Initialize a storage for local pixels (pls refer to sequential version for initialization of GLubyte)
    #ifdef GUI
    GLubyte* local_pixcels;
    #endif

    bool cont = true;
    while (cont) {
        // TODO: computation part
        
        // TODO: after computation, send border row to neighbours

        #ifdef GUI
        // TODO: conver raw temperature to pixels (much smaller than raw data)

        // TODO: send pixels to master (you can use MPI_Byte to transfer anything to master, then you won't need to declare MPI Type :-) )

        #endif

    }

    #ifdef GUI
    // data2pixels(local_data, local_pixcels);
    #endif

    // TODO: Remember to delete[] local_data and local_pixcels.
}



void master() {
    // TODO: MPI routine (one possible solution, you can use another partition method)
    float* data_odd = new float[size * size];
    float* data_even = new float[size * size];
    bool* fire_area = new bool[size * size];
    int my_begin_row_id = size * my_rank / (world_size);
    // size: 1000 rank:20 begin_id:0, 50, 100, ..., 950
    int my_end_row_id = size * (my_rank + 1) / world_size;

    int sub_problem_num = size / world_size;
    float* sub_local_data;
    sub_local_data = new float[sub_problem_num * size];
    printf("rank:%d, Step4\n", my_rank);


    initialize(data_odd);
    generate_fire_area(fire_area);


    #ifdef GUI
    GLubyte* pixels;
    pixels = new GLubyte[resolution * resolution * 3];
    #endif
    
    printf("rank:%d, Step5\n", my_rank);
    // MPI_Bcast(data_odd, size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    // MPI_Bcast(data_even, size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    printf("rank:%d, Step6\n", my_rank);
    // MPI_Bcast(fire_area, size*size, MPI_BOOL, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("rank:%d, Step7\n", my_rank);

    int count = 1;
    double total_time = 0;

    // TODO: Send initial distribution to each slave process
    std::chrono::high_resolution_clock::time_point t1;

    while (true) {
        if (my_rank == 0)
        {
            t1 = std::chrono::high_resolution_clock::now();   
        }

        // TODO: Computation of my part
        int local_idx = 0;
        if (count % 2 == 1) {
            update(data_odd, data_even, my_begin_row_id, my_end_row_id);
            maintain_fire(data_even, fire_area, my_begin_row_id, my_end_row_id);
            maintain_wall(data_even, my_begin_row_id, my_end_row_id);
            local_idx = 0;
            for (int i = my_begin_row_id; i < my_end_row_id; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    int idx = i * size + j;
                    sub_local_data[local_idx] = data_even[idx];
                }   
            }
            printf("rank:%d, Step8\n", my_rank);
            MPI_Barrier(MPI_COMM_WORLD);
            printf("rank:%d, Step10\n", my_rank);
            MPI_Gather(sub_local_data, sub_problem_num * size, MPI_FLOAT, data_even, sub_problem_num * size, MPI_FLOAT, 0, MPI_COMM_WORLD);
            printf("rank:%d, Step9\n", my_rank);
            MPI_Bcast(data_odd, size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);
            MPI_Bcast(data_even, size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);
        } else {
            update(data_even, data_odd, my_begin_row_id, my_end_row_id);
            maintain_fire(data_odd, fire_area, my_begin_row_id, my_end_row_id);
            maintain_wall(data_odd, my_begin_row_id, my_end_row_id);
            local_idx = 0;
            for (int i = my_begin_row_id; i < my_end_row_id; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    int idx = i * size + j;
                    sub_local_data[local_idx] = data_odd[idx];
                }   
            }
            MPI_Gather(sub_local_data, sub_problem_num * size, MPI_FLOAT, data_odd, sub_problem_num * size, MPI_FLOAT, 0, MPI_COMM_WORLD);
            MPI_Bcast(data_odd, size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);
            MPI_Bcast(data_even, size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);
        }
        // MPI_Gather(sub_local_data, sub_problem_num * size, MPI_FLOAT, data_even, sub_problem_num * size, MPI_FLOAT, 0, MPI_COMM_WORLD);

        // TODO: Send border row to neighbours
        if (my_rank == 0)
        {
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            double this_time = std::chrono::duration<double>(t2 - t1).count();
            total_time += this_time;
            printf("Iteration %d, elapsed time: %.6f\n", count, this_time);  
        }
        count++;
        if (my_rank == 0)
        {
            #ifdef GUI
            if (count % 2 == 1) {
                // TODO: Gather pixels of slave processes
                data2pixels(data_even, pixels);
            } else {
                // TODO: Gather pixels of slave processes
                data2pixels(data_odd, pixels);
            }
            plot(pixels);
            #endif
        }
    }

    delete[] data_odd;
    delete[] data_even;
    delete[] fire_area;

    if (my_rank == 0)
    {
        #ifdef GUI
        delete[] pixels;
        #endif
    }
}




int main(int argc, char *argv[]) {
    size = atoi(argv[1]);

	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    printf("rank:%d, Step1\n", my_rank);


	if (my_rank == 0) {
        #ifdef GUI
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
        glutInitWindowPosition(0, 0);
        glutInitWindowSize(resolution, resolution);
        glutCreateWindow("Heat Distribution Simulation Sequential Implementation");
        gluOrtho2D(0, resolution, 0, resolution);
        #endif
        printf("rank:%d, Step2\n", my_rank);
        master();
        printf("rank:%d, Step3\n", my_rank);
	} else {
        master();
    }

	if (my_rank == 0){
		printf("Student ID: 119010434\n"); // replace it with your student id
		printf("Name: Zhang Qihang\n"); // replace it with your name
		printf("Assignment 4: Heat Distribution Simulation MPI Implementation\n");
	}

	MPI_Finalize();

	return 0;
}

