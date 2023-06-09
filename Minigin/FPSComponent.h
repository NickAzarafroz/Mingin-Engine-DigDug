#pragma once
#include <SDL.h>
#include <string>
#include "BaseComponent.h"

namespace dae
{
	class TextComponent;
	class FPSComponent : public BaseComponent
	{
	public:
		FPSComponent();
		~FPSComponent();

		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

		virtual void Start() override;
		virtual void Update(float elapsedSec) override;
		virtual void Render() const override;
		virtual void ReceiveMessage(int message) override;

	private:
		int m_Fps{};
		int m_FrameCount{};
		Uint32 m_LastFrameTime{};
		TextComponent* m_pText{};
	};

}


