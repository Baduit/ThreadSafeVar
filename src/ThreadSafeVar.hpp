#pragma once

#include <mutex>
#include <thread>
#include <functional>
#include <type_traits>

namespace ThreadSafe
{

template<typename T>
class Var
{
	public:
		Var() = default;
		Var(const T& t): _value(t) {}
		Var(T&& t): _value(std::move(t)) {}

		Var&	operator=(const T& t)
		{
			store(t);
			return *this;
		}

		Var&	operator=(T&& t)
		{
			store(std::move(t));
			return *this;
		}

		void	store(const T& t)
		{
			std::lock_guard lock(_mutex);
			_value = t;
		}

		void	store(T&& t)
		{
			std::lock_guard lock(_mutex);
			_value = std::move(t);
		}

	
		T		load() const
		{
			std::lock_guard lock(_mutex);
			return _value;
		}

		operator T() const
		{
			return load();
		}

		T&&		extract()
		{
			std::lock_guard lock(_mutex);
			return std::move(_value);
		}

		void swap(T& other)
		{
			std::lock_guard lock(_mutex);
			std::swap(_value, other);
		}

		template <typename F>
		using ReturnType = decltype(std::declval<F>()(std::declval<T>()));

		template <typename U> struct is_reference_or_pointer		: std::false_type {};
		template <typename U> struct is_reference_or_pointer<U&>	: std::true_type {};
		template <typename U> struct is_reference_or_pointer<U&&>	: std::true_type {};
		template <typename U> struct is_reference_or_pointer<U*>	: std::true_type {};

		template
		<
			typename F,
			typename = typename std::enable_if<!is_reference_or_pointer<ReturnType<F>>::value>::type
		>
		auto access(F&& callback) 
		{
			std::lock_guard lock(_mutex);
			return callback(_value);
		}
		

		template <typename U>
		using NoRefOrPointer = typename std::remove_pointer<typename std::remove_reference<U>::type>::type;

		template <typename F>
		auto	access_compliant(F&& callback) -> NoRefOrPointer<ReturnType<F>>
		{
			std::lock_guard lock(_mutex);
			return callback(_value);
		}
		
	private:
		T					_value;
		mutable std::mutex	_mutex;
};

}
