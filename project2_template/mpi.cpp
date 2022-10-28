#include "asg2.h"
#include <stdio.h>
#include <mpi.h>


int rank;
int world_size;
int max_compute_per_p;


void master() {
	//TODO: procedure run in master process
	// MPI_Scatter...
	// MPI_Gather...
	// the following code is not a necessary, please replace it with MPI implementation.
	Point* p = data;
	for (int i = 0; i < total_size; i = i + world_size)
	{
		compute(p);
		for (int j = 0; j < world_size; j++)
		{
			p++;
		}
	}
	//TODO END

}


void slave() {
	//TODO: procedure run in slave process
	// MPI_Scatter...
	// MPI_Gather...
	Point* p = data;
	for (int i = 0; i < rank; i++)
	{
		p++;
	}
	for (int i = rank; i < total_size; i = i + world_size)
	{
		compute(p);
		for (int j = 0; j < world_size; j++)
		{
			p++;
		}
	}
	//TODO END
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
		#endif
	}

	/* computation part begin */
	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	max_compute_per_p = (total_size + world_size - 1) / world_size;

	// int all_flag[world_size];
	// void *point_all_flag = &all_flag;
	int my_flag;
	void *point_flag = &my_flag;

	if (rank == 0) {
		t1 = std::chrono::high_resolution_clock::now();

		initData();
		master();
		for (int i = 1; i < world_size; i++)
		{
			MPI_Recv(point_flag, 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
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
		slave();
		my_flag = 1; //slave compute over!
		MPI_Send(point_flag, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	/* computation part end */

	if (rank == 0){
		#ifdef GUI
		glutMainLoop();
		#endif
	}

	return 0;
}

