#pragma once
#include "BaseComponent.h"
#include "glm/glm.hpp"
namespace dae
{
	struct Cell;
	class GridComponent;
	class AIComponent : public BaseComponent
	{
	public:
		AIComponent() = default;
		~AIComponent() = default;

		AIComponent(const AIComponent& other) = delete;
		AIComponent(AIComponent&& other) = delete;
		AIComponent& operator=(const AIComponent& other) = delete;
		AIComponent& operator=(AIComponent&& other) = delete;

		virtual void Update(float elapsedSec) override;

		void SetObjectToMoveTo(GridComponent* pGrid);
		void MoveToObject();

		static bool m_MovementFlag;
		static bool m_IsValid;
		static glm::vec2 m_TempDir;
		static Cell m_Cell;
		static Cell m_DestinationCell;

	private:
		glm::vec2 m_Dir{0.f, 1.f};
		GridComponent* m_pGrid{};
	};
}
