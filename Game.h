#pragma once
#include "SDL/SDL.h"

struct Vector2 {
	float x;
	float y;
};

class Game{
public :
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;

	int mPaddleDir1;
	Vector2 mPaddlePos1;
	int mPaddleDir2;
	Vector2 mPaddlePos2;
	Vector2 mBallPos;
	Vector2 mBallVel;
};