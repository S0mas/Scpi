#pragma once
#include <cstddef>
#include <typeinfo>
#include <type_traits>
#include <utility>

template <size_t arg1, size_t ... others>
struct constexprMax;

template <size_t arg>
struct constexprMax<arg> {
	static const size_t value = arg;
};

template <size_t arg1, size_t arg2, size_t ... others>
struct constexprMax<arg1, arg2, others...> {
	static const size_t value = arg1 >= arg2 ? constexprMax<arg1, others...>::value :
		constexprMax<arg2, others...>::value;
};

template<typename... Ts>
struct variantHelper;

template<typename T, typename... Ts>
struct variantHelper<T, Ts...> {
	inline static void copy(size_t typeId, const void* oldValue, void* newValue) {
		if (typeId == typeid(T).hash_code())
			new (newValue) T(*reinterpret_cast<const T*>(oldValue));
		else
			variantHelper<Ts...>::copy(typeId, oldValue, newValue);
	}

	inline static void move(size_t typeId, void* oldValue, void* newValue) noexcept
	{
		if (typeId == typeid(T).hash_code())
			new (newValue) T(std::move(*reinterpret_cast<T*>(oldValue)));
		else
			variantHelper<Ts...>::move(typeId, oldValue, newValue);
	}

	inline static void destroy(size_t typeId, void* data) noexcept {
		if (typeId == typeid(T).hash_code())
			reinterpret_cast<T*>(data)->~T();
		else
			variantHelper<Ts...>::destroy(typeId, data);
	}
};

template<> 
struct variantHelper<> {
	inline static void destroy(size_t id, void* data) noexcept { }
	inline static void move(size_t old_t, void* old_v, void* new_v) noexcept { }
	inline static void copy(size_t old_t, const void* old_v, void* new_v) { }
};

template<typename...> struct disjunction : std::false_type { };
template<typename T> struct disjunction<T> : T { };
template<typename T, class... Rest>
struct disjunction<T, Rest...>
	: std::conditional_t<bool(T::value), T, disjunction<Rest...>> { };

template<typename T, typename... Ts>
constexpr bool contains() noexcept {
	return disjunction<std::is_same<T, Ts>...>::value;
}

template<typename... Ts>
class buVariant {
	static const size_t dataSize = constexprMax<sizeof(Ts)...>::value;
	static const size_t dataAlign = constexprMax<alignof(Ts)...>::value;
	using rawData = std::aligned_storage_t<dataSize, dataAlign>;

	using helper = variantHelper<Ts...>;

	static inline size_t invalidType() noexcept {
		return typeid(void).hash_code();
	}

	size_t typeId;
	rawData data;
public:
	buVariant() : typeId(invalidType()) {}

	buVariant(const buVariant<Ts...>& other) : typeId(other.typeId) {
		helper::copy(other.typeId, &other.data, &data);
	}

	buVariant(buVariant<Ts...>&& other) noexcept : typeId(other.typeId)  {
		helper::move(other.typeId, &other.data, &data);
	}

	buVariant<Ts...>& operator= (buVariant<Ts...> other) noexcept {
		std::swap(typeId, other.typeId);
		std::swap(data, other.data);
		return *this;
	}

	buVariant<Ts...>& operator= (buVariant<Ts...>&& other) noexcept {
		std::swap(typeId, other.typeId);
		std::swap(data, other.data);
		return *this;
	}

	~buVariant() { 
		helper::destroy(typeId, &data);
	}

	template<typename T, typename... Args>
	void set(Args&& ... args) {
		if (contains<T, Ts...>()) {
			helper::destroy(typeId, &data);
			new (&data) T(std::forward<Args>(args)...);
			typeId = typeid(T).hash_code();
		}
		else
			throw std::bad_typeid();
	}

	template<typename T>
	T& get() {
		if (typeId == typeid(T).hash_code())
			return *reinterpret_cast<T*>(&data);
		else
			throw std::bad_cast();
	}

	template<typename T>
	const T& get() const {
		if (typeId == typeid(T).hash_code())
			return *reinterpret_cast<const T*>(&data);
		else
			throw std::bad_cast();
	}

	template<typename T>
	bool is() const noexcept {
		return (typeId == typeid(T).hash_code());
	}

	size_t getTypeId() const noexcept {
		return typeId;
	}


	bool valid() const noexcept {
		return (typeId != invalidType());
	}
};

template <typename Visitor, typename Variant>
void visitHelp(const Visitor& visitor, const Variant& variant) noexcept {}

template <typename Visitor, typename Variant, typename T, typename... Rest>
void visitHelp(const Visitor& visitor, const Variant& variant) {
	if (variant. template is<T>())
		visitor(variant. template get<T>());
	else
		visitHelp<Visitor, Variant, Rest...>(visitor, variant);
}


template<typename Visitor, typename... Ts>
void visit(const Visitor& visitor, const buVariant<Ts...>& variant) noexcept {
	visitHelp<Visitor, buVariant<Ts...>, Ts...>(visitor, variant);
}
