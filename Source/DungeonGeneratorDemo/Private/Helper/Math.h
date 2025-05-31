/**
@author		Shun Moriya


*/

#pragma once

namespace math
{
	template<typename T>
	static constexpr T Square(const T t) noexcept
	{
		return t * t;
	}

	template<typename T>
	static constexpr T Pi() noexcept
	{
		return static_cast<T>(3.14159265358979323846264338327950288);
	}

	template<typename T>
	static constexpr T PiQuarter() noexcept
	{
		return Pi<T>() * static_cast<T>(0.25);
	}

	template<typename T>
	static constexpr T PiHalf() noexcept
	{
		return Pi<T>() * static_cast<T>(0.5);
	}

	template<typename T>
	static constexpr T Pi2() noexcept
	{
		return Pi<T>() * static_cast<T>(2.);
	}

	template<typename T>
	static constexpr T ToRadian(const T degree)
	{
		return degree * (Pi<T>() / static_cast<T>(180.));
	}

	template<typename T>
	static constexpr T ToDegree(const T radian)
	{
		return radian * (static_cast<T>(180.0) / Pi<T>());
	}

	template<typename T>
	static constexpr T RoundDegree(const T degree)
	{
		// https://stackoverflow.com/questions/11498169/dealing-with-angle-wrap-in-c-code
		if (degree > static_cast<T>(0.))
			return degree - static_cast<T>(360.) * std::floor(degree * static_cast<T>(1. / 360.));
		else
			return degree + static_cast<T>(360.) * std::floor(degree * static_cast<T>(1. / -360.));
	}

	template<typename T>
	static constexpr T RoundRadian(const T radian)
	{
		// https://stackoverflow.com/questions/11498169/dealing-with-angle-wrap-in-c-code
		if (radian > static_cast<T>(0.))
			return radian - Pi2<T>() * std::floor(radian * (static_cast<T>(1.) / Pi2<T>()));
		else
			return radian + Pi2<T>() * std::floor(radian * (static_cast<T>(1.) / -Pi2<T>()));
	}

	template<typename T>
	static constexpr T RoundDegreeWithSign(const T degree)
	{
		// https://stackoverflow.com/questions/11498169/dealing-with-angle-wrap-in-c-code
		if (degree > static_cast<T>(0.))
			return degree - static_cast<T>(360.) * std::floor((degree + static_cast<T>(180.)) * static_cast<T>(1. / 360.));
		else
			return degree + static_cast<T>(360.) * std::floor((degree - static_cast<T>(180.)) * static_cast<T>(1. / -360.));
	}

	template<typename T>
	static constexpr T RoundRadianWithSign(const T radian)
	{
		// https://stackoverflow.com/questions/11498169/dealing-with-angle-wrap-in-c-code
		if (radian > static_cast<T>(0.))
			return radian - Pi2<T>() * std::floor((radian + Pi<T>()) * (static_cast<T>(1.) / Pi2<T>()));
		else
			return radian + Pi2<T>() * std::floor((radian - Pi<T>()) * (static_cast<T>(1.) / -Pi2<T>()));
	}

	template<typename T>
	static constexpr T DegreeDistance(const T a, const T b)
	{
		return std::abs(RoundDegreeWithSign(a - b));
	}

	template<typename T>
	static constexpr T RadianDistance(const T a, const T b)
	{
		return std::abs(RoundRadianWithSign(a - b));
	}

	/**
	60FPS����ɂ���Chase�̃��V�I���擾���܂�
	\param[in]	ratio			60FPS�x�[�X��chase���V�I
	\param[in]	deltaSeconds	�o�ߕb��
	\return		chase���V�I
	*/
	template<typename T>
	static constexpr T CalcChaseRatio(const T ratio, const T deltaSeconds) noexcept
	{
		static constexpr T baseTime = static_cast<T>(1.) / static_cast<T>(60.);
		const T frameScale = deltaSeconds / baseTime;
		return static_cast<T>(1.) - std::pow(static_cast<T>(1.) - ratio, frameScale);
	}

	/**
	���l�������Ń^�[�Q�b�g�ɋ߂Â���B
	�߂Â��X�s�[�h�́A�ŏ��͑��������񂾂�x���Ȃ�B
	ratio �� 0.0f �ȏ� 1.0f �ȉ��̒l���w�肷��B
	ratio �� 1.0f ���ƈ�u�Ń^�[�Q�b�g�ɋ߂Â��Aratio ���������Ǝ��Ԃ�������B
	ratio ���������قǁA�߂Â����ŏI�I�Ȓl�ƃ^�[�Q�b�g����v�����A�덷��������B
	\param[inout]	now		���݂̒l
	\param[in]		target	�ڕW�Ƃ���l
	\param[in]		ratio	�ǂ̂悤�Ȋ����ŕ⊮���邩�Ba�̊�����1-ratio�Ab�̊�����ratio�ƂȂ�܂��B
	\param[in]		deltaSeconds	�f���^�b
	\param[in]		eps		�덷
	\return		�߂Â������ʁA�^�[�Q�b�g�ɓ��B���Ă���inow �̎w���悪 target �ɓ������j�Ƃ��^�A
				�^�[�Q�b�g�ɓ��B���Ă��Ȃ��inow �̎w���悪 target �Ɠ������Ȃ��j�Ƃ��U
	*/
	template<typename T>
	static constexpr bool ChaseDecrease(T& now, const T target, const T ratio, const T deltaSeconds, const T eps = std::numeric_limits<T>::epsilon())
	{
		const T length = target - now;
		if (std::abs(length) <= eps)
		{
			now = target;
			return true;
		}

		const T normalizedRatio = CalcChaseRatio(ratio, deltaSeconds);
		now += length * normalizedRatio;

		return false;
	}

	/**
	�������Ȃ���^�[�Q�b�g�ɋ߂Â���B
	�߂Â��X�s�[�h�́A�ŏ��͑��������񂾂�x���Ȃ�B
	ratio �� 0.0f �ȏ� 1.0f �ȉ��̒l���w�肷��B
	ratio �� 1.0f ���ƈ�u�Ń^�[�Q�b�g�ɋ߂Â��Aratio ���������Ǝ��Ԃ�������B
	*/
	template<typename T>
	bool ChaseDecreaseAngle(T& now, const T target, const float ratio, const float deltaSeconds, const T tolerance = 3.14159265359f / 180.f)
	{
		const float distance = RoundRadianWithSign(target - now);
		if (std::abs(distance) <= tolerance)
		{
			now = target;
			return true;
		}

		now += distance * CalcChaseRatio(ratio, deltaSeconds);
		now = RoundRadianWithSign(now);

		return false;
	}
}
