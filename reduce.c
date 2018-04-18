#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int Reduce(int my_val, int in_val, int my_rank, int root, int p, MPI_Comm comm);

int main(int argc, char* argv[]) {
   
   int p, my_rank, my_val;
   MPI_Comm comm;
   int result, in_val;

   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);

   //shared double global_elapsed;
   double my_start, my_finish, my_elapsed;
      //printf("Proc %d > Enter an int\n", my_rank);
      //scanf("%d", &my_val);
   my_val=p-my_rank;   
   in_val=my_val;

   MPI_Barrier(MPI_COMM_WORLD); 
   my_start=MPI_Wtime();
   
   //result = Reduce(my_val, in_val, my_rank, 5, p, comm);
   MPI_Reduce(&my_val, &in_val, 1, MPI_INT, MPI_SUM, 5,MPI_COMM_WORLD);
   MPI_Barrier(MPI_COMM_WORLD); 
   my_finish=MPI_Wtime();
   my_elapsed=my_finish-my_start;   

   //global_elapsed=Global_max(my_elapsed);

   if (my_rank == 5) {
	//printf("Proc %d > result = %d, time = %f\n", my_rank, result, my_elapsed);
        printf("Proc %d > result = %d, time = %f\n", my_rank, in_val, my_elapsed);
	
   }

   MPI_Finalize();
   return 0;
}  /* main */


/*-----------------------------------------------------------------*/
/* Function:    Reduce
 *           r     001 010 100 
 *           -     --- --- ---
 *           0 000 001 010 100  
 *           1 001 000 011 101
 *           2 010  x  000 110
 *           3 011  x  001 111
 *           4 100  x   x  000
 *           5 101  x   x  001
 *           6 110  x   x  010
 *           7 111  x   x  011
 */
int Reduce(int my_val, int in_val, int my_rank, int root, int p, MPI_Comm comm) {
   int        partner, partner1, aux, acum;
   unsigned   bitmask = 1;
   while (bitmask < p) bitmask <<= 1;
   bitmask >>= 1;
   int        participate = bitmask << 1;

//#  ifdef DEBUG
   printf("Proc %d > bitmask = %d\n", my_rank, bitmask);
   fflush(stdout);
//#  endif
   while (bitmask > 0) {
   aux=my_rank-root;
   if (aux<0) aux+=p;   
   if (aux < participate) {
	 
         partner = ((aux ^ bitmask)+root)%p;
         partner1 = aux ^ bitmask;
//#        ifdef DEBUG
         printf("Proc %d > bitmask = %d, in_val = %d, partner = %d, aux = %d\n",
            my_rank, bitmask, in_val, partner,aux);
         fflush(stdout);
//#        endif
         if (aux > partner1) {
            if (partner < p ){
	       printf("Proc %d, envio %d\n", my_rank, in_val);
               MPI_Send(&in_val, 1, MPI_INT, partner, 0, comm);
            }
         } else {
            MPI_Recv(&acum, 1, MPI_INT, partner, 0, comm, 
                  MPI_STATUS_IGNORE);
            printf("Proc %d, recibo %d de %d\n", my_rank, acum,partner);
            in_val+=acum;
            
         }
      }
      bitmask >>= 1;
      participate >>= 1;
   }
   return in_val;

}
