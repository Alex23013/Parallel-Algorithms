#include "cuda_runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <chrono>
 
typedef std::chrono::high_resolution_clock Clock;
 
#define ITER 65535
 
// Version CPU de la función suma de vectores
void vector_add_cpu(int *a, int *b, int *c, int n) {
    int i;
 
    // Add the vector elements a and b to the vector c
    for (i = 0; i < n; ++i) {
    c[i] = a[i] + b[i];
    }
}
 
// Versión GPU de la función suma de vectores
__global__ void vector_add_gpu(int *gpu_a, int *gpu_b, int *gpu_c, int n) {
    int i = threadIdx.x;
    // No es necesario el loop for por que el runtime de CUDA
    // maneja estos hilos ITER veces
    gpu_c[i] = gpu_a[i] + gpu_b[i];
}
 
int main() {
 
    int *a, *b, *c;
    int *gpu_a, *gpu_b, *gpu_c;
 
    a = (int *)malloc(ITER * sizeof(int));
    b = (int *)malloc(ITER * sizeof(int));
    c = (int *)malloc(ITER * sizeof(int));
 
    // Necesitamos variables accesibles en CUDA,
    // para eso cudaMallocManaged nos las provee
    cudaMallocManaged(&gpu_a, ITER * sizeof(int));
    cudaMallocManaged(&gpu_b, ITER * sizeof(int));
    cudaMallocManaged(&gpu_c, ITER * sizeof(int));
 
    for (int i = 0; i < ITER; ++i) {
        a[i] = i;
        b[i] = i;
        c[i] = i;
    }
 
    // Llama a la versión CPU y la temporiza
    auto cpu_start = Clock::now();
    vector_add_cpu(a, b, c, ITER);
    auto cpu_end = Clock::now();
    std::cout << "vector_add_cpu: "
    << std::chrono::duration_cast<std::chrono::nanoseconds>(cpu_end - cpu_start).count()
    << " nanoseconds.\n";
 
    // Llama a la versión GPU y la temporiza
    // Los triples <> es una extensión del runtime CUDA que permite
    // que los parametros de una llamada al kernel CUDA sean pasados
    // En este ejemplo estamos pasando un thread block con ITER threads
    auto gpu_start = Clock::now();
    vector_add_gpu <<<1, ITER>>> (gpu_a, gpu_b, gpu_c, ITER);
    cudaDeviceSynchronize();
    auto gpu_end = Clock::now();
    std::cout << "vector_add_gpu: "
    << std::chrono::duration_cast<std::chrono::nanoseconds>(gpu_end - gpu_start).count()
    << " nanoseconds.\n";
 
    // Libere la memoria basada en la función GPU allocations
    cudaFree(a);
    cudaFree(b);
    cudaFree(c);
 
    // Libere la memoria basada en la función CPU allocations
    free(a);
    free(b);
    free(c);
 
    return 0;
}


