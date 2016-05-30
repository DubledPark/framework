﻿#pragma once
#include <samchon/HashMap.hpp>

#include <vector>
#include <string>
#include <queue>
#include <memory>
#include <sstream>

#include <samchon/WeakString.hpp>
#include <samchon/library/Math.hpp>

namespace samchon
{
namespace library
{
	class XML;
	typedef std::vector<std::shared_ptr<XML>> XMLList;

	/**
	 * @brief XML is a class representing xml object
	 *
	 * @details 
	 * <p> The XML class provides methods and properties for working with XML objects. </p>
	 * 
	 * <p> The XML class (along with the XMLList and Namespace) implements 
	 * the powerful XML-handling standard defined in ECMAScript for XML (E4X) specification. </p>
	 *
	 * <p> XML class has a recursive, hierarchical relationship. </p>
	 * 
	 * <p> All XML objects're managed by shared_ptr. </p>
	 *	\li XML contains XMLList from dictionary of shared pointer<XMLList>
	 *  \li XMLList contains XML from vector of shared pointer<XML>
	 *  \li Even if user creates an XML object directly, it's the basic principle to use shared pointer
	 * 
	 * @image html cpp/subset/library_xml.png
	 * @image latex cpp/subset/library_xml.png
	 *
	 * @note 
	 * <p> Parsing comment is not supported yet. </p>
	 * <p> It's not recommeded to creating an XML object which is not being managed by shared pointer. </p>
	 * 
	 * @warning 
	 * <p> Do not abuse values for expressing member variables. </p>
	 *
	 * <table>
	 *	<tr>
	 *		<th>Standard Usage</th>
	 *		<th>Non-standard usage abusing value</th>
	 *	</tr>
	 *	<tr>
	 *		<td>
	 *			\<memberList\>\n
	 *			&nbsp;&nbsp;&nbsp;&nbsp; \<member id='jhnam88' name='Jeongho+Nam' birthdate='1988-03-11' /\>\n
	 *			&nbsp;&nbsp;&nbsp;&nbsp; \<member id='master' name='Administartor' birthdate='2011-07-28' /\>\n
	 *			\</memberList\>
	 *		</td>
	 *		<td>
	 *			\<member\>\n
	 *			&nbsp;&nbsp;&nbsp;&nbsp; \<id\>jhnam88\</id\>\n
	 *			&nbsp;&nbsp;&nbsp;&nbsp; \<name\>Jeongho+Nam\</name\>\n
	 *			&nbsp;&nbsp;&nbsp;&nbsp; \<birthdate\>1988-03-11\</birthdate\>\n
	 *			\</member\>
	 *		</td>
	 *	</tr>
	 * </table>
	 * 
	 * @includelineno example/xml/main.cpp
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class XML
		: public HashMap<std::string, std::shared_ptr<XMLList>>
	{
	private:
		typedef HashMap<std::string, std::shared_ptr<XMLList>> super;

		/**
		 * @brief Tag name
		 *
		 * @details
		 *	\li \<<b>tag</b> label='property' /\>: tag => \"tag\"
		 *  \li \<<b>price</b> high='1500' low='1300' open='1450' close='1320' /\>: tag => \"price\"
		 */
		std::string tag;

		/**
		 * @brief Value of the XML
		 *
		 * @details
		 *  \li \<parameter name='age' type='int'\><b>26</b>\</parameter\>: value => 26
		 *	\li \<price high='1500' low='1300' open='1450' close='1320' /\>: tag => null
		 */
		std::string value;

		/**
		 * @brief Properties belongs to the XML
		 *
		 * @details
		 * A Dictionary of properties accessing each property by its key.
		 *	\li \<price <b>high='1500' low='1300' open='1450' close='1320'</b> /\>:
		 *		properties => {{\"high\": 1500}, {\"low\": 1300}, {\"open\": 1450}, {\"close\", 1320}}
		 *	\li \<member <b>id='jhnam88' name='Jeongho+Nam' comment='Hello.+My+name+is+Jeongho+Nam'</b> \>:
		 *		properties => {{\"id\", \"jhnam88\"}, {\"name\", \"Jeongho Nam <http://samchon.org>\"}, {\"comment\", \"Hello. My name is Jeongho Nam <http://samchon.org>\"}}
		 */
		HashMap<std::string, std::string> properties;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 *
		 * @warning Declare XML to managed by shared pointer
		 */
		XML() : super()
		{
		};

