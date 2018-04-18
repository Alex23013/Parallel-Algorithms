#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

int compute_partner(int phase, int my_rank, int size){
    
	int partner;
	
	if(phase%2==0)
		if(my_rank % 2 !=0)
			partner=my_rank-1;
		else
			partner=my_rank+1;
	else
		if(my_rank % 2 != 0)
			partner=my_rank+1;
		else
			partner=my_rank-1;
		
	if(partner==-1 || partner == size)
		partner=MPI_PROC_NULL;
	
	return partner;
}

int main(int argc, char* argv[]) {

	int n, i, local_n, phase, m_i, r_i, t_i, partner;
	
	MPI_Comm comm;
	
	n=atoi(argv[1]);
	srand(time(NULL));
	
	int *A = (int *)malloc(sizeof(int) *n);
	int rank;
	int size;
	printf("Vivo\n");
        MPI_Init(&argc, &argv);
	comm = MPI_COMM_WORLD;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	local_n=n/size;
	
	if(rank==0){	

		for(i=0;i<n;++i){
			A[i]=(rand() %100);
			printf("%d, ", A[i]);
		}

	}

	int *my_keys = (int *)malloc(sizeof(int) * local_n);

	int *recv_keys = (int *)malloc(sizeof(int) * local_n);
	
	int *temp_keys = (int *)malloc(sizeof(int) * local_n);
	
	MPI_Scatter(A, local_n, MPI_INT, my_keys,
              local_n, MPI_INT, 0, comm);
	
	qsort (my_keys, local_n, sizeof(int), compare);
	//for(i=0; i<local_n; ++i)
	//	printf("Proceso %d %d\n", rank, my_keys[i]);
	//printf("\n\n");
	for(phase=0; phase < size; ++phase){
	  
		partner = compute_partner(phase, rank, size);
		//printf("Fase %d: %d envia a %d\n", phase, rank, partner);
		if(partner>=0){
		MPI_Sendrecv(my_keys, local_n, MPI_INT, partner, 0,recv_keys, local_n, MPI_INT, partner, 0, comm, MPI_STATUS_IGNORE);
		//for(i=0; i<local_n; ++i)
		//	printf("Fase %d Proceso %d rec %d\n", phase, rank, recv_keys[i]);
		//printf("\n\n");
		
		if(rank<partner){
			m_i= r_i= t_i=0;
			while (t_i < local_n) {
				if (my_keys[m_i] <= recv_keys[r_i]) {
					
					temp_keys[t_i] = my_keys[m_i];
					t_i++; m_i++;
					
				} else {
					temp_keys[t_i] = recv_keys[r_i];
					t_i++; r_i++;
					
				}
			}
			for (m_i = 0; m_i < local_n; m_i++)
				my_keys[m_i] = temp_keys[m_i];
		}
		else{
			m_i= r_i= t_i=local_n-1;
			while (t_i > -1) {
				if (my_keys[m_i] > recv_keys[r_i]) {
					
					temp_keys[t_i] = my_keys[m_i];
					t_i--; m_i--;
					
				} else {
					temp_keys[t_i] = recv_keys[r_i];
					t_i--; r_i--;
					
				}
			}
			for (m_i = 0; m_i < local_n; m_i++)
				my_keys[m_i] = temp_keys[m_i];
		}
		  
		}
		//for(i=0; i<local_n; ++i)
		//	printf("Fase %d Proceso %d %d\n", phase, rank, my_keys[i]);
		//printf("\n\n");
		
	}
	
	printf("Proceso %d\n", rank);
	for(i=0; i<local_n; ++i)
		printf("%d, ", my_keys[i]);
	printf("\n");
	
	MPI_Finalize();	

	return 0;
}

