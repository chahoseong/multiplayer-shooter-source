#pragma once

#include "TypeTraits.h"

struct FInplace
{
	explicit FInplace() = default;
};
inline constexpr FInplace Inplace{};

struct FUnexpect
{
	explicit FUnexpect() = default;
};
inline constexpr FUnexpect Unexpect{};

template <typename T, typename E>
class TExpected
{
public:
	using ValueType = T;
	using ErrorType = E;
	
public:
	TExpected()
		: Data(MakeValue())
	{
	}
	
	TExpected(const T& Value)
		: Data(MakeValue(Value))
	{
	}
	
	TExpected(T&& Value)
		: Data(MakeValue(MoveTemp(Value)))
	{
	}
	
	TExpected(FInplace, const T& Value)
		: Data(MakeValue(Value))
	{
	}
	
	TExpected(FInplace, T&& Value)
		: Data(MakeValue(MoveTemp(Value)))
	{
	}
	
	TExpected(FUnexpect, const E& Error)
		: Data(MakeError(Error))
	{
	}
	
	TExpected(FUnexpect, E&& Error)
		: Data(MakeError(MoveTemp(Error)))
	{
	}
	
	template <typename F>
	constexpr auto AndThen(F&& Func) &
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Data.GetValue())>>;
		
		static_assert(is_specialization_v<ReturnType, TExpected>);
		static_assert(std::is_same_v<typename ReturnType::ErrorType, E>);
		static_assert(std::is_constructible_v<E, decltype(Data.GetError())>);
		
		if (HasValue())
		{
			return Invoke(Forward<F>(Func), Data.GetValue());
		}
		else
		{
			return ReturnType(Unexpect, Data.GetError());
		}
	}
	
	template <typename F>
	constexpr auto AndThen(F&& Func) const&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Data.GetValue())>>;
		
		static_assert(is_specialization_v<ReturnType, TExpected>);
		static_assert(std::is_same_v<typename ReturnType::ErrorType, E>);
		static_assert(std::is_constructible_v<E, decltype(Data.GetError())>);
		
		if (HasValue())
		{
			return Invoke(Forward<F>(Func), Data.GetValue());
		}
		else
		{
			return ReturnType(Unexpect, Data.GetError());
		}
	}
	
	template <typename F>
	constexpr auto AndThen(F&& Func) &&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(MoveTemp(Data.GetValue()))>>;
		
		static_assert(is_specialization_v<ReturnType, TExpected>);
		static_assert(std::is_same_v<typename ReturnType::ErrorType, E>);
		static_assert(std::is_constructible_v<E, decltype(MoveTemp(Data.GetError()))>);
		
		if (HasValue())
		{
			return Invoke(Forward<F>(Func), MoveTemp(Data.GetValue()));
		}
		else
		{
			return ReturnType(Unexpect, MoveTemp(Data.GetError()));
		}
	}
	
	template <typename F>
	constexpr auto AndThen(F&& Func) const &&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(MoveTemp(Data.GetValue()))>>;
		
		static_assert(is_specialization_v<ReturnType, TExpected>);
		static_assert(std::is_same_v<typename ReturnType::ErrorType, E>);
		static_assert(std::is_constructible_v<E, decltype(MoveTemp(Data.GetError()))>);

		if (HasValue())
		{
			return Invoke(Forward<F>(Func), MoveTemp(Data.GetValue()));
		}
		else
		{
			return ReturnType(Unexpect, MoveTemp(Data.GetValue()));
		}
	}
	
	template <typename F>
	constexpr auto Transform(F&& Func) &
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Data.GetValue())>>;
		
		static_assert(std::is_constructible_v<E, decltype(Data.GetError())>);
		
		if (HasValue())
		{
			if constexpr (std::is_void_v<ReturnType>)
			{
				Invoke(Forward<F>(Func), Data.GetValue());
				return TExpected<ReturnType, E>();
			}
			else
			{
				return TExpected<ReturnType, E>(Inplace, Invoke(Forward<F>(Func), Data.GetValue()));
			}
		}
		else
		{
			return TExpected<ReturnType, E>(Unexpect, Data.GetError());
		}
	}
	
	template <typename F>
	constexpr auto Transform(F&& Func) const&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Data.GetValue())>>;
		
		static_assert(std::is_constructible_v<E, decltype(Data.GetError())>);

		if (HasValue())
		{
			if constexpr (std::is_void_v<ReturnType>)
			{
				Invoke(Forward<F>(Func), Data.GetValue());
				return TExpected<ReturnType, E>();
			}
			else
			{
				return TExpected<ReturnType, E>(Inplace, Invoke(Forward<F>(Func), Data.GetValue()));
			}
		}
		else
		{
			return TExpected<ReturnType, E>(Unexpect, Data.GetError());
		}
	}
	
	template <typename F>
	constexpr auto Transform(F&& Func) &&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(MoveTemp(Data.GetValue()))>>;
		
		static_assert(std::is_constructible_v<E, decltype(MoveTemp(Data.GetError()))>);
		
		if (HasValue())
		{
			if constexpr (std::is_void_v<ReturnType>)
			{
				Invoke(Forward<F>(Func), MoveTemp(Data.GetValue()));
				return TExpected<ReturnType, E>();
			}
			else
			{
				auto Result = Invoke(Forward<F>(Func), MoveTemp(Data.GetValue()));
				return TExpected<ReturnType, E>(Inplace, MoveTemp(Result));
			}
		}
		else
		{
			return TExpected<ReturnType, E>(Unexpect, MoveTemp(Data.GetError()));
		}
	}
	
	template <typename F>
	constexpr auto Transform(F&& Func) const&&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(MoveTemp(Data.GetValue()))>>;
		
		static_assert(std::is_constructible_v<E, decltype(MoveTemp(Data.GetError()))>);
		
		if (HasValue())
		{
			if constexpr (std::is_void_v<ReturnType>)
			{
				Invoke(Forward<F>(Func), Data.GetValue());
				return TExpected<ReturnType, E>();
			}
			else
			{
				auto Result = Invoke(Forward<F>(Func), MoveTemp(Data.GetValue()));
				return TExpected<ReturnType, E>(Inplace, MoveTemp(Result));
			}
		}
		else
		{
			return TExpected<ReturnType, E>(Unexpect, MoveTemp(Data.GetError()));
		}
	}
	
	template <typename F>
	constexpr auto OrElse(F&& Func) &
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Data.GetError())>>;
		
		static_assert(is_specialization_v<ReturnType, TExpected>);
		static_assert(std::is_same_v<typename ReturnType::ValueType, T>);
		static_assert(std::is_constructible_v<T, decltype(Data.GetValue())>);
		
		if (HasValue())
		{
			return TExpected<ReturnType, E>(Inplace, Data.GetValue());
		}
		else
		{
			return Invoke(Forward<F>(Func), Data.GetError());
		}
	}
	
	template <typename F>
	constexpr auto OrElse(F&& Func) const&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Data.GetError())>>;
		
		static_assert(is_specialization_v<ReturnType, TExpected>);
		static_assert(std::is_same_v<typename ReturnType::ValueType, T>);
		static_assert(std::is_constructible_v<T, decltype(Data.GetValue())>);
		
		if (HasValue())
		{
			return TExpected<ReturnType, E>(Inplace, Data.GetValue());
		}
		else
		{
			return Invoke(Forward<F>(Func), Data.GetError());
		}
	}
	
	template <typename F>
	constexpr auto OrElse(F&& Func) &&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(MoveTemp(Data.GetError()))>>;
		
		static_assert(is_specialization_v<ReturnType, TExpected>);
		static_assert(std::is_same_v<typename ReturnType::ValueType, T>);
		static_assert(std::is_constructible_v<T, decltype(MoveTemp(Data.GetValue()))>);
		
		if (HasValue())
		{
			return TExpected<ReturnType, E>(Inplace, MoveTemp(Data.GetValue()));
		}
		else
		{
			return Invoke(Forward<F>(Func), MoveTemp(Data.GetError()));
		}
	}
	
	template <typename F>
	constexpr auto OrElse(F&& Func) const&&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(MoveTemp(Data.GetError()))>>;
		
		static_assert(is_specialization_v<ReturnType, TExpected>);
		static_assert(std::is_same_v<typename ReturnType::ValueType, T>);
		static_assert(std::is_constructible_v<T, decltype(MoveTemp(Data.GetValue()))>);
		
		if (HasValue())
		{
			return TExpected<ReturnType, E>(Inplace, MoveTemp(Data.GetValue()));
		}
		else
		{
			return Invoke(Forward<F>(Func), MoveTemp(Data.GetError()));
		}
	}
	
	template <typename F>
	constexpr auto With(F&& Func) const
	{
		if (HasValue())
		{
			Invoke(Forward<F>(Func), Data.GetValue());
		}
		return *this;
	}
	
	template <typename F>
	constexpr auto Alert(F&& Func) const
	{
		if (HasError())
		{
			Invoke(Forward<F>(Func), Data.GetError());
		}
		return *this;
	}
	
	void SetValue(const T& Value) &
	{
		Data = MakeValue(Value);
	}
	
	void SetValue(T&& Value) &
	{
		Data = MakeValue(MoveTemp(Value));
	}
	
	constexpr const T& GetValue() const&
	{
		return Data.GetValue();
	}
	
	constexpr const T&& GetValue() const&&
	{
		return MoveTemp(Data.GetValue());
	}
	
	constexpr bool HasValue() const noexcept
	{
		return Data.HasValue();
	}
	
	void SetError(const E& Error) & noexcept
	{
		Data = MakeError(Error);
	}
	
	void SetError(E&& Error) && noexcept
	{
		Data = MakeError(MoveTemp(Error));
	}
	
	constexpr const E& GetError() const& noexcept
	{
		return Data.GetError();
	}
	
	constexpr const E&& GetError() const&& noexcept
	{
		return MoveTemp(Data.GetError());
	}
	
	constexpr bool HasError() const noexcept
	{
		return Data.HasError();
	}
	
	explicit operator bool() const noexcept
	{
		return HasValue();
	}
	
	constexpr T* operator->() noexcept
	{
		return std::addressof(Data.GetValue());
	}
	
	constexpr const T* operator->() const noexcept
	{
		return std::addressof(Data.GetValue());
	}
	
	constexpr T& operator*() & noexcept
	{
		return Data.GetValue();
	}
	
	constexpr const T& operator*() const& noexcept
	{
		return Data.GetValue();
	}
	
	constexpr T&& operator*() && noexcept
	{
		return MoveTemp(Data.GetValue());
	}
	
	constexpr const T&& operator*() const&& noexcept
	{
		return MoveTemp(Data.GetValue());
	}
	
