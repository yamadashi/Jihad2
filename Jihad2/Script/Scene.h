#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>


struct SharedData {
	bool cleared = false;

	void reset() {}
};


enum class SceneName : int {
	Title = 0,
	Explain,
	Game,
	Result,
	Exit
};


using MySceneManager = SceneManager<SceneName, SharedData>;
using MyScene = SceneManager<SceneName, SharedData>::Scene;
