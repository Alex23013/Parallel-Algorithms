#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int output(int in_val, int my_rank, int p, MPI_Comm comm);

int main(int argc, char* argv[]) {
   int p, my_rank;
   MPI_Comm comm;
   int in_val=0;
   double my_start, my_finish, my_elapsed;

   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);

   in_val=output(in_val, my_rank, p, comm);

   MPI_Finalize();
   return 0;
}  /* main */


int output(int in_val, int my_rank, int p, MPI_Comm comm) {

   int n=0;
   //printf("Proceso %d\n", my_rank);
   if(my_rank !=0){

	//printf("REcibiendo...\n");
	MPI_Recv(&in_val, 1, MPI_INT, my_rank - 1 , 0, comm, MPI_STATUS_IGNORE);
        printf("REcibi\n");
	printf("Proc %d > Imprimo\n", my_rank);
	if(my_rank +1<p) MPI_Send(&in_val, 1, MPI_INT, my_rank +1, 0, comm);
   }
   else {
	//printf("Envio %d\n", in_val);
        MPI_Send(&in_val, 1, MPI_INT, my_rank+1, 0, comm);
   }

   return in_val;

}
