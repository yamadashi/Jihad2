
# include "FileRegister.h"
# include "Game.h"



void Main()
{
	Window::Resize(1500, 800);
	//Graphics::SetBackground(Palette::White);

	TextureRegisterer().fileRegister(L"Files/").preload();
	

	MySceneManager manager;
	manager.add<Game>(SceneName::Game);
	
	while (System::Update())
	{
		manager.updateAndDraw();
	}
}
