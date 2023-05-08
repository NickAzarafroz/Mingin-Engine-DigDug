#pragma once
#include "BaseComponent.h"
#include "Subject.h"
namespace dae
{
	class PlayerComponent : public BaseComponent
	{
	public:
		PlayerComponent() = default;
		virtual void Start() override;
		virtual void Update(float elapsedSec) override;
		virtual void Render() const override;

		int GetHealth() const;
		void TakeDamage(int amount);
		~PlayerComponent();

		Subject<> healthChanged;

	private:
		int m_Health;
	};
}