		/**
		 * @brief Copy Constructor
		 *
		 * @details
		 * Not copying (shared) pointer of children xml objects,
		 * but copying the real objects of children xml
		 */
		XML(const XML &xml) : super()
		{
			tag = xml.tag;
			value = xml.value;
			properties = xml.properties;

			//COPYING CHILDREN OBJECTS
			for (auto it = xml.begin(); it != xml.end(); it++)
			{
				if (it->second->empty() == true)
					continue;

				std::shared_ptr<XMLList> xmlList(new XMLList());
				xmlList->reserve(it->second->size());

				for (size_t i = 0; i < it->second->size(); it++)
					xmlList->emplace_back(new XML(*it->second->at(i)));

				set(xmlList->at(0)->tag, xmlList);
			}
		};

		/**
		 * @brief Move Constructor
		 */
		XML(XML &&xml) : super(move(xml))
		{
			tag = move(xml.tag);
			value = move(xml.value);

			properties = move(xml.properties);
		};

		/**
		 * @brief Constructor by string
		 *
		 * @details
		 * Parses a string so that constructs an XML object
		 *
		 * @param str A string representing xml object
		 * @warning Declare XML to managed by shared pointer
		 */
		XML(WeakString wstr) : super()
		{
			if (wstr.find('<') == std::string::npos)
				return;

			//WHEN COMMENT IS
			std::string replaced_str;
			if (wstr.find("<!--") != std::string::npos)
			{
				std::queue<std::pair<size_t, size_t>> index_pair_queue;
				size_t begin_x = 0, end_x;

				//CONSTRUCT INDEXES
				replaced_str.reserve(wstr.size());
				while ((begin_x = wstr.find("<!--", begin_x)) != std::string::npos)
				{
					index_pair_queue.push({ begin_x, wstr.find("-->", begin_x + 1) + 3 });
					begin_x++;
				}

				//INSERT STRINGS
				begin_x = 0;
				while (index_pair_queue.empty() == false)
				{
					end_x = index_pair_queue.front().first;
					replaced_str.append(wstr.substring(begin_x, end_x).str());

					begin_x = index_pair_queue.front().second;
					index_pair_queue.pop();
				}
				replaced_str.append(wstr.substr(begin_x).str());

				//RE-REFERENCE
				wstr = replaced_str;
			}

			//ERASE HEADERS OF XML
			if (wstr.find("<?xml") != std::string::npos)
				wstr = wstr.between("?>");

			construct(wstr);
		};

	private:
		/**
		 * @brief Protected Constructor by string for child
		 *
		 * @details
		 * Parses a string so that creates an XML object
		 * It is called for creating children XML objects from parent XML object.
		 *
		 * @param parent Parent object who will contains this XML object
		 * @param str A string to be parsed
		 */
		XML(XML *parent, WeakString &wstr) : XML()
		{
			//	this->parent = parent;
			//	this->level = parent->level + 1;

			construct(wstr);
		};

		void construct(WeakString &wstr)
		{
			construct_key(wstr);
			construct_properties(wstr);

			if (construct_value(wstr) == true)
				construct_children(wstr);
		};

		void construct_key(WeakString &wstr)
		{
			size_t start_x = wstr.find("<") + 1;
			size_t end_x = calc_min_index
				({
					wstr.find(' ', start_x),
					wstr.find("\r\n", start_x),
					wstr.find('\n', start_x),
					wstr.find('\t', start_x),
					wstr.find('>', start_x),
					wstr.find('/', start_x)
				});

			//Determinate the KEY
			tag = wstr.substring(start_x, end_x).str();
		};
		
