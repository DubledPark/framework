#pragma once
#include <samchon/API.hpp>

#include <samchon/library/FTFile.hpp>

#include <samchon/ByteArray.hpp>

namespace samchon
{
	namespace library
	{
		 /**
		  * @brief A binary file
		  * @details A file having data as binary.
		  *
		  * @image html cpp/library_file_tree.png
		  * @image latex cpp/library_file_tree.png
		  *
		  * @author Jeongho Nam
		  */
		class SAMCHON_FRAMEWORK_API FTByteFile
			: public FTFile
		{
		private:
			typedef protocol::Entity FTFile;

		protected:
			/**
			 * @brief Binary data of the file
			 */
			ByteArray data;

		public:
			/**
			 * @copydoc FTFile::FTFile()
			 */
			FTByteFile(FTFolder*);
			virtual ~FTByteFile() = default;

			virtual void construct(std::shared_ptr<library::XML> xml);

			/**
			 * @brief Get binaray data of the file.
			 */
			auto getData() -> ByteArray;

			virtual auto toXML() const -> std::shared_ptr<library::XML>;
		};
	};
};