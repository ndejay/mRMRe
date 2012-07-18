#include "Matrix.hpp"

Matrix::Matrix(unsigned int const rowCount, unsigned int const columnCount) :
        mpData(new float[rowCount * columnCount]), mRowCount(rowCount), mColumnCount(columnCount), mHasAllocation(
                true)
{

}

Matrix::Matrix(float* const data, unsigned int const rowCount, unsigned int const columnCount) :
        mpData(data), mRowCount(rowCount), mColumnCount(columnCount), mHasAllocation(false)
{

}

Matrix::~Matrix()
{
    if (mHasAllocation)
        delete[] mpData;
}

/* inline */float&
Matrix::operator()(unsigned int const i, unsigned int const j)
{
    return mpData[(j * mRowCount) + i];
}

/* inline */unsigned int const
Matrix::getRowCount() const
{
    return mRowCount;
}

/* inline */unsigned int const
Matrix::getColumnCount() const
{
    return mColumnCount;
}
