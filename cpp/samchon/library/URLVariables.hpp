#pragma once
#include <samchon/API.hpp>

#include <samchon/Map.hpp>
#include <samchon/WeakString.hpp>

namespace samchon
{
	namespace library
	{
		/**
		 * @brief URLVariables class is for representing variables of HTTP
		 *
		 * @details
		 * <p> URLVariables class allows you to transfer variables between an application and server.
		 * When transfering, URLVariables will be converted to a URI string. </p>
		 *	\li URI: Uniform Resource Identifier
		 *
		 * <p> Use URLVariabels objects with methods of HTTPLoader class. </p>
		 *
		 * @image html cpp/subset/library_http.png
		 * @image latex cpp/subset/library_http.png
		 *
		 * <h4> Example code </h4>
		 * @includelineno url_variables/main.cpp
		 *
		 * @author Jeongho Nam
		 */
		class SAMCHON_FRAMEWORK_API URLVariables
			: public Map<std::string, std::string>
		{
		private:
			typedef Map<std::string, std::string> super;

		public:
			/* ------------------------------------------------------------
				CONSTRUCTORS
			 ------------------------------------------------------------ */
			//using super::super;

			/**
			 * @brief Default Constructor
			 */
			URLVariables();
			
			/**
			 * @brief Constructor by a string representing encoded properties
			 * @details Converts the variable string to properties of the specified URLVariables object.
			 *
			 *	\li URLVariables(\"id=jhnam88&name=Jeongho+Nam") => {{\"id\", \"jhnam88\"}, {\"name\", \"Jeongho Nam\"}}
			 * 
			 * @param A uri-encoded string containing pair of properties
			 */
			URLVariables(const WeakString &flashVars);
			
		public:
			/**
			 * @brief Encode a string into a valid URI
			 * @details Encodes a string to follow URI standard format.
			 *
			 * @param A string to encode to URI
			 * @return A string converted to URI
			 */
			static auto encode(const WeakString &) -> std::string;

			/**
			 * @brief Decode a URI string
			 * @details Decodes a URI string to its original
			 *
			 * @param A string encoded
			 * @return A string decoded from URI
			 */
			static auto decode(const WeakString &) -> std::string;

		private:
			/* ------------------------------------------------------------
				URI ENCODING & DECONDING
			------------------------------------------------------------ */
			static auto toHex(const char ch) -> char;
			static auto fromHex(const char ch) -> char;

		public:
			/* ------------------------------------------------------------
				TO_STRING
			------------------------------------------------------------ */
			/**
			 * @brief Get the string representing URLVariables
			 * @details Returns a string object representing URLVariables following the URI\n
			 * 
			 *	\li URLVariables({{"id", "jhnam88"}, {"name", "Jeongho Nam"}}).toString() => "id=jhnam88&name=Jeongho+Nam"
			 *
			 * @return A string representing URLVariables following the URI
			 */
			auto toString() const -> std::string;
		};
	};
};