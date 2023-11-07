#include"Master.hpp"
#include<cassert>
#include<cmath>
#include<iostream>
#include<chrono>

using namespace std;

// copy matrix and vector so that by gaussian elimination original matrix and vector remains unchanged
LinearSystem::LinearSystem(const Matrix& A, const Vector& b )
{
    //Check for size compatibility of matrix and vector
    int local_size= A.GetNumberOfRows();
    assert(A.GetNumberOfCols() == local_size);
    assert(b.GetSize() == local_size);

    //Set variables for linear system
    mSize=local_size;
    mpA = new Matrix(A);
    mpb = new Vector(b);
}

// Destructor frees memory
LinearSystem::~LinearSystem()
{
    delete mpA;
    delete mpb;
}

//Solve the linear system using Gaussian elimination
//This will change the matrix mpA
Vector LinearSystem::Solve()
{
    auto start = chrono::steady_clock::now(); //for execution time
    Vector m(mSize);
    Vector solution(mSize);

    //Introducing pointer for easy access to Matrix and vector
    Matrix rA = *mpA;
    Vector rb = *mpb;

    //forward sweep of gaussian elimination (conversion to upper triangular form)
    for (int k=0; k < mSize-1; k++)
    {
        //see if pivoting is necessary
        double max=0.0;
        int row = -1;
        for(int i = k; i < mSize; i++)
        {
            if (fabs(rA(i+1,k+1)) > max)
            {
                row = i;
            }
        }
        assert (row > 0);

        // pivot if necessary
        if (row != k)
        {
            //swap matrix k+1 with row+1
            for (int i=0; i < mSize; i++)
            {
                double temp = rA(k+1,i+1);
                rA(k+1,i+1) = rA(row+1,i+1);
                rA(row+1,i+1) = temp;
            }
            //swap vector enteries k+1 with row+1
            double temp = rb(k+1);
            rb(k+1) = rb(row+1);
            rb(row+1) = temp;
        }

        //create 0 in the lower part of column k
        for (int i=k+1; i < mSize; i++)
        {
            m(i+1) = rA(i+1,k+1)/rA(k+1,k+1);
            for (int j=k; j < mSize; j++)
            {
                rA(i+1,j+1) -= rA(k+1,j+1)*m(i+1);
            }
            rb(i+1) -= rb(k+1)*m(i+1);
        }
    }

    //back substitution
    for (int i=mSize-1; i > -1; i--)
    {
        solution(i+1) = rb(i+1);
        for (int j=i+1; j < mSize; j++ )
        {
            solution(i+1) -= rA(i+1,j+1)*solution(j+1);
        }
        solution (i+1) /= rA(i+1,i+1);
    }
    auto end = chrono::steady_clock::now();  //for execution time
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
    return solution;
}