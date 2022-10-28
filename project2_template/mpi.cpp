#include "asg2.h"
#include <stdio.h>
#include <mpi.h>



int rank;
int world_size;


void master() {
	Point* p = data;
	for (int i = 0; i < total_size; i = i + world_size)
	{	
		compute(p);
		for (int j = 0; j < world_size; j++)
		{
			p++;
		}
	}
}


void slave(int total_s) {
	Point* p = data;
	for (int i = 0; i < rank; i++)
	{
		p++;
	}
	for (int i = rank; i < total_s; i = i + world_size)
	{
		compute(p);
		for (int j = 0; j < world_size; j++)
		{
			p++;
		}
	}
}

float get_color(int px, int py, int x_max, int y_max, int max_itera) {
	float z_real, z_imag, c_real, c_imag;
	float lengthsq, temp;
	int k;
	float final_color;

	/* scale [0, X_RESN] x [0, Y_RESN] to [-1, 1] x [-1, 1] */
	c_real = ((float) px - x_max / 2) / (x_max / 2);
	c_imag = ((float) py - y_max / 2) / (y_max / 2);

	/* the following block is about math. */ 

	z_real = z_imag = 0.0;
    k = 0;

	do { 
		temp = z_real*z_real - z_imag*z_imag + c_real;
		z_imag = 2.0*z_real*z_imag + c_imag;
		z_real = temp;
		lengthsq = z_real*z_real+z_imag*z_imag;
		k++;
	} while (lengthsq < 4.0 && k < max_itera);

	/* math block end */ 

	final_color = (float) k / max_itera;

	return final_color;
}


int main(int argc, char *argv[]) {
	if ( argc == 4 ) {
		X_RESN = atoi(argv[1]);
		Y_RESN = atoi(argv[2]);
		max_iteration = atoi(argv[3]);
	} else {
		X_RESN = 1000;
		Y_RESN = 1000;
		max_iteration = 100;
	}

	/* computation part begin */
	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int my_flag;
	int total_pixel;

	void *point_flag = &my_flag;

	printf("X:%d\n",X_RESN);
	printf("%d\n",max_iteration);
	int x_array[X_RESN * Y_RESN];
	int y_array[X_RESN * Y_RESN];
	float color_array[X_RESN * Y_RESN];
	int pixels_per_process = (X_RESN * Y_RESN + world_size - 1) / world_size;

	float sub_color_array[pixels_per_process];
	float max_color_array[pixels_per_process * world_size];

	for (int i = 0; i < X_RESN; i++)
	{
		for (int j = 0; j < Y_RESN; j++)
		{
			x_array[i * X_RESN + j] = i;
			y_array[i * Y_RESN + j] = j;
		}
	}
	if (rank != world_size-1)
	{
		int start_point = rank * pixels_per_process;
		for (int i = 0; i < pixels_per_process; i++)
		{
			sub_color_array[i] = get_color(x_array[i + start_point], y_array[i + start_point], X_RESN, Y_RESN, max_iteration);
		}
		if (rank == 0)
		{
			for (int i = 1; i < world_size; i++)
			{
				MPI_Recv(point_flag, 1, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		} else {
			my_flag = rank;
			MPI_Send(point_flag, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		}
	} else {
		int start_point = rank * pixels_per_process;
		for (int i = 0; i < X_RESN * Y_RESN - start_point; i++)
		{
			sub_color_array[i] = get_color(x_array[i + start_point], y_array[i + start_point], X_RESN, Y_RESN, max_iteration);
		}
		if (rank != 0)
		{
			my_flag = rank;
			MPI_Send(point_flag, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		}
	}
	MPI_Gather(void* send_data,int send_count,MPI_Datatype send_datatype,void* recv_data,int recv_count,MPI_Datatype recv_datatype,int root,MPI_Comm communicator);
	// gather to max_color_array
	if (rank == 0)
	{
		Point* p = data;
		for (int index = 0; index < X_RESN * Y_RESN; index++){
			p->x = x_array[index];
			p->y = y_array[index];
			p->color = max_color_array[i];
			p++;
		}
	}
	
	
	for (rank i = 0; i < count; i++)
	{
		/* code */
	}
	
	

	void *point_flag = &my_flag;
	void *point_total_size = &total_pixel;
	if (rank == 0) {
		t1 = std::chrono::high_resolution_clock::now();
		initData();
		Point* p1 = data;
		for (int i = 0; i < X_RESN*Y_RESN; i++)
		{
			x_array[i] = p1->x;
			y_array[i] = p1->y;
		}
		
		total_pixel = total_size;
		printf("+++++%d, %d++++++\n", total_size, total_pixel);

		for (int i = 1; i < world_size; i++)
		{
			MPI_Send(point_total_size, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
		
		master();
		for (int i = 1; i < world_size; i++)
		{
			MPI_Recv(point_flag, 1, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// printf("%d/%d is over\n", my_flag, world_size);
		}

		t2 = std::chrono::high_resolution_clock::now();  
		time_span = t2 - t1;

		printf("Student ID: 119010001\n"); // replace it with your student id
		printf("Name: Your Name\n"); // replace it with your name
		printf("Assignment 2 MPI\n");
		printf("Run Time: %f seconds\n", time_span.count());
		printf("Problem Size: %d * %d, %d\n", X_RESN, Y_RESN, max_iteration);
		printf("Process Number: %d\n", world_size);
		
	} else {
		MPI_Recv(point_total_size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("))))))))%d\n", total_pixel);
		slave(total_pixel);
		my_flag = rank; //slave compute over!
		MPI_Send(point_flag, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
	}
	printf("%d,%d , rank:%d\n", total_size,total_pixel, rank);


	if (rank == 0) {
		#ifdef GUI
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(500, 500); 
		glutInitWindowPosition(0, 0);
		glutCreateWindow("MPI");
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glMatrixMode(GL_PROJECTION);
		gluOrtho2D(0, X_RESN, 0, Y_RESN);
		glutDisplayFunc(plot);
		glutMainLoop();
		#endif
	}

	if (rank == 0){
		#ifdef GUI
		#endif
	}

	MPI_Finalize();
	
	return 0;
}

