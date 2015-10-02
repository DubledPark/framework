#pragma once
#include <samchon/protocol/Entity.hpp>
#include <samchon/protocol/IEntityGroup.hpp>

#include <samchon/library/XML.hpp>

namespace samchon
{
	namespace protocol
	{
		/**
		 * @brief An Entity and a container of children Entity objects.
		 *
		 * @tparam _Container A type of container containing children entity objects.
		 * @tparam _Ety A type of children entity. It must be a class derived from an Entity class, or Entity class itself.
		 * @tparam _Ty A type of children element of _Container. Using default template parameter is recommended.
		 *
		 * @details
		 * <p> EntityGroup is a template class for containinig children Entity objects, and also another type 
		 * of an Entity, too. You can realize hierarchical relationship. Although some entities have complicated
		 * hierarchical relationship, you can deduct a optimal solution easily with EntityGroup and Entity. </p>
		 *
		 * <p> If an entity has some subordinate entities of same type, they are in "Composite relationship". 
		 * Make the entity to be EmntityGroup and subordinate entities to be children of the entity. When
		 * those relationships are continued, continue to create classes dervied from EntityGroup. When those
		 * relationshiop meets a terminal node, then make the terminal node to be an Entity. </p>
		 *
		 * <p> @image html  cpp/example_inspect.png
		 * @image latex cpp/example_inspect.png </p>
		 * 
		 * <p> EntityGroup is an Entity, and a container of children Entity objects at the same time. If
		 * children type, of a class derived from an EntityGroup, is itself, you can realize hierarchical
		 * and recursive relationship. The relationship is called as "Composite pattern". </p>
		 *
		 *	\li FTFolder extends FTInstance and SharedEntityArray<FTInstance>
		 *	\li NTCriteria extends SharedEntityArray<NTCriteria>
		 *
		 * @note
		 * <p> As a freelancer developer and architect I am, I even design DB I/O to follow the format 
		 * representing Entity and EntityGroup by XML. Below T-SQL script also follows the standard format 
		 * of expressing Entity with XML by procedure and "FOR XML AUTO" statement. </p>
		 * 
		 * @includelineno entity/goCandidateInspectList.sql
		 *
		 * \par Inherited
		 *		@copydoc protocol::Entity
		 */
		template <typename _Container, typename _ETy = Entity, typename _Ty = _Container::value_type>
		class EntityGroup
			: public _Container, public virtual Entity, //CLASS
			public virtual IEntityGroup	//INTERFACE
		{
		protected:
			typedef _Container container_type;
			typedef _Ty value_type;
			typedef _ETy entity_type;

		public:
			/* ------------------------------------------------------------------------------------
				CONSTRUCTORS
			------------------------------------------------------------------------------------ */
			/**
			 * @brief Default Constructor.
			 */
			EntityGroup()
				: _Container(), Entity(), 
				IEntityGroup()
			{
			};
			virtual ~EntityGroup() = default;

			/**
			 * @brief Construct data of the Entity from an XML object
			 *
			 * <p> Constructs the EntityGroup's own member variables only from the input XML object. </p>
			 *
			 * <p> Do not consider about constructing children Entity objects' data in EntityGroup::construct(). 
			 * Those children Entity objects' data will constructed by their own construct() method. Even insertion of XML objects
			 * representing children are done by abstract method of EntityGroup::toXML(). </p>
			 *
			 * <p> Constructs only data of EntityGroup's own. </p>
			 *
			 * \par Inherited
			 *		@copydoc Entity::construct()
			 */
			virtual void construct(std::shared_ptr<library::XML> xml)
			{
				clear();
				if (xml->has(CHILD_TAG()) == false)
					return;

				std::shared_ptr<library::XMLList> &xmlList = xml->get(CHILD_TAG());

				if (std::is_same<_Container, std::vector<_Container::value_type, _Container::allocator_type>>::value == true)
				{
					//FOR RESERVE
					assign(xmlList->size(), nullptr);
					erase(begin(), end());
				}

				for (size_t i = 0; i < xmlList->size(); i++)
				{
					std::shared_ptr<library::XML> &xmlElement = xmlList->at(i);

					entity_type *entity = createChild(xmlElement);
					if (entity != nullptr)
					{
						entity->construct(xmlList->at(i));
						emplace_back(entity);
					}
 				}
			};

		protected:
			/**
			 * @brief Factory method of a child Entity.
			 *
			 * @details
			 * <p> EntityGroup::createChild() is a factory method creating a new child Entity which is belonged 
			 * to the EntityGroup. This method is called by EntityGroup::construct(). The children construction
			 * methods Entity::construct() will be called by abstract method of the EntityGroup::construct(). </p>
			 *
			 * @return A new child Entity belongs to EntityGroup.
			 */
			virtual auto createChild(std::shared_ptr<library::XML>) -> entity_type* = 0;
			
			/* ------------------------------------------------------------------------------------
				GETTERS
			------------------------------------------------------------------------------------ */
		public:
			/**
			* @brief Indicates whether a container has an object having the specified identifier. </p>
			* 
			* @param key An identifier of an Entity
			* @return If there's the object then true, otherwise false
			*/
			auto has(const std::string &key) const -> bool
			{
				for (auto it = begin(); it != end(); it++)
					if ((*it)->key() == key)
						return true;

				return false;
			};

			/**
			* @brief Access the element by specified identifier(key).
			* 
			* @param key the identifier of the element wants to access
			* @return The element having the key, or throw exception if there is none.
			*/
			auto get(const std::string &key) -> value_type&
			{
				for (auto it = begin(); it != end(); it++)
				if ((*it)->key() == key)
					return *it;

				throw std::exception("out of range");
			};

			/**
			* @brief Access the const element by specified identifier(key).
			* 
			* @param key the identifier of the element wants to access
			* @return The const element having the key, or throw exception if there is none.
			*/
			auto get(const std::string &key) const -> const value_type&
			{
				for (auto it = begin(); it != end(); it++)
					if ((*it)->key() == key)
						return *it;

				throw std::exception("out of range");
			};

			/**
			 * @brief Get an XML object represents the EntityGroup
			 *
			 * <p> Archives the EntityGroup's own member variables only to the returned XML object. </p>
			 *
			 * <p> Do not consider about archiving children Entity objects' data in EntityGroup::toXML(). 
			 * Those children Entity objects will converted to XML object by their own toXML() method. The 
			 * insertion of XML objects representing children are done by abstract method of 
			 * EntityGroup::toXML(). </p>
			 *
			 * <p> Archives only data of EntityGroup's own. </p>
			 *
			 * \par Inherited
			 *		@copydoc Entity::toXML()
			 */
			virtual auto toXML() const -> std::shared_ptr<library::XML>
			{
				std::shared_ptr<library::XML> &xml = Entity::toXML();
				
				std::shared_ptr<library::XMLList> xmlList(new library::XMLList());
				xmlList->reserve(this->size());

				for (auto it = begin(); it != end(); it++)
					xmlList->push_back( (*it)->toXML() );

				xml->set(CHILD_TAG(), xmlList);
 				return xml;
			};
		};
	};
};