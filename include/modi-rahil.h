/**
 * @file matmul_trans.h
 * @author Rahil Modi
 * @brief Portable linear algebra library with transpose and multiplication of matrices functionality.
 * @date 2021-06-25
 */

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <cstdio>
#include <sstream>

class Matrix{

  public:

    /**
     * @brief Creating a empty matrix of the required size.
     * @param rows : Number of rows in the matrix.
     * @param cols : Number of colummns in the matrix.
     * @return matrix : returns a 2D matrix.
     */
    double** EmptyMatrix(int rows, int cols){
      double** matrix = 0;
      matrix = new double*[rows];
      for(int i = 0; i < rows; i++){
        matrix[i] = new double[cols];
        for(int j = 0; j < cols; j++){
          matrix[i][j] = 0.0;
        }
      }
      return matrix;
    }

    /**
     * @brief Create a 2D matrix based on user input.
     * @param str : string of matrix values.
     * @param rows : Number of rows in the matrix.
     * @param cols : Number of columns in the matrix.
     * @return 2D Matrix.
     */
    double** CreateMatrix(std::string str, int rows, int cols){
      double** matrix = 0;
      matrix = new double*[rows];
      for(int i = 0; i < rows; i++){
        matrix[i] = new double[cols];
      }
      int r = 0, c = 0;
      std::stringstream ss(str);
      std::string S;
      for(int i = 0; i < rows; i++){
        matrix[i] = new double[cols];
        for(int j = 0; j < cols; j++){
          getline(ss, S, ',');
          float value = std::stof(S);
          matrix[i][j] = value;
        }
      }
      std::cout << "Input Matrix : " << std::endl << std::endl;
      print(matrix, rows, cols);
      return matrix;
    }

    /**
     * @brief Function to print the matrix.
     * @param input_matrix : The matrix to be printed.
     * @param rows : Number of rows in the matrix.
     * @param cols : Number of columns in the matrix.
     */
    void print(double** input_matrix, int rows, int cols){
      for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
          std::cout << input_matrix[i][j] << " ";
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }

    /**
     * @brief Function to compare if two matrices are equal or not.
     * @param m1 : First matrix.
     * @param m2 : Second matrix.
     * @param rows : Number of rows to be expected in result matrix.
     * @param cols : Number of columns to be expected in result matrix.
     */
    int check(double** m1, double** m2, int rows, int cols){
      int i, j;
      for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
          if (m1[i][j] != m2[i][j])
        return 0;
      return 1;
    }

    /**
     * @brief Returning transpose of the input matrix.
     * @param input_matrix
     * @param rows
     * @param cols
     * @return transposed 2D matrix.
     */
    double** transpose(double** input_matrix, int rows, int cols, int num_threads, bool show_timing){
      if(num_threads <= 1){
        return transmul(input_matrix, rows, cols, show_timing);
      }else{
        std::cout << "Multithreaded method is selected. " << std::endl << std::endl;
        return TransmulThread(input_matrix, rows, cols, num_threads, show_timing);
      }
    }

    /**
     * @brief Returning the result of the multiplication of two matrices.
     * @param input_matrix_1
     * @param input_matrix_2
     * @param r1 : Number of rows of the first matrix.
     * @param c1 : Number of colummns in the first matrix.
     * @param c2 : Number of colummns in the second matrix.
     * @return result of multiplication.
     */
    double** multiplication(double** input_matrix_1, double** input_matrix_2, int r1, int c1, int c2, int num_threads,
      bool show_timing){
      if(num_threads <= 1){
        return matmul(input_matrix_1, input_matrix_2, r1, c1, c2, show_timing);
      }else{
        std::cout << "Multithreaded method is selected. " << std::endl << std::endl;
        return MatmulThread(input_matrix_1, input_matrix_2, r1, c1, c2, num_threads, show_timing);
      }
    }

  private:

