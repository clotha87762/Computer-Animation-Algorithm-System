#ifndef _MATH_SPATIAL_TEMPORAL_DATA_H_
#define _MATH_SPATIAL_TEMPORAL_DATA_H_

#include <vector>
#include <algorithm>
#include <iterator>
#include "boost/numeric/conversion/cast.hpp"
#include "boost/iterator/counting_iterator.hpp"
#include "console_log.h"

namespace math {

template<typename Data, typename allocator_type = std::allocator<Data>>
class SpatialTemporalData final
{

public:

    typedef Data Data_t;
    typedef std::vector<Data, allocator_type> DataColl_t;
    /**
     * \brief Default constructor
     */
    SpatialTemporalData();
    /**
     * \brief
     * \param[in] spatial_size
     * \param[in] temporal_size
     */
    explicit SpatialTemporalData(const int32_t spatial_size, const int32_t temporal_size);
    /**
     * \brief Copy constructor
     */
    SpatialTemporalData(const SpatialTemporalData &other);
    /**
     * \brief Destructor
     */
    virtual ~SpatialTemporalData();
    /**
     * \brief Assignment operator
     */
    SpatialTemporalData &operator=(const SpatialTemporalData &other);
    /**
     * \brief
     */
    int32_t SpatialSize() const;
    /**
     * \brief
     */
    int32_t TemporalSize() const;
    /**
     * \brief
     * \param[in] spatial_idx
     * \param[in] temporal_idx
     * \return
     */
    Data Element(const int32_t spatial_idx, const int32_t temporal_idx) const;
    /**
     * \brief
     * \param[in] spatial_idx
     * \return
     */
    DataColl_t SpatialElements(const int32_t spatial_idx) const;
    /**
     * \brief
     * \param[in] temporal_idx
     * \return
     */
    DataColl_t TemporalElements(const int32_t temporal_idx) const;
    /**
     * \brief
     * \param[in] spatial_idx
     * \param[in] temporal_idx
     * \param[in] data
     */
    void SetElement(const int32_t spatial_idx, const int32_t temporal_idx, const Data &data);
    /**
     * \brief
     * \param[in] spatial_idx
     * \param[in] spatial_elements
     */
    void SetSpatialElements(const int32_t spatial_idx, const DataColl_t &spatial_elements);
    /**
     * \brief
     * \param[in] temporal_idx
     * \param[in] temporal_elements
     */
    void SetTemporalElements(const int32_t temporal_idx, const DataColl_t &temporal_elements);
    /**
     * \brief
     */
    void Clear();
    /**
     * \brief
     * \param[in] spatial_size
     * \param[in] temporal_size
     */
    void Resize(const int32_t spatial_size, const int32_t temporal_size);
    /**
     * \brief
     */
    bool IsEmpty() const;

protected:

private:

    typedef std::vector<DataColl_t> DataCollSeq_t;

    void VerifySpatialIdx(const int32_t spatial_idx) const;
    void VerifyTemporalIdx(const int32_t temporal_idx) const;
    void VerifySpatialElementSize(const int32_t spatial_element_size) const;
    void VerifyTemporalElementSize(const int32_t temporal_element_size) const;

    DataCollSeq_t data_coll_sequence_;
};

} // namespace math {

