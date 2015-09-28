#pragma once
#include <samchon/API.hpp>

#include <samchon/library/CaseGenerator.hpp>

namespace samchon
{
	namespace library
	{
		/**
		 * @brief A permutation case generator
		 * @details 
		 * <p> nPr </p>
		 *
		 * @image html cpp/subset/library_case_generator.png
		 * @image latex cpp/subset/library_case_generator.png
		 */
		class SAMCHON_FRAMEWORK_API PermutationGenerator
			: public CaseGenerator
		{
		private:
			typedef CaseGenerator super;

		public:
			/**
			 * @brief Construct from size of N and R
			 *
			 * @copydoc CaseGenerator::CaseGenerator()
			 */
			PermutationGenerator(size_t n, size_t r);
			virtual ~PermutationGenerator() = default;

			virtual auto operator[](size_t) const -> std::vector<size_t> override;
		};
	};
};