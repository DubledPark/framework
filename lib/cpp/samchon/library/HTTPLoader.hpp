#pragma once
#include <samchon/API.hpp>

#include <samchon/ByteArray.hpp>
#include <samchon/library/URLVariables.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief A http, web-page loader
	 * 
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/library_data.png)
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API HTTPLoader
	{
	private:
		std::string url;

		/**
		 * @brief Method, Get or Post.
		 */
		int method;

		/**
		 * @brief Cookies got from remote web server.
		 */
		static HashMap<std::string, std::string> cookie_map;

	public:
		enum METHOD : int
		{
			GET = 1,
			POST = 2
		};

	public:
		/* ------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------ */
		/**
		 * @brief Construct from request method.
		 *
		 * @param method Get or Post.
		 */
		HTTPLoader(int method = POST);

		/**
		 * @brief Construct from request url and method.
		 *
		 * @param url Target url of remote web server.
		 * @param method Get or Post
		 */
		HTTPLoader(const std::string &, int method = POST);
		virtual ~HTTPLoader();

		/* ------------------------------------------------------------
			SETTERS & GETTERS
		------------------------------------------------------------ */
		/**
		 * @brief Set url.
		 */
		void setURL(const std::string &);

		/**
		 * @brief Set method.
		 */
		void setMethod(int);

		/**
		 * @brief Get url.
		 */
		auto getURL() const -> std::string;

		/**
		 * @brief Get method.
		 */
		auto getMethod() const -> int;

		/**
		 * @brief Get cookie.
		 */
		auto getCookie(const std::string &) const -> std::string;

		/* ------------------------------------------------------------
			LOADERS
		------------------------------------------------------------ */
		/**
		 * @brief Load data from target url.
		 *
		 * @details Loads binary data from target web server and address.
		 *
		 * @param data URLVariables containing parameters to request.
		 * @return Binary data fetched from remote web server.
		 */
		auto load(const URLVariables & = {}) const->ByteArray;
	};
};
};