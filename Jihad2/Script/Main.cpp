
# include "FileRegister.h"
# include "Title.h"
# include "Game.h"
# include "Explain.h"
# include "Result.h"



void Main()
{
	Window::Resize(1500, 800);
	//Graphics::SetBackground(Palette::Skyblue);

	TextureRegisterer().fileRegister(L"Files/").preload();
	
	MySceneManager manager;
	manager.add<Title>(SceneName::Title);
	manager.add<Game>(SceneName::Game);
	manager.add<Explain>(SceneName::Explain);
	manager.add<Result>(SceneName::Result);
	
	Eye::setExplosionGIF(L"Files/bullet/eye_exp.gif");

	while (System::Update())
	{
		manager.updateAndDraw();
	}
}