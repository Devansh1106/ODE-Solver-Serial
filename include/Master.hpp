#ifndef MASTERHEADERDEF
#define MASTERHEADERDEF
#include<vector>
#include<string>
class BoundaryConditions {
    private:
        bool mLhsBcIsDirichlet;
        bool mRhsBcIsDirichlet;
        bool mLhsBcIsNeumann;
        bool mRhsBcIsNeumann;
        double mLhsBcValue;
        double mRhsBcValue;
    public:
        friend class BvpOde;
        BoundaryConditions();
        void SetLhsDirichletBc(double lhsValue);
        void SetRhsDirichletBc(double rhsValue);
        void SetLhsNeumannBc (double lhsDerivValue);
        void SetRhsNeumannBc (double rhsDerivValue);
};

class Node
{
    public:
        double coordinate;
};

class Vector{
    private:
        double* mData; //data stored in a vector
        int mSize; //size of a vector
    public:
    Vector(int size);
    int GetSize() const;
    double operator[] (int i);
    double& operator() (int i);
    double Read(int i) const;     
};

class SecondOrderOde{
    friend class BvpOde;
public:
    double mCoeffofUxx;
    double mCoeffofUx;
    double mCoeffofU;
    double (*mpRhsFunc) (double x);
    double mXmin;
    double mXmax;
    SecondOrderOde()=default;
    SecondOrderOde(double coeffUxx, double coeffUx, double coeffU,double (*righthandside)(double),double xMinimum,double xMaximum);    
};

class Matrix
{
    private:
        double** mData; 
        int mNumRows,mNumcols;
    public:
        Matrix(const Matrix& otherMatrix);
        Matrix(int numRows,int numCols);
        ~Matrix();
        int GetNumberOfRows() const;
        int GetNumberOfCols() const;
        double& operator()(int i,int j); // 1-based indexing
        Matrix& operator=(const Matrix &otherMatrix); // overloading assignment operator
        Matrix operator+() const; // unary +
        Matrix operator-() const; // unary-
        Matrix operator+(const Matrix& m1) const ; // binary +
        Matrix operator-(const Matrix& m2) const ; // binary -
        Matrix operator*(double a) const; // scalar multiplication
        double CalculateDeterminant () const;
        friend Vector operator* (const Matrix& m, const Vector& v); // Vector multiplication friendship
        friend Vector operator* (const Vector& v, const Matrix& m); // Vector multipliaction is not commutative
};
Vector operator* (const Matrix& m, const Vector& v); // prototype signature for friend operators
Vector operator* (const Vector& v, const Matrix& m); // prototype ensures its use before its actual definition

class LinearSystem
{
    private:
        int mSize; // Size of the linear system
        Matrix* mpA; // matrix for linear system
        Vector* mpb; // vector for linear system(rhs vector)
        /*Copy constructor is private. Only allow constructor that specifies matrix and vector */
        LinearSystem (const LinearSystem& otherLinearSystem);
    public:
        LinearSystem(const Matrix& A, const Vector& b);
        ~LinearSystem(); // destructor frees memory
        virtual Vector Solve(); 
};

class FiniteDifferenceGrid{ 
    public:
        std::vector<Node> mNodes;
        FiniteDifferenceGrid(int numNodes, double xMin, double xMax);
};

class BvpOde{
    private:
        BvpOde(const BvpOde& otherBvpOde){}
        int mNumNodes;
        FiniteDifferenceGrid* mpGrid;
        SecondOrderOde* mpOde;
        BoundaryConditions* mpBconds;
        Vector* mpSolVec;
        Vector* mpRhsVec;
        Matrix* mpLhsMat;
        LinearSystem* mpLinearSystem;
        std::string mFilename;
        void PopulateMatrix();
        void PopulateVector();
        void ApplyBoundaryConditions();
    public:
        BvpOde(SecondOrderOde* pOde, BoundaryConditions* pBcs,int numNodes);
        ~BvpOde();
        void SetFilename(const std::string& name){
            mFilename= name;
        }
        void Solve();
        void WriteSolutionFile();
};
#endif