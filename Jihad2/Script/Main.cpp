
# include "FileRegister.h"
# include "Title.h"
# include "Game.h"



void Main()
{
	Window::Resize(1500, 800);
	//Graphics::SetBackground(Palette::Skyblue);

	TextureRegisterer().fileRegister(L"Files/").preload();
	
	MySceneManager manager;
	manager.add<Title>(SceneName::Title);
	manager.add<Game>(SceneName::Game);
	
	while (System::Update())
	{
		manager.updateAndDraw();
		PutText(Mouse::Pos()).at(Mouse::Pos());
	}
}