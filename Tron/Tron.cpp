#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "InputManager.h"

#include "TransformComponent.h"
#include "TextureComponent.h"
#include "FPSComponent.h"
#include "RotateComponent.h"
#include "LiveDisplayComponent.h"
//#include "ScoreDisplayComponent.h"
#include "GridComponent.h"
#include "BoxTriggerComponent.h"
#include "AIComponent.h"

#include "AllCommands.h"
#include "XBox360Controller.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& input = dae::InputManager::GetInstance();

	auto pDigDugFont = dae::ResourceManager::GetInstance().LoadFont("DigDug_Font.ttf", 14);
	auto pFontFpsText = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	auto goLogo = std::make_shared<dae::GameObject>(&scene);
	auto goHighScoreText = std::make_shared<dae::GameObject>(&scene);
	auto goPlayer1Text = std::make_shared<dae::GameObject>(&scene);
	auto goFPS = std::make_shared<dae::GameObject>(&scene);
	auto goPlayer = std::make_shared<dae::GameObject>(&scene);
	auto goEnemy = std::make_shared<dae::GameObject>(&scene);
	auto goGrid = std::make_shared<dae::GameObject>(&scene);
	auto goHealth = std::make_shared<dae::GameObject>(&scene);
	auto goPlayerTurret = std::make_shared<dae::GameObject>(&scene);

	//Logo Texture
	//-----------------------------------------------------------------------
	//goLogo->AddComponent<dae::TextureComponent>()->SetTexture("logo.tga");
	//goLogo->SetLocalPosition(glm::vec3(216.f, 180.f, 0.f));
	//--------------------------------------------------------------------------

	//HighScore Text
	//--------------------------------------------------------------------------
	goHighScoreText->AddComponent<dae::TextComponent>()->SetText("HIGH SCORE");
	goHighScoreText->GetComponent<dae::TextComponent>()->SetFont(pDigDugFont);
	goHighScoreText->GetComponent<dae::TextComponent>()->SetFontColor(255, 0, 0);

	goHighScoreText->AddComponent<dae::TransformComponent>()->SetPosition(140.f, 30.f, 0.f);
	//--------------------------------------------------------------------------

	//Player 1 Text
	//--------------------------------------------------------------------------
	goPlayer1Text->AddComponent<dae::TextComponent>()->SetText("P1");
	goPlayer1Text->GetComponent<dae::TextComponent>()->SetFont(pDigDugFont);
	goPlayer1Text->GetComponent<dae::TextComponent>()->SetFontColor(255, 0, 0);

	goPlayer1Text->AddComponent<dae::TransformComponent>()->SetPosition(60.f, 30.f, 0.f);
	//--------------------------------------------------------------------------

	//FPS
	//--------------------------------------------------------------------------
	goFPS->AddComponent<dae::FPSComponent>();
	goFPS->AddComponent<dae::TextComponent>();
	goFPS->GetComponent<dae::TextComponent>()->SetFont(pDigDugFont);
	goFPS->GetComponent<dae::TextComponent>()->SetFontColor(0, 255, 0);

	goFPS->AddComponent<dae::TransformComponent>()->SetPosition(5.f, 5.f, 0.f);
	//--------------------------------------------------------------------------

	//Grid
	//---------------------------------------------------------------------------
	goGrid->AddComponent<dae::TextureComponent>();
	goGrid->GetComponent<dae::TextureComponent>()->AddTexture("path.png");
	goGrid->GetComponent<dae::TextureComponent>()->AddTexture("wall.png");

	goGrid->AddComponent<dae::GridComponent>();
	goGrid->GetComponent<dae::GridComponent>()->Initialize(32.f, 32.f, "../Data/LevelDigDug.json");
	//---------------------------------------------------------------------------

	//Player
	//--------------------------------------------------------------------------
	goPlayer->AddComponent<dae::TextureComponent>()->SetTexture("RedTank.png");
	goPlayer->AddComponent<dae::BoxTriggerComponent>()->SetSize(32.f, 32.f);
	goPlayer->AddComponent<dae::PlayerComponent>();
	goPlayer->AddComponent<dae::TransformComponent>()->SetPosition(0.f, 96.f, 0.0f);
	//---------------------------------------------------------------------------

	//Enemy
	//---------------------------------------------------------------------------
	goEnemy->AddComponent<dae::TextureComponent>()->SetTexture("BlueTank.png");
	goEnemy->AddComponent<dae::BoxTriggerComponent>()->SetSize(32.f, 32.f);
	goEnemy->AddComponent<dae::PlayerComponent>();
	//goEnemy->AddComponent<dae::AIComponent>()->SetObjectToMoveTo(goGrid->GetComponent<dae::GridComponent>());

	goEnemy->AddComponent<dae::TransformComponent>()->SetPosition(0.f, 320.f, 0.0f);
	//---------------------------------------------------------------------------

	//Health Displayer
	//---------------------------------------------------------------------------
	goHealth->AddComponent<dae::TextComponent>()->SetFont(pDigDugFont);
	goHealth->GetComponent<dae::TextComponent>()->SetFontColor(255, 0, 255);

	goHealth->AddComponent<dae::LiveDisplayComponent>()->SetObjectToDisplayLives(goPlayer.get());
	goHealth->AddComponent<dae::TransformComponent>()->SetPosition(0.f, 50.f, 0.f);

	//---------------------------------------------------------------------------

	//Player Turret
	//-----------------------------------------------------------------------------------
	goPlayerTurret->AddComponent<dae::TextureComponent>()->AddTexture("RedTankGun.png");
	goPlayerTurret->AddComponent<dae::TransformComponent>()->SetPosition(-5.f, 88.f, 0.f);
	//-----------------------------------------------------------------------------------

	auto gridRight = std::make_unique<dae::MoveGridCommand>(goPlayer.get(), glm::vec2{ 1.f, 0.f }, goGrid->GetComponent<dae::GridComponent>());
	auto gridLeft = std::make_unique<dae::MoveGridCommand>(goPlayer.get(), glm::vec2{ -1.f, 0.f }, goGrid->GetComponent<dae::GridComponent>());
	auto gridUp = std::make_unique<dae::MoveGridCommand>(goPlayer.get(), glm::vec2{ 0.f, -1.f }, goGrid->GetComponent<dae::GridComponent>());
	auto gridDown = std::make_unique<dae::MoveGridCommand>(goPlayer.get(), glm::vec2{ 0.f, 1.f }, goGrid->GetComponent<dae::GridComponent>());

	auto incTurret = std::make_unique<dae::MoveTurretCommand>(goPlayerTurret.get(), true);
	auto decTurret = std::make_unique<dae::MoveTurretCommand>(goPlayerTurret.get(), false);

	auto spawnBullet = std::make_unique<dae::SpawnBulletCommand>(goPlayer.get(), goEnemy.get(), goGrid->GetComponent<dae::GridComponent>());

	input.BindCommandKeyBoard(SDL_Scancode::SDL_SCANCODE_D, std::move(gridRight), 0);
	input.BindCommandKeyBoard(SDL_Scancode::SDL_SCANCODE_A, std::move(gridLeft), 0);
	input.BindCommandKeyBoard(SDL_Scancode::SDL_SCANCODE_S, std::move(gridDown), 0);
	input.BindCommandKeyBoard(SDL_Scancode::SDL_SCANCODE_W, std::move(gridUp), 0);
	input.BindCommandKeyBoard(SDL_Scancode::SDL_SCANCODE_SPACE, std::move(spawnBullet), 1);
	input.BindCommandKeyBoard(SDL_Scancode::SDL_SCANCODE_UP, std::move(incTurret), 1);
	input.BindCommandKeyBoard(SDL_Scancode::SDL_SCANCODE_DOWN, std::move(decTurret), 1);

	auto p0 = std::make_unique<dae::XBox360Controller>(0);

	auto moveRightCommand = std::make_unique<dae::MoveGridCommand>(goPlayer.get(), glm::vec2{ 1.f, 0.f }, goGrid->GetComponent<dae::GridComponent>());
	auto moveLeftCommand = std::make_unique<dae::MoveGridCommand>(goPlayer.get(), glm::vec2{ -1.f, 0.f }, goGrid->GetComponent<dae::GridComponent>());
	auto moveUpCommand = std::make_unique<dae::MoveGridCommand>(goPlayer.get(), glm::vec2{ 0.f, -1.f }, goGrid->GetComponent<dae::GridComponent>());
	auto moveDownCommand = std::make_unique<dae::MoveGridCommand>(goPlayer.get(), glm::vec2{ 0.f, 1.f }, goGrid->GetComponent<dae::GridComponent>());

	input.AddController(std::move(p0));

	input.BindCommandController(0, dae::XBox360Controller::ControllerButton::DPadRight, std::move(moveRightCommand));
	input.BindCommandController(0, dae::XBox360Controller::ControllerButton::DPadLeft, std::move(moveLeftCommand));
	input.BindCommandController(0, dae::XBox360Controller::ControllerButton::DPadUp, std::move(moveUpCommand));
	input.BindCommandController(0, dae::XBox360Controller::ControllerButton::DPadDown, std::move(moveDownCommand));

	goPlayer->SendMessageID(0);

	goPlayerTurret->SetParent(goPlayer.get(), true);

	goGrid->AddGameObject();
	goHighScoreText->AddGameObject();
	goPlayer1Text->AddGameObject();
	goFPS->AddGameObject();
	goPlayer->AddGameObject();
	goHealth->AddGameObject();
	goEnemy->AddGameObject();
}

int main(int, char*[]) 
{
	dae::Minigin engine("../Data/");
	engine.Run(load);

    return 0;
}