		void construct_properties(WeakString &wstr)
		{
			// INLINE CLASS
			class QuotePair
			{
			public:
				enum TYPE : int
				{
					SINGLE = 1,
					DOUBLE = 2
				};

				TYPE type;
				size_t start_index;
				size_t end_index;

				QuotePair(TYPE type, size_t start_index, size_t end_index)
				{
					this->type = type;
					this->start_index = start_index;
					this->end_index = end_index;
				};
			};

			size_t i_begin = wstr.find('<' + tag) + tag.size() + 1;
			size_t end_slash = wstr.rfind('/');
			size_t end_block = wstr.find('>', i_begin);

			size_t i_end = calc_min_index({ end_slash, end_block });
			if (i_end == std::string::npos || i_begin >= i_end)
				return;

			//<comp label='ABCD' /> : " label='ABCD' "
			WeakString &line = wstr.substring(i_begin, i_end);

			if (line.find('=') == std::string::npos)
				return;

			std::string label, value;
			std::vector<QuotePair*> helpers;
			bool in_quote = false;
			QuotePair::TYPE type;
			size_t start_point, equal_point;
			size_t i;

			for (i = 0; i < line.size(); i++)
			{
				//Start of quote
				if (in_quote == false && (line[i] == '\'' || line[i] == '"'))
				{
					in_quote = true;
					start_point = i;

					if (line[i] == '\'')
						type = QuotePair::SINGLE;
					else if (line[i] == '"')
						type = QuotePair::DOUBLE;
				}
				else if
					(
						in_quote == true &&
						(
						(type == QuotePair::SINGLE && line[i] == '\'') ||
							(type == QuotePair::DOUBLE && line[i] == '"')
							)
						)
				{
					helpers.push_back(new QuotePair(type, start_point, i));
					in_quote = false;
				}
			}
			for (i = 0; i < helpers.size(); i++)
			{
				if (i == 0)
				{
					equal_point = (long long)line.find('=');
					label = line.substring(0, equal_point).trim().str();
				}
				else
				{
					equal_point = line.find('=', helpers[i - 1]->end_index + 1);
					label = line.substring(helpers[i - 1]->end_index + 1, equal_point).trim().str();
				}

				value = decodeProperty
					(
						line.substring
						(
							helpers[i]->start_index + 1,
							helpers[i]->end_index
						)
					);

				//INSERT INTO PROPERTY_MAP
				properties.set(label, value);
			}
			for (i = 0; i < helpers.size(); i++)
				delete helpers[i];
		};
		
		auto construct_value(WeakString &wstr) -> bool
		{
			size_t end_slash = wstr.rfind('/');
			size_t end_block = wstr.find('>');

			if (end_slash < end_block || end_block + 1 == wstr.rfind('<'))
			{
				//STATEMENT1: <TAG />
				//STATEMENT2: <TAG></TAG> -> SAME WITH STATEMENT1: <TAG />
				value.clear();
				return false;
			}

			size_t start_x = end_block + 1;
			size_t end_x = wstr.rfind('<');
			wstr = wstr.substring(start_x, end_x); //REDEFINE WEAK_STRING -> IN TO THE TAG

			if (wstr.find('<') == std::string::npos)
				value = wstr.trim().str();
			else
				value.clear();

			return true;
		};
		
		void construct_children(WeakString &wstr)
		{
			if (wstr.find('<') == std::string::npos)
				return;

			size_t start_x = wstr.find('<');
			size_t end_x = wstr.rfind('>') + 1;
			wstr = wstr.substring(start_x, end_x);

			int block_start_cnt = 0;
			int block_end_cnt = 0;
			size_t start = 0;
			size_t end;
			size_t i;

			//FIND BLOCKS, CREATES XML AND PUT IN TEMPORARY CONTAINER
			for (i = 0; i < wstr.size(); i++)
			{
				if (wstr[i] == '<' && wstr.substr(i, 2) != "</")
					block_start_cnt++;
				else if (wstr.substr(i, 2) == "/>" || wstr.substr(i, 2) == "</")
					block_end_cnt++;

				if (block_start_cnt >= 1 && block_start_cnt == block_end_cnt)
				{
					//NO PROBLEM TO AVOID COMMENT
					end = wstr.find('>', i);

					std::shared_ptr<XML> xml(new XML(this, wstr.substring(start, end + 1)));
					push_back(xml);

					i = end; //WHY NOT END+1? 
					start = end + 1;
					block_start_cnt = 0;
					block_end_cnt = 0;
				}
			}

			if (size() > 0)
				value.clear();
		};

