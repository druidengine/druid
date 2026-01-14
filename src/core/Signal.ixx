module;

#include <functional>
#include <vector>

export module druid.core.Signal;

export namespace druid::core
{
	/// @brief Primary template declaration for Signal.
	///
	/// This primary template is intentionally left undefined and is specialized
	/// for function-signature types of the form `R(Args...)`.
	///
	/// @tparam T Function-signature type (e.g., `void(int, float)`).
	/// @note Only the specialization for function types is valid.
	template <typename T>
	class Signal;

	/// @brief Lightweight signal/callback wrapper for function-signature types.
	///
	/// @tparam R Return type of the signal invocation.
	/// @tparam Args Argument types accepted by the signal.
	///
	/// `Signal` provides a minimal observer-style mechanism where a single
	/// callable can be connected and later invoked. It is intentionally simple:
	/// - Only one callback is stored at a time.
	/// - Invocation is safe if no callback is connected.
	/// - Exceptions thrown by the callback are caught and suppressed.
	///
	/// This class serves as a thin wrapper around `std::function` while preserving
	/// a function-signature–based interface.
	///
	/// @note
	/// This wrapper exists primarily to ensure that RTTI / type information for
	/// the function signature is emitted in translation units that include this
	/// header. This helps avoid missing-symbol or RTTI-related issues on toolchains
	/// with limited or incomplete C++20 module support.
	template <typename R, typename... Args>
	class Signal<R(Args...)>
	{
	public:
		/// @brief Connect a callback to this signal.
		///
		/// Any previously connected callback is replaced. The provided callable
		/// must be invocable with the signal's argument types.
		///
		/// @tparam Callback Callable type compatible with `R(Args...)`.
		/// @param callback Callable object to store and invoke.
		template <typename Callback>
		auto connect(Callback callback) -> void
		{
			signals_.emplace_back(std::forward<Callback>(callback));
		}

		/// @brief Invoke the connected callback, if any.
		///
		/// If no callback is connected, this function is a no-op.
		/// Any `std::exception` thrown by the callback is caught and ignored.
		///
		/// @param args Arguments forwarded to the connected callback.
		auto operator()(Args... args) const noexcept -> void
		{
			try
			{
				for (auto&& signal : signals_) {

					if(!signal) {
						continue;
					}

					signal(std::forward<Args>(args)...);
				}
			}
			catch (const std::exception& e)
			{
				// Exceptions are intentionally suppressed to prevent
				// signal dispatch from destabilizing the engine.
				(void)e;
			}
		}

	private:
		/// @brief Stored callback function.
		///
		/// An empty `std::function` indicates that no callback is connected.
		std::vector<std::function<R(Args...)>> signals_{};
	};
}
