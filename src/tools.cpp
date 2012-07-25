#include "tools.hpp"

DataMatrixComparator::DataMatrixComparator(unsigned int const featureIndex,
        Matrix* const pDataMatrix) :
        mFeatureIndex(featureIndex), mpDataMatrix(pDataMatrix)
{

}

bool const
DataMatrixComparator::operator()(unsigned int const i, unsigned int const j) const
{
    return (*mpDataMatrix)(i, mFeatureIndex) < (*mpDataMatrix)(j, mFeatureIndex);
}

void const
placeRanksByFeatureIndex(unsigned int const index, Matrix* const pRankedDataMatrix,
        Matrix* const pDataMatrix)
{
    unsigned int const sample_count = pRankedDataMatrix->getRowCount();
    unsigned int p_order[sample_count];

    for (unsigned int i = 0; i < sample_count; ++i)
        p_order[i] = i;

    std::sort(p_order, p_order + sample_count, DataMatrixComparator(index, pDataMatrix));

    for (unsigned int i = 0; i < sample_count; ++i)
        (*pRankedDataMatrix)(p_order[i], index) = i;
}

float const
computeSpearmanCorrelation(unsigned int const i, unsigned int const j,
        Matrix* const pRankedDataMatrix)
{
    float *a = &(*pRankedDataMatrix)(0, i);
    float *b = &(*pRankedDataMatrix)(0, j);
    unsigned int const sample_count = pRankedDataMatrix->getRowCount();
    float sum = 0.;

    for (unsigned int n = 0; n < sample_count; ++n)
    {
        float const difference = a[n] - b[n];
        sum += difference * difference;
    }

    float const correlation = 1
            - ((6 * sum) / (sample_count * ((sample_count * sample_count) - 1)));

    return -0.5 * log(1 - (correlation * correlation));
}

float const
computeConcordanceIndex(unsigned int const discreteFeatureIndex,
        unsigned int const continuousFeatureIndex, int const timeFeatureIndex,
        Matrix* const pDataMatrix, float const* const pSampleWeights,
        unsigned int const* const pSampleStrata, bool const outX)
{
    float concordant_weight = 0.;
    float discordant_weight = 0.;
    float uninformative_weight = 0.;
    float relevant_weight = 0.;

    unsigned int const sample_count = pDataMatrix->getRowCount();
    for (unsigned int i = 0; i < sample_count; ++i)
    {
        for (unsigned int j = 0; j < sample_count; ++j)
        {
            if (pSampleStrata[i] == pSampleStrata[j])
            {
                float pair_weight = pSampleWeights[i] * pSampleWeights[j];
                if ((timeFeatureIndex >= 0
                        && ((*pDataMatrix)(i, timeFeatureIndex)
                                < (*pDataMatrix)(j, timeFeatureIndex)
                                && (*pDataMatrix)(i, discreteFeatureIndex) == 1))
                        || (timeFeatureIndex < 0
                                && (*pDataMatrix)(i, discreteFeatureIndex)
                                        > (*pDataMatrix)(j, discreteFeatureIndex)))
                {
                    relevant_weight += pair_weight;
                    if ((*pDataMatrix)(i, continuousFeatureIndex)
                            > (*pDataMatrix)(j, continuousFeatureIndex))
                        concordant_weight += pair_weight;
                    else if ((*pDataMatrix)(i, continuousFeatureIndex)
                            < (*pDataMatrix)(j, continuousFeatureIndex))
                        discordant_weight += pair_weight;
                    else if (outX)
                        uninformative_weight += pair_weight;
                    else
                        discordant_weight += pair_weight;
                }
                else if ((timeFeatureIndex >= 0
                        && ((*pDataMatrix)(i, timeFeatureIndex)
                                > (*pDataMatrix)(j, timeFeatureIndex)
                                && (*pDataMatrix)(j, discreteFeatureIndex) == 1))
                        || (timeFeatureIndex < 0
                                && (*pDataMatrix)(i, discreteFeatureIndex)
                                        < (*pDataMatrix)(j, discreteFeatureIndex)))
                {
                    relevant_weight += pair_weight;
                    if ((*pDataMatrix)(i, continuousFeatureIndex)
                            < (*pDataMatrix)(j, continuousFeatureIndex))
                        concordant_weight += pair_weight;
                    else if ((*pDataMatrix)(i, continuousFeatureIndex)
                            > (*pDataMatrix)(j, continuousFeatureIndex))
                        discordant_weight += pair_weight;
                    else if (outX)
                        uninformative_weight += pair_weight;
                    else
                        discordant_weight += pair_weight;
                }
            }
        }
    }

    return concordant_weight / relevant_weight;
}