namespace math {

// public func.

template<typename Data, typename allocator_type>
SpatialTemporalData<Data, allocator_type>::SpatialTemporalData()
    :data_coll_sequence_()
{
}

template<typename Data, typename allocator_type>
SpatialTemporalData<Data, allocator_type>::SpatialTemporalData(const int32_t spatial_size, const int32_t temporal_size)
    :data_coll_sequence_()
{
    this->Clear();
    this->Resize(spatial_size, temporal_size);
}

template<typename Data, typename allocator_type>
SpatialTemporalData<Data, allocator_type>::SpatialTemporalData(
        const SpatialTemporalData<Data, allocator_type> &other
        )
    :data_coll_sequence_()
{
    this->Clear();
    this->Resize(
            other.SpatialSize(),
            other.TemporalSize()
            );

    for (int32_t spatial_idx = 0; spatial_idx < other.SpatialSize(); ++spatial_idx)
    {
        for (int32_t temporal_idx = 0; temporal_idx < other.TemporalSize(); ++temporal_idx)
        {
            this->SetElement(
                    spatial_idx,
                    temporal_idx,
                    other.Element(spatial_idx, temporal_idx)
                    );
        }
    }
}

template<typename Data, typename allocator_type>
SpatialTemporalData<Data, allocator_type>::~SpatialTemporalData()
{
}

template<typename Data, typename allocator_type>
SpatialTemporalData<Data, allocator_type> &SpatialTemporalData<Data, allocator_type>::operator=(
        const SpatialTemporalData<Data, allocator_type> &other
        )
{
    if (&other == this)
    {
        return *this;
    }

    this->Clear();
    this->Resize(
            other.SpatialSize(),
            other.TemporalSize()
            );

    for (int32_t spatial_idx = 0; spatial_idx < other.SpatialSize(); ++spatial_idx)
    {
        for (int32_t temporal_idx = 0; temporal_idx < other.TemporalSize(); ++temporal_idx)
        {
            this->SetElement(
                    spatial_idx,
                    temporal_idx,
                    other.Element(spatial_idx, temporal_idx)
                    );
        }
    }

    return *this;
}

template<typename Data, typename allocator_type>
int32_t SpatialTemporalData<Data, allocator_type>::SpatialSize() const
{
    return boost::numeric_cast<int32_t>(data_coll_sequence_.front().size());
}

template<typename Data, typename allocator_type>
int32_t SpatialTemporalData<Data, allocator_type>::TemporalSize() const
{
    return boost::numeric_cast<int32_t>(data_coll_sequence_.size());
}

template<typename Data, typename allocator_type>
Data SpatialTemporalData<Data, allocator_type>::Element(const int32_t spatial_idx, const int32_t temporal_idx) const
{
    this->VerifySpatialIdx(spatial_idx);
    this->VerifyTemporalIdx(temporal_idx);
    return data_coll_sequence_[temporal_idx][spatial_idx];
}

template<typename Data, typename allocator_type>
typename SpatialTemporalData<Data, allocator_type>::DataColl_t SpatialTemporalData<Data, allocator_type>::SpatialElements(
        const int32_t spatial_idx
        ) const
{
    this->VerifySpatialIdx(spatial_idx);
    DataColl_t spatial_coll;
    spatial_coll.reserve(this->TemporalSize());

    std::transform(
            boost::counting_iterator<int32_t>(0),
            boost::counting_iterator<int32_t>(this->TemporalSize()),
            std::back_inserter(spatial_coll),
            [&](const int32_t temporal_idx)
            {return data_coll_sequence_[temporal_idx][spatial_idx];}
            );

    return spatial_coll;
}

template<typename Data, typename allocator_type>
typename SpatialTemporalData<Data, allocator_type>::DataColl_t SpatialTemporalData<Data, allocator_type>::TemporalElements(
        const int32_t temporal_idx
        ) const
{
    this->VerifyTemporalIdx(temporal_idx);
    return data_coll_sequence_[temporal_idx];
}

template<typename Data, typename allocator_type>
void SpatialTemporalData<Data, allocator_type>::SetElement(
        const int32_t spatial_idx,
        const int32_t temporal_idx,
        const Data &data
        )
{
    this->VerifySpatialIdx(spatial_idx);
    this->VerifyTemporalIdx(temporal_idx);
    data_coll_sequence_[temporal_idx][spatial_idx] = data;
}

template<typename Data, typename allocator_type>
void SpatialTemporalData<Data, allocator_type>::SetSpatialElements(
        const int32_t spatial_idx,
        const DataColl_t &spatial_elements
        )
{
    this->VerifySpatialIdx(spatial_idx);
    this->VerifySpatialElementSize(
            boost::numeric_cast<int32_t>(spatial_elements.size())
            );
    std::for_each(
            boost::counting_iterator<int32_t>(0),
            boost::counting_iterator<int32_t>(this->TemporalSize()),
            [&](const int32_t temporal_idx)
            {data_coll_sequence_[temporal_idx][spatial_idx] = spatial_elements[temporal_idx];}
            );
}

template<typename Data, typename allocator_type>
void SpatialTemporalData<Data, allocator_type>::SetTemporalElements(
        const int32_t temporal_idx,
        const DataColl_t &temporal_elements
        )
{
    this->VerifyTemporalIdx(temporal_idx);
    this->VerifyTemporalElementSize(
            boost::numeric_cast<int32_t>(temporal_elements.size())
            );
    data_coll_sequence_[temporal_idx] = temporal_elements;
}

template<typename Data, typename allocator_type>
void SpatialTemporalData<Data, allocator_type>::Clear()
{
    data_coll_sequence_.clear();
}

template<typename Data, typename allocator_type>
void SpatialTemporalData<Data, allocator_type>::Resize(const int32_t spatial_size, const int32_t temporal_size)
{
    DataColl_t init_coll(spatial_size);
    data_coll_sequence_ = DataCollSeq_t(temporal_size, init_coll);
}

template<typename Data, typename allocator_type>
bool SpatialTemporalData<Data, allocator_type>::IsEmpty() const
{
    return data_coll_sequence_.empty();
}

// protected func.

// private func.

template<typename Data, typename allocator_type>
void SpatialTemporalData<Data, allocator_type>::VerifySpatialIdx(const int32_t spatial_idx) const
{
    if (spatial_idx >= this->SpatialSize())
    {
        LOGERR << "Invalid spaital index: " << spatial_idx << '\n'
            << "spatial num: " << this->SpatialSize() << '\n'
            << std::endl;
        assert(FALSE);
    }
}

template<typename Data, typename allocator_type>
void SpatialTemporalData<Data, allocator_type>::VerifyTemporalIdx(const int32_t temporal_idx) const
{
    if (temporal_idx >= this->TemporalSize())
    {
        LOGERR << "Invalid temporal index: " << temporal_idx << '\n'
            << "temporal num: " << this->TemporalSize() << '\n'
            << std::endl;
        assert(FALSE);
    }
}

template<typename Data, typename allocator_type>
void SpatialTemporalData<Data, allocator_type>::VerifySpatialElementSize(const int32_t spatial_element_size) const
{
    if (spatial_element_size != this->TemporalSize())
    {
        LOGERR << "Invalid sptial element size: " << spatial_element_size << '\n'
            << "correct spaital element size: " << this->TemporalSize() << '\n'
            << std::endl;
        assert(FALSE);
    }
}

template<typename Data, typename allocator_type>
void SpatialTemporalData<Data, allocator_type>::VerifyTemporalElementSize(const int32_t temporal_element_size) const
{
    if (temporal_element_size != this->SpatialSize())
    {
        LOGERR << "Invalid temporal element size: " << temporal_element_size << '\n'
            << "correct temporal element size: " << this->SpatialSize() << '\n'
            << std::endl;
        assert(FALSE);
    }
}

} // namespace math {

#endif // #ifndef _MATH_SPATIAL_TEMPORAL_DATA_H_
