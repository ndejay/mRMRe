#include "Matrix.hpp"

Matrix::Matrix(unsigned int const rowCount, unsigned int const columnCount) :
        mpData(new float[rowCount * columnCount]), mRowCount(rowCount), mColumnCount(columnCount), mHasAllocation(
                true)
{

}

/* explicit */
Matrix::Matrix(unsigned int const size, unsigned int const rowCount, unsigned int const columnCount) :
        mpData(new float[size]), mRowCount(rowCount), mColumnCount(columnCount), mHasAllocation(
                true)
{

}

/* explicit */
Matrix::Matrix(float* const pData, unsigned int const rowCount, unsigned int const columnCount) :
        mpData(pData), mRowCount(rowCount), mColumnCount(columnCount), mHasAllocation(false)
{

}

/* virtual */
Matrix::~Matrix()
{
    if (mHasAllocation)
        delete[] mpData;
}

/* virtual */float&
Matrix::operator()(unsigned int const i, unsigned int const j)
{
    return mpData[(j * mRowCount) + i];
}

unsigned int const
Matrix::getColumnCount() const
{
    return mColumnCount;
}

unsigned int const
Matrix::getRowCount() const
{
    return mRowCount;
}