	public:
		/**
		 * @brief Add children xml objects by string representing them
		 *
		 * @param str A string representing xml objects whould be belonged to this XML
		 */
		void push_back(const WeakString &wstr)
		{
			if (wstr.empty() == true)
				return;

			std::shared_ptr<XML> xml(new XML(this, (WeakString&)wstr));
			auto it = find(xml->tag);

			//if not exists
			if (it == end())
			{
				set(xml->tag, std::make_shared<XMLList>());
				it = find(xml->tag);
			}

			//insert
			it->second->push_back(xml);
		};

		/**
		 * @brief Add children xml
		 *
		 * @param xml An xml object you want to add
		 */
		void push_back(const std::shared_ptr<XML> xml)
		{
			std::string &tag = xml->tag;
			if (this->has(tag) == false)
				set(tag, std::make_shared<XMLList>());

			this->get(tag)->push_back(xml);
		};

		/**
		 * @brief Add all properties from another XML
		 *
		 * @details
		 * \par Copies all properties from target to here.
		 *
		 * @warning Not a category of assign, but an insert.
		 * @param xml Target xml object to deliver its properties
		 */
		void addAllProperties(const std::shared_ptr<XML> xml)
		{
			for (auto it = xml->properties.begin(); it != xml->properties.end(); it++)
				properties[it->first] = it->second;
		};

		/* -----------------------------------------------------------
			SETTERS
		----------------------------------------------------------- */
		/**
		 * @brief Set tag (identifier) of the XML
		 *
		 * @see XML::tag
		 */
		void setTag(const std::string &val)
		{
			tag = val;
		};

		/**
		 * @brief Set value of the XML
		 *
		 * @tparam _Ty Type of the value
		 * @param val The value to set
		 *
		 * @warning Do not abuse values for expressing member variables
		 * <table>
		 *	<tr>
		 *		<th>Standard Usage</th>
		 *		<th>Non-standard usage abusing value</th>
		 *	</tr>
		 *	<tr>
		 *		<td>
		 *			\<memberList\>\n
		 *			&nbsp;&nbsp;&nbsp;&nbsp;\<member id='jhnam88' name='Jeongho+Nam' birthdate='1988-03-11' /\>\n
		 *			&nbsp;&nbsp;&nbsp;&nbsp;\<member id='master' name='Administartor' birthdate='2011-07-28' /\>\n
		 *			\</memberList\>
		 *		</td>
		 *		<td>
		 *			\<member\>\n
		 *				\<id\>jhnam88\</id\>\n
		 *				\<name\>Jeongho+Nam\</name\>\n
		 *				\<birthdate\>1988-03-11\</birthdate\>\n
		 *			\</member\>
		 *		</td>
		 *	</tr>
		 * </table>
		 */
		template <typename T>
		void setValue(const T &val)
		{
			value = std::to_string(val);
		};
		template<> void setValue(const std::string &val)
		{
			this->value = val;
		};
		template<> void setValue(const WeakString &val)
		{
			this->value = val.str();
		};

		template <typename T>
		void insertValue(const std::string &tag, const T &val)
		{
			std::shared_ptr<XML> xml(new XML());
			xml->setTag(tag);
			xml->setValue(val);

			push_back(xml);
		};

		/**
		 * @brief Set a property with its key
		 */
		template<typename T>
		void setProperty(const std::string &key, const T &val)
		{
			properties.set(key, std::to_string(val));
		};
		template<> void setProperty(const std::string &key, const std::string &val)
		{
			properties.set(key, val);
		};
		template<> void setProperty(const std::string &key, const WeakString &val)
		{
			properties.set(key, val.str());
		};

		/**
		 * @brief Erase a property by its key
		 *
		 * @param key The key of the property to erase
		 * @throw exception Unable to find the element
		 */
		void eraseProperty(const std::string &key)
		{
			properties.erase(key);
		};

		/**
		 * @brief Remove all properties in the XML
		 */
		void clearProperties()
		{
			properties.clear();
		};

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
	public:
		/**
		 * @brief Get key; identifer of the XML
		 *
		 * @return tag, identifer of the XML
		 * @see XML::tag
		 */
		auto getTag() const -> std::string
		{
			return tag;
		};

