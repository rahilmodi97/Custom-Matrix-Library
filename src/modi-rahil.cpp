/**
 * @file main.cpp
 * @author Rahil Modi
 * @brief Test correctness of the matmul_trans library using tests.
 * 
 * Test file for the portable linear algebra library defined in the matmul_trans.h file.
 * 
 * There are two modes manual and test.
 * 
 * manual : This mode lets you select which function to choose transpose or multiply and also manually enter the
 * matrices. For improved performance there is a multithreaded version where you can enter the number of threads you
 * want other wise for non-multithreaded version you will have to enter number of threads as 1.
 * 
 * test : This an automatic mode which will automatically run six test cases defined in this file and show if the test
 * cases passed or not.
 * 
 * Pseudo command:
 * ./matmul <mode> <function> <show execution time or not> <no of threads> <row1> <col1> <matrix values> <row2> <col2>
 * <matrix values>
 * 
 * Command for manual mode:
 *  Transpose:
 *  ./matmul manual transpose true 1 2 2 1,2,2,1
 * 
 *  Multiplication:
 *  ./matmul manual multiply false 2 3 3 1,2,3,4,5,6,7,8,9 3 1 4,5,6
 * 
 * Command for test mode:
 *  ./matmul test true
 * 
 * @date 2021-06-25
 */

#include "modi-rahil.h"
#include <sstream>
#include <cstring>