    /**
     * @brief Function to perform the transpose on the matrix.
     * @param input_matrix
     * @param rows : Number of rows in the matrix.
     * @param cols : Number of colummns in the matrix.
     * @param show_timing : Boolean to display execution time.
     * @return transposed matrix.
     */
    double** transmul(double** input_matrix, int rows, int cols, bool show_timing){

      double** matrix = EmptyMatrix(cols, rows);
      std::chrono::system_clock::time_point begin;
      if (show_timing)
        begin = std::chrono::high_resolution_clock::now();

      for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
          matrix[j][i] = input_matrix[i][j];
        }
      }

      if (show_timing){
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        std::cout << "Time measured: " << elapsed.count() << " nanoseconds" << std::endl << std::endl;
      }
      return matrix;
    }

    /**
     * @brief Worker Thread Function.
     * @param matrix : Transposed matrix.
     * @param input_matrix : Input matrix.
     * @param row_start : Row to start with.
     * @param rows_computed : Number of rows to compute.
     * @param cols : Number of columns.
     */
    static void TransmulWorkerThread(double** matrix, double** input_matrix, int row_start, int rows_computed,
      int cols){
      for(int i = row_start; i < rows_computed + row_start; i++){
        for(int j = 0; j < cols; j++){
          matrix[j][i] = input_matrix[i][j];
        }
      }
    }

    /**
     * @brief Multithreaded transpose function. The work will be divide among the threads. The difference can be noticed
     * with only very large matrix.
     * @param input_matrix : The input matrix.
     * @param rows : Number of rows in the matrix.
     * @param cols : Number of columns in the matrix.
     * @param num_threads : Number of threads to perform the function.
     * @param show_timing : Boolean to display execution time.
     * @return Transposed matrix.
     */
    double** TransmulThread(double** input_matrix, int rows, int cols, int num_threads, bool show_timing){

      double** matrix = EmptyMatrix(cols, rows);
      std::chrono::system_clock::time_point begin;
      if (show_timing)
        begin = std::chrono::high_resolution_clock::now();

      // array to hold all the threads.
      std::vector<std::thread> p;
      int rowThread = rows / num_threads;
      // Create a thread and divide the work.
      for(int i = 0; i < rows; i+=rowThread){
        int rowCompute = rowThread;
        if(rowThread + i > rows){
          rowCompute = rows - i;
        }
        p.emplace_back(TransmulWorkerThread, matrix, input_matrix, i, rowCompute, cols);
      }
      // Wait for all the threads to finish the work and then merge them together.
      for(auto& t: p){
        t.join();
      }

      if (show_timing){
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        std::cout << "Time measured: " << elapsed.count() << " nanoseconds for number of threads "<< num_threads
        << std::endl << std::endl;
      }
      return matrix;
    }

    /**
     * @brief Function to perform matrix multiplication on two 2D matrices.
     * @param input_matrix_1 : First matrix.
     * @param input_matrix_2 : Second matrix.
     * @param r1 : Number of rows of the first matrix.
     * @param c1 : Number of colummns in the first matrix.
     * @param c2 : Number of colummns in the second matrix.
     * @param show_timing : Boolean to display execution time.
     * @return Matrix after multiplication opeartion.
     */
    double** matmul(double** input_matrix_1, double** input_matrix_2, int r1, int c1, int c2, bool show_timing){

      double** matrix = EmptyMatrix(r1, c2);
      std::chrono::system_clock::time_point begin;
      if (show_timing)
        begin = std::chrono::high_resolution_clock::now();

      for (int i = 0; i < r1; i++){
        for (int j = 0; j < c2; j++){
          for (int k = 0; k< c1; k++){
            matrix[i][j] += input_matrix_1[i][k] * input_matrix_2[k][j];
          }
        }
      }
      if (show_timing){
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        std::cout << "Time measured: " << elapsed.count() << " nanoseconds" << std::endl << std::endl;
      }
      return matrix;
    }

    /**
     * @brief : Muliplication Worker thread
     * @param matrix : Result matrix.
     * @param input_matrix_1 : First matrix.
     * @param input_matrix_2 : Second matrix.
     * @param r1 : Number of rows in first matrix.
     * @param c1 : Number of columns in first matrix.
     * @param c2 : Number of columns in second matrix.
     * @param num_threads : Number of threads to perform the operation.
     * @param index : Index of thread to which the work should be allocated.
     * @param show_timing : Boolean to display execution time.
     */
    static void MatmulWorkerThread(double** matrix, double** input_matrix_1, double** input_matrix_2, int r1, int c1,
      int c2, int num_threads, int index){

        // How much work will be done per thread.
        int elements_per_thread = r1 / num_threads;
        // There might be some remainder after dividing the work for each thread.
        int remainder = r1 % num_threads;
        int start, end;

        // The remainder can be given to the first thread as it will be active till the end.
        if(index == 0){
          start = elements_per_thread * index;
          end = (elements_per_thread * (index + 1)) + remainder;
        }else{
          start = elements_per_thread * index + remainder;
          end = (elements_per_thread * (index + 1)) + remainder;
        }

        for(int i = start; i < end; i++){
          for(int j = 0; j < c2; j++){
            for(int k = 0; k < c1; k++){
              matrix[i][j] += input_matrix_1[i][k] * input_matrix_2[k][j];
            }
          }
        }
    }

    /**
     * @brief : Function for multithreaded matrix multiplication. The matrix multiplication is divided in small parts
     * and assigned to different threads and at the end work of every thread is joined to toegther and the matrix is
     * generated.
     * @param input_matrix_1 : First matrix.
     * @param input_matrix_2 : Second matrix.
     * @param r1 : Number of rows in first matrix.
     * @param c1 : Number of columns in first matrix.
     * @param c2 : Number of columns in second matrix.
     * @param num_threads : Number of threads to perform the operation.
     * @param show_timing : Boolean to display execution time.
     * @return : Result of matrix multiplication.
     */
    double** MatmulThread(double** input_matrix_1, double** input_matrix_2, int r1, int c1, int c2, int num_threads,
      bool show_timing){

        double** matrix = EmptyMatrix(r1, c2);
        std::chrono::system_clock::time_point begin;
        if (show_timing)
          begin = std::chrono::high_resolution_clock::now();

        // array to hold all the threads.
        std::thread p[num_threads];
        // Create a thread and divide the work.
        for(int i = 0; i < num_threads; i++){
          p[i] = std::thread(MatmulWorkerThread, matrix, input_matrix_1, input_matrix_2, r1, c1, c2, num_threads, i);
        }
        // Wait for all the threads to finish the work and then merge them together.
        for(int i = 0; i < num_threads; i++){
          p[i].join();
        }
        if (show_timing){
          auto end = std::chrono::high_resolution_clock::now();
          auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
          std::cout << "Time measured: " << elapsed.count() << " nanoseconds for number of threads "<< num_threads
          << std::endl << std::endl;
        }
        return matrix;
    }
};