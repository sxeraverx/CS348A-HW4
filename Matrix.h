#pragma once
/*
template <typename T, int M, int N>
class Matrix
{
public:
    class ColumnLookup
    {
    private:
        T *data; 
        ColumnLookup() {}
        friend class Matrix<T,M,N>;
        int row;
    public:
        T &operator[](int column)             {return data[N*row+column];}
        const T &operator[](int column) const {return data[N*row+column];}
    };

    class ConstColumnLookup
    {
    private:
        const T *data; 
        ConstColumnLookup() {}
        friend class Matrix<T,M,N>;
        int row;
    public:
        const T &operator[](int column) const {return data[N*row+column];}
    };
public:
    static const int width=M;
    static const int height=N;
private:
    T *data;
public:
    Matrix() //Default constructor is zero matrix
    {
        data = new T[M*N];
        for(int row = 0; row<M; row++)
            for(int col = 0; col<N; col++)
                data[row*N+col] = 0;
    }
    ~Matrix()
    {
        delete[] data;
    }
    Matrix &operator=(const Matrix<T,M,N> &other)
    {
        for(int i = 0; i < M; i++)
            for(int j = 0; j < N; j++)
                (*this)[i][j] = other[i][j];
    }
    ColumnLookup operator[](int row) {ColumnLookup cl; cl.data=data; cl.row=row; return cl;}
    ConstColumnLookup operator[](int row) const {ConstColumnLookup ccl; ccl.data=data; ccl.row=row; return ccl;}
    
    template <typename T1, int N1>
        Matrix<typeof(T*T1),M,N1> operator*(const Matrix<T1,N,N1> &other) const
    {
        Matrix<typeof(T*T1),M,N1> ret;
        for(int i = 0; i < M; i++)
            for(int j = 0; j < N; j++)
                for(int k = 0; k < N1; k++)
                    ret[i][k]+=(*this)[i][j]*other[j][k];
    }
};
*/