int main(int argc, char** argv){

  if(strcmp(argv[1], "manual") == 0){

    if(argc < 8){
      std::cerr << "Not enough arguments given to run the manual mode. Refer to readme on how to use the arguments."
      << std::endl;
      return 1;
    }
    std::cout << std::endl;
    std::cout << "Manual mode is selected and values have been entered. " << std::endl << std::endl;
    int rows_1 = atoi(argv[5]);
    int cols_1 = atoi(argv[6]);
    int num_threads = atoi(argv[4]);

    // Check to see if the number of threads are not more than the hardware capabilities.
    if (num_threads > std::thread::hardware_concurrency()){
      std::cout << "You have selected number threads beyond your system capacity. Please keep it equal to or below "
      << std::thread::hardware_concurrency() << std::endl;
      return 0;
    }

    // Check to see if the number of threads are not more than the size of the matrix.
    if (num_threads > std::max(rows_1, cols_1)){
      num_threads = 1;
      std::cout << "You have entered number of threads more than the size of matrix so number of threads are set to 1."
      << std::endl << std::endl;
    }
    std::string values_1 = argv[7];
    bool show_timing;
    if(strcmp(argv[3], "true") == 0){
      show_timing = true;
    }else{
      show_timing = false;
    }
    Matrix m;

    if(strcmp(argv[2], "transpose") == 0){

      std::cout << "Transpose function is selected. " << std::endl << std::endl;

      // Check to see if invalid matrix sizes are not entered.
      if(rows_1 <= 0 || cols_1 <= 0){
        std::cerr << "Values are zero or less than zero" << std::endl;
      }

      double** m1 = m.CreateMatrix(values_1, rows_1, cols_1);
      double** trans_mat = m.transpose(m1, rows_1, cols_1, num_threads, show_timing);

      std::cout << "Result of matrix transpose is: " << std::endl << std::endl;
      m.print(trans_mat,cols_1,rows_1);
    }
    else if(strcmp(argv[2], "multiply") == 0){

      if(argc < 11){
        std::cerr << "Not enough arguments given to run manual mode for matrix multiplication. Refer to readme on " <<
        "how to use the arguments. " << std::endl;
        return 1;
      }
      std::cout << "Matrix multiplication function is selected. " << std::endl << std::endl;
      int rows_2 = atoi(argv[8]);
      int cols_2 = atoi(argv[9]);
      std::string values_2 = argv[10];

      // Check to see if the number of threads are not more than the size of the matrix.
      if (num_threads > std::max(rows_2, cols_2)){
        num_threads = 1;
        std::cout << "You have entered number of threads more than the size of matrix so number of threads are "
        << "set to 1." << std::endl << std::endl;
      }

      // Check to see if invalid matrix sizes are not entered.
      if(rows_1 <= 0 || cols_1 <= 0 || rows_2 <= 0 || cols_2 <= 0){
        std::cerr << "Values are zero or less than zero" << std::endl;
        return 1;
      }

      // Check to confirm cols of m1 are equal to rows of m2 otherwise matrix multiplication is not possible.
      if(cols_1 != rows_2){
        std::cerr << "Cannot multiply columns of first matrix should be equal to rows of second matrix " << std::endl;
        return 1;
      }

      double** m1 = m.CreateMatrix(values_1, rows_1, cols_1);
      double** m2 = m.CreateMatrix(values_2, rows_2, cols_2);

      double** mul_matrix = m.multiplication(m1, m2, rows_1, cols_1, cols_2, num_threads, show_timing);
      std::cout << "Result of matrix multiplication is: " << std::endl << std::endl;
      m.print(mul_matrix, rows_1, cols_2);

    }else{

      std::cout << "You have selected manual method but did not specify the function transpose or multiply."
      << std::endl << std::endl;
      return 1;
    }
  }
  else if(strcmp(argv[1], "test") == 0){

    if(argc > 4){
      std::cerr << "You have selected test mode and entered additional arguments. You are not required to enter " <<
      "any other arguments except show time flag in test mode. The test mode is an automatic mode." << std::endl;
      return 1;
    }

    std::cout << std::endl;
    std::cout << "Test mode is selected. After performing all the test cases it will display how many test cases" <<
    " were passed" << std::endl << std::endl;

    bool show_timing;
    if(strcmp(argv[2], "true") == 0){
      show_timing = true;
    }else{
      show_timing = false;
    }
    Matrix m;
    int count = 0;

    /**
     * Transpose Test Case 1 : Symmetric matrix.
     */
    double** m1 = 0;
    int rows = 2, cols = 2;
    int num_threads = 1;
    m1 = m.EmptyMatrix(rows, cols);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[1][0] = 2;
    m1[1][1] = 1;
    double** trans_mat = m.transpose(m1, rows, cols, num_threads, show_timing);
    double** expec_mat = 0;
    expec_mat = m.EmptyMatrix(rows, cols);
    expec_mat[0][0] = 1;
    expec_mat[0][1] = 2;
    expec_mat[1][0] = 2;
    expec_mat[1][1] = 1;
    if(!m.check(trans_mat, expec_mat, rows, cols)){
      std::cout << "Test Case 1 : Transpose symmetric matrix failed" << std::endl << std::endl;
      count++;
    }else{
      std::cout << "Test Case 1 : Transpose symmetric matrix passed" << std::endl << std::endl;
    }

    /**
     * Transpose Test Case 2 : Asymmetric matrix.
     */
    m1 = 0;
    rows = 2, cols = 2;
    m1 = m.EmptyMatrix(rows, cols);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[1][0] = 3;
    m1[1][1] = 4;
    trans_mat = m.transpose(m1, rows, cols, num_threads, show_timing);
    expec_mat = 0;
    expec_mat = m.EmptyMatrix(rows, cols);
    expec_mat[0][0] = 1;
    expec_mat[0][1] = 3;
    expec_mat[1][0] = 2;
    expec_mat[1][1] = 4;
    if(!m.check(trans_mat, expec_mat, rows, cols)){
      std::cout << "Test Case 2 : Transpose asymmetric matrix failed" << std::endl << std::endl;
      count++;
    }else{
      std::cout << "Test Case 2 : Transpose asymmetric matrix passed" << std::endl << std::endl;
    }

    /**
     * Transpose Test Case 3.
     */
    m1 = 0;
    rows = 5, cols = 7;
    m1 = m.EmptyMatrix(rows, cols);
    int value = 1;
    for(int i = 0; i<rows; i++){
      for(int j = 0; j<cols; j++){
        m1[i][j] = value++;
      }
    }
    trans_mat = m.transpose(m1, rows, cols, num_threads, show_timing);
    expec_mat = 0;
    expec_mat = m.EmptyMatrix(cols, rows);
    value = 1;
    for(int i = 0; i<rows; i++){
      for(int j = 0; j<cols; j++){
        expec_mat[j][i] = value++;
      }
    }
    if(!m.check(trans_mat, expec_mat, rows, cols)){
      std::cout << "Test Case 3 : Transpose matrix failed" << std::endl << std::endl;
      count++;
    }else{
      std::cout << "Test Case 3 : Transpose matrix passed" << std::endl << std::endl;
    }

    /**
     * Matrix multiplication Test Case 4.
     */
    m1 = 0;
    rows = 2, cols = 2;
    m1 = m.EmptyMatrix(rows, cols);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[1][0] = 2;
    m1[1][1] = 1;
    double** matmul = m.multiplication(m1, m1, rows, cols, cols, num_threads, show_timing);
    expec_mat = m.EmptyMatrix(rows, cols);
    expec_mat[0][0] = 5;
    expec_mat[0][1] = 4;
    expec_mat[1][0] = 4;
    expec_mat[1][1] = 5;
    if(!m.check(matmul, expec_mat, rows, cols)){
      std::cout << "Test Case 4 : Matrix multiplication failed" << std::endl << std::endl;
      count++;
    }else{
      std::cout << "Test Case 4 : Matrix multiplication passed" << std::endl << std::endl;
    }

    /**
     * Matrix multiplication Test Case 5.
     */
    m1 = 0;
    rows = 2, cols = 2;
    m1 = m.EmptyMatrix(rows, cols);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[1][0] = 2;
    m1[1][1] = 1;
    double** m2 = 0;
    m2 = m.EmptyMatrix(rows, cols);
    m2[0][0] = 1;
    m2[0][1] = 2;
    m2[1][0] = 3;
    m2[1][1] = 4;
    matmul = m.multiplication(m1, m2, rows, cols, cols, num_threads, show_timing);
    expec_mat = m.EmptyMatrix(rows, cols);
    expec_mat[0][0] = 7;
    expec_mat[0][1] = 10;
    expec_mat[1][0] = 5;
    expec_mat[1][1] = 8;
    if(!m.check(matmul, expec_mat, rows, cols)){
      std::cout << "Test Case 5 : Matrix multiplication failed" << std::endl << std::endl;
      count++;
    }else{
      std::cout << "Test Case 5 : Matrix multiplication passed" << std::endl << std::endl;
    }

    /**
     * Matrix multiplication Test Case 3.
     */
    m1 = 0;
    int rows1 = 2, cols1 = 5;
    m1 = m.EmptyMatrix(rows1, cols1);
    std::string str = "7,9,11,13,15,8,10,12,14,16";
    std::stringstream ss(str);
    std::string S;
    for(int i = 0; i < rows1; i++){
      m1[i] = new double[cols1];
      for(int j = 0; j < cols1; j++){
        getline(ss, S, ',');
        float value = std::stof(S);
        m1[i][j] = value;
      }
    }
    m2 = 0;
    int rows2 = 5, cols2 = 4;
    m2 = m.EmptyMatrix(rows2, cols2);
    std::string str2 = "1,5,5,0,6,1,6,1,8,8,1,0,9,1,9,1,1,10,10,0";
    std::stringstream ss2(str2);
    std::string S2;
    for(int i = 0; i < rows2; i++){
      m2[i] = new double[cols2];
      for(int j = 0; j < cols2; j++){
        getline(ss2, S2, ',');
        float value = std::stof(S2);
        m2[i][j] = value;
      }
    }
    matmul = m.multiplication(m1, m2, rows1, cols1, cols2, num_threads, show_timing);
    expec_mat = m.EmptyMatrix(rows1, cols2);
    expec_mat[0][0] = 281;
    expec_mat[0][1] = 295;
    expec_mat[0][2] = 367;
    expec_mat[0][3] = 22;
    expec_mat[1][0] = 306;
    expec_mat[1][1] = 320;
    expec_mat[1][2] = 398;
    expec_mat[1][3] = 24;
    if(!m.check(matmul, expec_mat, rows1, cols2)){
      std::cout << "Test Case 6 : Matrix multiplication failed" << std::endl << std::endl;
      count++;
    }else{
      std::cout << "Test Case 6 : Matrix multiplication passed" << std::endl << std::endl;
    }

    if (count == 0){
      std::cout << "All " << 6 - count << " out of 6 test cases passed." << std::endl << std::endl;
    }else{
      std::cout << "Only " << 6 - count << " out of 6 test cases passed. Please look above to see which test " <<
      "cases have failed." << std::endl << std::endl;
    }
  }else{
    std::cout << "You have not selected the mode manual or test. Refer to readme on how to use arguments" << std::endl;
    return 1;
  }
}