		/**
		 * @brief Get value of the XML
		 */
		template<class T = std::string> auto getValue() const -> T
		{
			double val = std::stod(value);

			return (T)val;
		};

		template<> auto getValue() const -> std::string
		{
			return value;
		};
		template<> auto getValue() const -> WeakString
		{
			return value;
		};

		/**
		 * @brief Get property
		 */
		template<class T = std::string> auto getProperty(const std::string &key) const -> T
		{
			double val = std::stod(properties.get(key));

			return (T)val;
		};

		template<> auto getProperty(const std::string &key) const -> std::string
		{
			return properties.get(key);
		};
		template<> auto getProperty(const std::string &key) const -> WeakString
		{
			return properties.get(key);
		};

		/**
		 * @brief Test wheter a property exists or not
		 */
		auto hasProperty(const std::string &key) const -> bool
		{
			return properties.has(key);
		};

		/**
		 * @brief Get properties
		 */
		auto getProperties() const -> const HashMap<std::string, std::string>&
		{
			return properties;
		};

		/* -----------------------------------------------------------
			FILTERS
		----------------------------------------------------------- */
	private:
		auto calc_min_index(const std::vector<size_t> &vec) const -> size_t
		{
			size_t val = std::string::npos;
			for (size_t i = 0; i < vec.size(); i++)
				if (vec[i] != std::string::npos && vec[i] < val)
					val = vec[i];

			return val;
		};

		auto encode_value(const WeakString &wstr) const -> std::string
		{
			static std::vector<std::pair<std::string, std::string>> pairArray =
			{
				{ "&", "&amp;" },
				{ "<", "&lt;" },
				{ ">", "&gt;" },
				{ "\"", "&quot;" },
				{ "'", "&apos;" },
				{ "\t", "&#x9;" }, //9
				{ "\n", "&#xA;" }, //10
				{ "\r", "&#xD;" } //13
			};

			return wstr.replaceAll(pairArray);
		};
		
		auto decode_value(const WeakString &wstr) const -> std::string
		{
			static std::vector<std::pair<std::string, std::string>> pairArray =
			{
				{ "&amp;", "&" },
				{ "&lt;", "<" },
				{ "&gt;", ">" },
				{ "&quot;", "\"" },
				{ "&apos;", "'" },
				{ "&#x9;", "\t" }, //9
				{ "&#xA;", "\n" }, //10
				{ "&#xD;", "\r" } //13
			};

			return wstr.replaceAll(pairArray);
		};
		
		auto encode_property(const WeakString &wstr) const -> std::string
		{
			static std::vector<std::pair<std::string, std::string>> pairArray =
			{
				{ "&", "&amp;" },
				{ "<", "&lt;" },
				{ ">", "&gt;" }
			};

			return wstr.trim().replaceAll(pairArray);
		};
		
		auto decodeProperty(const WeakString &wstr) const -> std::string
		{
			static std::vector<std::pair<std::string, std::string>> pairArray =
			{
				{ "&amp;", "&" },
				{ "&lt;", "<" },
				{ "&gt;", ">" }
			};

			return wstr.replaceAll(pairArray);
		};

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
	public:
		/**
		 * @brief Get the string content.
		 * @details Returns a string representation of the XML and its all children.
		 *
		 * @return A string represents the XML.
		 */
		auto toString(size_t level = 0) const -> std::string
		{
			// KEY
			std::string str = std::string(level, '\t') + "<" + tag;

			// PROPERTIES
			for (auto it = properties.begin(); it != properties.end(); it++)
				str += " " + it->first + "=\"" + encode_property(it->second) + "\"";

			if (this->empty() == true)
			{
				// VALUE
				if (value.empty() == true)
					str += " />";
				else
					str += ">" + encode_value(value) + "</" + tag + ">";
			}
			else
			{
				// CHILDREN
				str += ">\n";

				for (auto it = begin(); it != end(); it++)
					for (size_t i = 0; i < it->second->size(); i++)
						str += it->second->at(i)->toString(level + 1);

				str += std::string(level, '\t') + "</" + tag + ">";
			}

			return str + "\n";
		};
	};
};
};