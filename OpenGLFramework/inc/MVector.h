#pragma once
#include <map>
#include <vector>
#include <iostream>

namespace hiveGraphics
{
	//This container class is designed in order to achieve that find corresponding data by name fastly and gaurantee that data stored in containers are ordered. The type of data recommended is pointer.
	template <typename TDataType>
	class MVector
	{
	public:
		void	 push_back(const std::string &vDataName, const TDataType &vData);
		void	 push_back(const std::string &vDataName, const TDataType &vData, int vPointerFlag);
		template <typename Pred>
		void	 push_back(const std::string &vDataName, const TDataType &vData, Pred vPred);
		void	 reserve(size_t vCapacity);
		void	 clear();
		bool	 empty();
		bool	 empty() const;
		bool	 find(const std::string &vDataName);
		bool	 find(const std::string &vDataName) const;
		size_t	 size();
		size_t	 size() const;
		auto&	 operator[](int i);
		auto&	 operator[](const std::string &vDataName);
		auto&	 operator[](int i) const;

		auto	 begin();
		auto	 begin() const;
		auto	 end();
		auto	 end() const;

	private:
		std::vector<TDataType> m_DataSet;
		std::map<std::string, TDataType> m_DataMap;
	};
}

template <typename TDataType>
void hiveGraphics::MVector<TDataType>::push_back(const std::string &vDataName, const TDataType &vData)
{
	m_DataSet.insert(std::lower_bound(m_DataSet.begin(), m_DataSet.end(), vData, [](const TDataType &vPass1, const TDataType &vPass2) {return vPass1 < vPass2; }), vData);   //sort when insert;
	m_DataMap[vDataName] = vData;
}

template <typename TDataType>
void hiveGraphics::MVector<TDataType>::push_back(const std::string &vDataName, const TDataType &vData, int vPointerFlag)
{
	m_DataSet.insert(std::lower_bound(m_DataSet.begin(), m_DataSet.end(), vData, [](const TDataType &vPass1, const TDataType &vPass2) {return *vPass1 < *vPass2; }), vData);   //sort when insert;
	m_DataMap[vDataName] = vData;
}

template <typename TDataType>
template <typename Pred>
void hiveGraphics::MVector<TDataType>::push_back(const std::string &vDataName, const TDataType &vData, Pred vPred)
{
	m_DataSet.insert(std::lower_bound(m_DataSet.begin(), m_DataSet.end(), vData, vPred), vData);   //sort when insert;
}

template <typename TDataType>
auto& hiveGraphics::MVector<TDataType>::operator [](int i)
{
	return const_cast<TDataType&>(static_cast<const MVector&>(*this)[i]);
}

template <typename TDataType>
auto& hiveGraphics::MVector<TDataType>::operator [](const std::string &vDataName)
{
	_ASSERT(!vDataName.empty() && m_DataMap.find(vDataName) != m_DataMap.end());
	return m_DataMap[vDataName];
}

template <typename TDataType>
auto& hiveGraphics::MVector<TDataType>::operator [](int i) const 
{
	_ASSERT(i < m_DataSet.size() && i >= 0);
	return m_DataSet[i];
}

template <typename TDataType>
auto hiveGraphics::MVector<TDataType>::begin()
{
	return m_DataSet.begin();
}

template <typename TDataType>
auto hiveGraphics::MVector<TDataType>::begin() const
{
	return m_DataSet.begin();
}

template <typename TDataType>
auto hiveGraphics::MVector<TDataType>::end()
{
	return m_DataSet.end();
}

template <typename TDataType>
auto hiveGraphics::MVector<TDataType>::end() const
{
	return m_DataSet.end();
}

template <typename TDataType>
bool hiveGraphics::MVector<TDataType>::empty()
{
	return m_DataSet.empty() || m_DataMap.empty();
}

template <typename TDataType>
bool hiveGraphics::MVector<TDataType>::empty() const
{
	return m_DataSet.empty() || m_DataMap.empty();
}

template <typename TDataType>
void hiveGraphics::MVector<TDataType>::clear()
{
	m_DataSet.clear();
	m_DataMap.clear();
}

template <typename TDataType>
size_t hiveGraphics::MVector<TDataType>::size()
{
	return static_cast<const MVector&>(*this).size();
}

template <typename TDataType>
size_t hiveGraphics::MVector<TDataType>::size() const
{
	_ASSERT(m_DataMap.size() == m_DataSet.size);
	return m_DataSet.size();
}

template <typename TDataType>
void hiveGraphics::MVector<TDataType>::reserve(size_t vCapacity)
{
	_ASSERT(vCapacity >= 0);
	m_DataSet.reserve(vCapacity);
}

template <typename TDataType>
bool hiveGraphics::MVector<TDataType>::find(const std::string &vDataName)
{
	return m_DataMap.find(vDataName) != m_DataMap.end();
}

template <typename TDataType>
bool hiveGraphics::MVector<TDataType>::find(const std::string &vDataName) const
{
	return m_DataMap.find(vDataName) != m_DataMap.end();
}