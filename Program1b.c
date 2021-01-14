#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#define SEED 3655942

int main(int argc, char **argv){
	int iter = 100000000;
    int p;
	int i, reduced_count,reduced_iter, count = 0;
	int rank;
	double pi,x,y,z,t;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size( MPI_COMM_WORLD, &p );
	iter=iter/(p-1);
	if(rank==0){
		t = MPI_Wtime();
	}
	if(rank>0){
		srand(SEED+rank*10);
		for(i=0;i<iter;i++){
			x = (double)rand()/RAND_MAX;
			y = (double)rand()/RAND_MAX;
			z = x*x + y*y;
			if(z<=1) count++;
		}
	}
	MPI_Reduce(&count,&reduced_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Reduce(&iter,&reduced_iter,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	reduced_iter = reduced_iter - iter;
	if(rank==0){
		t = MPI_Wtime() - t;
		pi = (double)reduced_count/(double)reduced_iter * 4;
		printf("Pi : %lf Time: %lf Iterations: %d\n",pi,t,reduced_iter);
	}
	MPI_Finalize();
	return 0;
}
