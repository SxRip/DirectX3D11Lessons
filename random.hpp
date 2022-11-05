#pragma once
#include <random>
#define NOMINMAX
#pragma warning(disable : 4244)

class random
{
public:
	using DLONG = long long;
	using limitsDOUBLE = std::numeric_limits<double>;
	using limitsDLONG = std::numeric_limits<DLONG>;
	using limitsINT = std::numeric_limits<int>;

	using distributionINT = std::uniform_int_distribution<>;
	using distributionDLONG = std::uniform_int_distribution<DLONG>;
	using distributionDOUBLE = std::uniform_real_distribution<>;

	random(const random&) = delete;
	random(const random&&) = delete;

	random& operator=(const random&) = delete;
	random&& operator=(const random&&) = delete;


	random()
		: _mtValues(_randomDevice()), _mtValues64(_randomDevice()),
		_randomGeneratorDOUBLE{ limitsDOUBLE::min(), limitsDOUBLE::max() },
		_randomGeneratorINT{ limitsINT::min(), limitsINT::max() },
		_randomGeneratorDLONG{ limitsDLONG::min(), limitsDLONG::max() }
	{}

	template <class value_type>
	random(value_type from, value_type to)
		: _mtValues(_randomDevice()), _mtValues64(_randomDevice()),
		_randomGeneratorINT{ from, to },
		_randomGeneratorDLONG{ from, to },
		_randomGeneratorDOUBLE{ from, to }
	{}

	inline DLONG get_longlong() noexcept { return _randomGeneratorDLONG(_mtValues64); }
	inline double get_double() noexcept { return _randomGeneratorDOUBLE(_mtValues); }
	inline int get_int() noexcept { return _randomGeneratorINT(_mtValues); }

	inline DLONG max() const noexcept { return _randomGeneratorDLONG.max(); }
	inline double max_double() const noexcept { return _randomGeneratorDOUBLE.max(); }

	inline DLONG min() const noexcept { return _randomGeneratorDLONG.min(); }
	inline double min_double() const noexcept { return _randomGeneratorDOUBLE.min(); }

	inline std::pair<DLONG, DLONG> minmax() const noexcept
	{
		return { _randomGeneratorDLONG.min(), _randomGeneratorDLONG.max() };
	};

	template <class value_type>
	inline void set_range(value_type from, value_type to) noexcept
	{
		if (from > to)
			std::swap(from, to);

		_change_distribution_range(_randomGeneratorDOUBLE, from, to);
		_change_distribution_range(_randomGeneratorINT, from, to);
		_change_distribution_range(_randomGeneratorDLONG, from, to);
	}

private:

	template<class distribution, class value_type>
	void _change_distribution_range(distribution& distributionObj,
		value_type from, value_type to)
	{
		distributionObj.param(distribution::param_type(from, to));
	}

	std::random_device _randomDevice;
	std::mt19937 _mtValues;
	std::mt19937_64 _mtValues64;

	distributionDOUBLE _randomGeneratorDOUBLE;
	distributionINT _randomGeneratorINT;
	distributionDLONG _randomGeneratorDLONG;
};