private:
	TValueOrError<T,E> Data;
};

template <typename E>
class TExpected<void, E>
{
public:
	using ValueType = void;
	using ErrorType = E;
	
public:
	TExpected()
	{
	}
	
	TExpected(FUnexpect, const E& Error)
		: Error(Error)
	{
	}
	
	TExpected(FUnexpect, E&& Error)
		: Error(MoveTemp(Error))
	{
	}
	
	template <typename F>
	constexpr auto AndThen(F&& Func) &
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Func)>>;
		
		static_assert(is_specialization_v<ReturnType, TExpected>);
		static_assert(std::is_same_v<typename ReturnType::ErrorType, E>);
		static_assert(std::is_constructible_v<E, decltype(Error.GetValue())>);
		
		if (HasValue())
		{
			return Invoke(Forward<F>(Func));
		}
		else
		{
			return TExpected<ReturnType, E>(Unexpect, Error.GetValue());
		}
	}
	
	template <typename F>
	constexpr auto AndThen(F&& Func) const&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Func)>>;
		
		static_assert(is_specialization_v<ReturnType, TExpected>);
		static_assert(std::is_same_v<typename ReturnType::ErrorType, E>);
		static_assert(std::is_constructible_v<E, decltype(Error.GetValue())>);
		
		if (HasValue())
		{
			return Invoke(Forward<F>(Func));
		}
		else
		{
			return TExpected<ReturnType, E>(Unexpect, Error.GetValue());
		}
	}
	
	template <typename F>
	constexpr auto AndThen(F&& Func) &&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Func)>>;
		
		static_assert(is_specialization_v<ReturnType, TExpected>);
		static_assert(std::is_same_v<typename ReturnType::ErrorType, E>);
		static_assert(std::is_constructible_v<E, decltype(MoveTemp(Error.GetValue()))>);
		
		if (HasValue())
		{
			return Invoke(Forward<F>(Func));
		}
		else
		{
			return TExpected<ReturnType, E>(Unexpect, MoveTemp(Error.GetValue()));
		}
	}
	
	template <typename F>
	constexpr auto AndThen(F&& Func) const&&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Func)>>;
		
		static_assert(is_specialization_v<ReturnType, TExpected>);
		static_assert(std::is_same_v<typename ReturnType::ErrorType, E>);
		static_assert(std::is_constructible_v<E, decltype(MoveTemp(Error.GetValue()))>);
		
		if (HasValue())
		{
			return Invoke(Forward<F>(Func));
		}
		else
		{
			return TExpected<ReturnType, E>(Unexpect, MoveTemp(Error.GetValue()));
		}
	}
	
	template <typename F>
	constexpr auto Transform(F&& Func) &
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Func)>>;
		
		static_assert(std::is_constructible_v<E, decltype(Error.GetValue())>);
		
		if (HasValue())
		{
			if constexpr (std::is_void_v<ReturnType>)
			{
				Invoke(Forward<F>(Func));
				return TExpected<ReturnType, E>();
			}
			else
			{
				auto Result = Invoke(Forward<F>(Func));
				return TExpected<ReturnType, E>(Inplace, MoveTemp(Result));
			}
		}
		else
		{
			return TExpected<ReturnType, E>(Unexpect, Error.GetValue());
		}
	}
	
	template <typename F>
	constexpr auto Transform(F&& Func) const&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Func)>>;
		
		static_assert(std::is_constructible_v<E, decltype(Error.GetValue())>);
		
		if (HasValue())
		{
			if constexpr (std::is_void_v<ReturnType>)
			{
				Invoke(Forward<F>(Func));
				return TExpected<ReturnType, E>();
			}
			else
			{
				auto Result = Invoke(Forward<F>(Func));
				return TExpected<ReturnType, E>(Inplace, MoveTemp(Result));
			}
		}
		else
		{
			return TExpected<ReturnType, E>(Unexpect, Error.GetValue());
		}
	}
	
	template <typename F>
	constexpr auto Transform(F&& Func) &&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Func)>>;
		
		static_assert(std::is_constructible_v<E, decltype(MoveTemp(Error.GetValue()))>);
		
		if (HasValue())
		{
			if constexpr (std::is_void_v<ReturnType>)
			{
				Invoke(Forward<F>(Func));
				return TExpected<ReturnType, E>();
			}
			else
			{
				auto Result = Invoke(Forward<F>(Func));
				return TExpected<ReturnType, E>(Inplace, MoveTemp(Result));
			}
		}
		else
		{
			return TExpected<ReturnType, E>(Unexpect, MoveTemp(Error.GetValue()));
		}
	}
	
	template <typename F>
	constexpr auto Transform(F&& Func) const&&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Func)>>;
		
		static_assert(std::is_constructible_v<E, decltype(MoveTemp(Error.GetValue()))>);
		
		if (HasValue())
		{
			if constexpr (std::is_void_v<ReturnType>)
			{
				Invoke(Forward<F>(Func));
				return TExpected<ReturnType, E>();
			}
			else
			{
				auto Result = Invoke(Forward<F>(Func));
				return TExpected<ReturnType, E>(Inplace, MoveTemp(Result));
			}
		}
		else
		{
			return TExpected<ReturnType, E>(Unexpect, MoveTemp(Error.GetValue()));
		}
	}
	
	template <typename F>
	constexpr auto OrElse(F&& Func) &
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Func)>>;
		
		if (HasValue())
		{
			return TExpected<ReturnType, E>();
		}
		else
		{
			return Invoke(Forward<F>(Func), GetError());
		}
	}
	
	template <typename F>
	constexpr auto OrElse(F&& Func) const&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Func)>>;
		
		if (HasValue())
		{
			return TExpected<ReturnType, E>();
		}
		else
		{
			return Invoke(Forward<F>(Func), GetError());
		}
	}
	
	template <typename F>
	constexpr auto OrElse(F&& Func) &&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Func)>>;
		
		if (HasValue())
		{
			return TExpected<ReturnType, E>();
		}
		else
		{
			return Invoke(Forward<F>(Func), MoveTemp(Error.GetValue()));
		}
	}
	
	template <typename F>
	constexpr auto OrElse(F&& Func) const&&
	{
		using ReturnType = std::remove_cvref_t<TInvokeResult_T<F, decltype(Func)>>;
		
		if (HasValue())
		{
			return TExpected<ReturnType, E>();
		}
		else
		{
			return Invoke(Forward<F>(Func), MoveTemp(GetError()));
		}
	}
	
	constexpr bool HasValue() const noexcept
	{
		return !Error.IsSet();
	}
	
	constexpr void SetError(const E& NewError) & noexcept
	{
		Error = NewError;
	}
	
	constexpr const E& GetError() const& noexcept
	{
		return Error;
	}
	
	constexpr void SetError(E&& NewError) && noexcept
	{
		Error = MoveTemp(NewError);
	}
	
	constexpr const E&& GetError() const&& noexcept
	{
		return MoveTemp(Error);
	}
	
	explicit operator bool() const noexcept
	{
		return HasValue();
	}
	
	constexpr void operator*() const noexcept
	{
		check(HasValue());
	}
	
private:
	TOptional<E> Error;
};

template <typename T, typename F, typename E> requires std::is_invocable_v<std::decay_t<F>, std::decay_t<T>>
TExpected<T, E> Expect(T&& Context, F&& Predicate, E&& Error)
{
	if (Invoke(Forward<F>(Predicate), Forward<T>(Context)))
	{
		return TExpected<T, E>(Forward<T>(Context));
	}
	else
	{
		return TExpected<T,E>(Unexpect, Forward<E>(Error));
	}
}

template <typename T, typename E> requires !std::is_invocable_v<std::decay_t<E>, std::decay_t<T>>
TExpected<T, E> Expect(T&& Context, E&& Error)
{
	if (Context)
	{
		return TExpected<T, E>(Forward<T>(Context));
	}
	else
	{
		return TExpected<T,E>(Unexpect, Forward<E>(Error));
	}
}
